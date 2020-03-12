#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<fcntl.h>
#include<linux/fb.h>



int main(int argc, char *argv[])
{
    int fd;
    struct fb_var_screeninfo screen_info;
    
    fd = open("/dev/fb0",O_RDWR);
    if (fd < 0) {
        printf("open /dev/fb0 error\n");
        return -1;
    }

    ioctl(fd, FBIOGET_VSCREENINFO, &screen_info);
    
    /* xres横像素  yres竖像素*/
    printf("%d*%d\n", screen_info.xres, screen_info.yres);
    
    /* bits_per_pixel代表颜色深度 */
    printf("%d\n", screen_info.bits_per_pixel);
    close(fd);
    return 0;
}
