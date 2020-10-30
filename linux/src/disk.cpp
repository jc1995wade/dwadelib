#include <cctype>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <scsi/sg.h>
#include <sys/ioctl.h>
#include <linux/hdreg.h>
#include <string>
#include <fstream>
 
static bool get_disk_name(std::string & disk_name)
{
    disk_name.c_str();
 
    std::ifstream ifs("/etc/mtab", std::ios::binary);
    if (!ifs.is_open())
    {
        return(false);
    }
 
    char line[4096] = { 0 };
    while (!ifs.eof())
    {
        ifs.getline(line, sizeof(line));
        if (!ifs.good())
        {
            break;
        }
 
        const char * disk = line;
        while (isspace(disk[0]))
        {
            ++disk;
        }
 
        const char * space = strchr(disk, ' ');
        if (NULL == space)
        {
            continue;
        }
 
        const char * mount = space + 1;
        while (isspace(mount[0]))
        {
            ++mount;
        }
        if ('/' != mount[0] || ' ' != mount[1])
        {
            continue;
        }
 
        while (space > disk && isdigit(space[-1]))
        {
            --space;
        }
 
        if (space > disk)
        {
            std::string(disk, space).swap(disk_name);
            break;
        }
    }
 
    ifs.close();
 
    return(!disk_name.empty());
}
 
static void trim_serial(const void * serial, size_t serial_len, std::string & serial_no)
{
    const char * serial_s = static_cast<const char *>(serial);
    const char * serial_e = serial_s + serial_len;
    while (serial_s < serial_e)
    {
        if (isspace(serial_s[0]))
        {
            ++serial_s;
        }
        else if ('\0' == serial_e[-1] || isspace(serial_e[-1]))
        {
            --serial_e;
        }
        else
        {
            break;
        }
    }
 
    if (serial_s < serial_e)
    {
        std::string(serial_s, serial_e).swap(serial_no);
    }
}
 
static bool get_disk_serial_by_way_1(const std::string & disk_name, std::string & serial_no)
{
    serial_no.clear();
 
    int fd = open(disk_name.c_str(), O_RDONLY);
    if (-1 == fd)
    {
        return(false);
    }
 
    struct hd_driveid drive = { 0 };
    if (0 == ioctl(fd, HDIO_GET_IDENTITY, &drive))
    {
        trim_serial(drive.serial_no, sizeof(drive.serial_no), serial_no);
    }
 
    close(fd);
 
    return(!serial_no.empty());
}
 
static bool scsi_io(
                int fd, unsigned char * cdb, 
                unsigned char cdb_size, int xfer_dir, 
                unsigned char * data, unsigned int data_size, 
                unsigned char * sense, unsigned int sense_len
            )
{
    sg_io_hdr_t io_hdr = { 0 };
    io_hdr.interface_id = 'S';
    io_hdr.cmdp = cdb;
    io_hdr.cmd_len = cdb_size;
    io_hdr.sbp = sense;
    io_hdr.mx_sb_len = sense_len;
    io_hdr.dxfer_direction = xfer_dir;
    io_hdr.dxferp = data;
    io_hdr.dxfer_len = data_size;
    io_hdr.timeout = 5000;
 
    if (ioctl(fd, SG_IO, &io_hdr) < 0)
    {
        return(false);
    }
 
    if (SG_INFO_OK != (io_hdr.info & SG_INFO_OK_MASK) && io_hdr.sb_len_wr > 0)
    {
        return(false);
    }
 
    if (io_hdr.masked_status || io_hdr.host_status || io_hdr.driver_status)
    {
        return(false);
    }
 
    return(true);
}
 
static bool get_disk_serial_by_way_2(const std::string & disk_name, std::string & serial_no)
{
    serial_no.clear();
 
    int fd = open(disk_name.c_str(), O_RDONLY);
    if (-1 == fd)
    {
        return(false);
    }
 
    int version = 0;
    if (ioctl(fd, SG_GET_VERSION_NUM, &version) < 0 || version < 30000)
    {
        close(fd);
        return(false);
    }
 
    const unsigned int data_size = 0x00ff;
    unsigned char data[data_size] = { 0 };
    const unsigned int sense_len = 32;
    unsigned char sense[sense_len] = { 0 };
    unsigned char cdb[] = { 0x12, 0x01, 0x80, 0x00, 0x00, 0x00 };
    cdb[3] = (data_size >> 8) & 0xff;
    cdb[4] = (data_size & 0xff);
 
    if (scsi_io(fd, cdb, sizeof(cdb), SG_DXFER_FROM_DEV, data, data_size, sense, sense_len))
    {
        int page_len = data[3];
        trim_serial(data + 4, page_len, serial_no);
    }
 
    close(fd);
 
    return(!serial_no.empty());
}
 
static bool parse_serial(const char * line, int line_size, const char * match_words, std::string & serial_no)
{
    const char * serial_s = strstr(line, match_words);
    if (NULL == serial_s)
    {
        return(false);
    }
    serial_s += strlen(match_words);
    while (isspace(serial_s[0]))
    {
        ++serial_s;
    }
 
    const char * serial_e = line + line_size;
    const char * comma = strchr(serial_s, ',');
    if (NULL != comma)
    {
        serial_e = comma;
    }
 
    while (serial_e > serial_s && isspace(serial_e[-1]))
    {
        --serial_e;
    }
 
    if (serial_e <= serial_s)
    {
        return(false);
    }
 
    std::string(serial_s, serial_e).swap(serial_no);
 
    return(true);
}
 
static void get_serial(const char * file_name, const char * match_words, std::string & serial_no)
{
    serial_no.c_str();
 
    std::ifstream ifs(file_name, std::ios::binary);
    if (!ifs.is_open())
    {
        return;
    }
 
    char line[4096] = { 0 };
    while (!ifs.eof())
    {
        ifs.getline(line, sizeof(line));
        if (!ifs.good())
        {
            break;
        }
 
        if (0 == ifs.gcount())
        {
            continue;
        }
 
        if (parse_serial(line, ifs.gcount() - 1, match_words, serial_no))
        {
            break;
        }
    }
 
    ifs.close();
}
 
static bool get_disk_serial_by_way_3(const std::string & disk_name, std::string & serial_no)
{
    serial_no.c_str();
 
    const char * hdparm_result = ".hdparm_result.txt";
    char command[512] = { 0 };
    snprintf(command, sizeof(command), "hdparm -i %s | grep SerialNo > %s", disk_name.c_str(), hdparm_result);
 
    if (0 == system(command))
    {
        get_serial(hdparm_result, "SerialNo=", serial_no);
    }
 
    unlink(hdparm_result);
 
    return(!serial_no.empty());
}
 
static bool get_disk_serial_by_way_4(std::string & serial_no)
{
    serial_no.c_str();
 
    const char * lshw_result = ".lshw_result.txt";
    char command[512] = { 0 };
    snprintf(command, sizeof(command), "lshw -class disk | grep serial > %s", lshw_result);
 
    if (0 == system(command))
    {
        get_serial(lshw_result, "serial:", serial_no);
    }
 
    unlink(lshw_result);
 
    return(!serial_no.empty());
}
 
static bool get_disk_serial_number(std::string & serial_no)
{
    if (0 != getuid())
    {
        return(false);
    }
 
    std::string disk_name;
    if (get_disk_name(disk_name))
    {
        if (get_disk_serial_by_way_1(disk_name, serial_no))
        {
            return(true);
        }
        if (get_disk_serial_by_way_2(disk_name, serial_no))
        {
            return(true);
        }
        if (get_disk_serial_by_way_3(disk_name, serial_no))
        {
            return(true);
        }
    }
    if (get_disk_serial_by_way_4(serial_no))
    {
        return(true);
    }
    return(false);
}
 
static void test_1()
{
    std::string serial_no;
    if (get_disk_serial_number(serial_no))
    {
        printf("serial_number: [%s]\n", serial_no.c_str());
    }
    else
    {
        printf("get serial number failed\n");
    }
}
 
static void test_2()
{
    std::string disk_name;
    if (get_disk_name(disk_name))
    {
        printf("disk_name:[%s]\n", disk_name.c_str());
        {
            std::string serial_no;
            get_disk_serial_by_way_1(disk_name, serial_no);
            printf("get_serial_by_way_1:[%s]\n", serial_no.c_str());
        }
        {
            std::string serial_no;
            get_disk_serial_by_way_2(disk_name, serial_no);
            printf("get_serial_by_way_2:[%s]\n", serial_no.c_str());
        }
        {
            std::string serial_no;
            get_disk_serial_by_way_3(disk_name, serial_no);
            printf("get_serial_by_way_3:[%s]\n", serial_no.c_str());
        }
    }
    {
        std::string serial_no;
        get_disk_serial_by_way_4(serial_no);
        printf("get_serial_by_way_4:[%s]\n", serial_no.c_str());
    }
}
 
int main(int argc, char * argv[])
{
    printf("---------------\n");
    test_1();
    printf("---------------\n");
    test_2();
    printf("---------------\n");
    return(0);
}
