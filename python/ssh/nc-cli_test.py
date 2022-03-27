import re,os,time,sys,chardet
import datetime
import paramiko          #引入ssh模块，该模块需要单独安装。


def GetNowTime():#获取当前系统时间
    return time.strftime("%Y-%m-%d %H:%M:%S",time.localtime(time.time()))

def connect_test(hostname, port, username, password):
    try:
        client = paramiko.SSHClient()
        client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        client.connect(hostname, port, username, password, timeout=5)
        #ssh_shell = client.invoke_shell()
        #print(ssh_shell.recv(4096))
    except:
        ERR = '登录失败: ' + hostname
        print(ERR)
        client.close()
        return
    else:
        print('登录成功')

        cmd = 'ls'
        stdin, stdout, stderr = client.exec_command(cmd)
        print(stdout.read().decode('utf-8'))
        print(stderr.read().decode('utf-8'))

        client.close()
        return

def shell_cmd(ssh_shell, cmd):
    ssh_shell.sendall(cmd.strip() + '\n')
    time.sleep(0.1)

    print(ssh_shell.recv(4096).decode('utf-8'))
    time.sleep(0.1)
 
def shell_cmd_commit(ssh_shell, cmd):
    ssh_shell.sendall(cmd.strip() + '\n')
    time.sleep(3)

    print(ssh_shell.recv(4096).decode('utf-8'))
    time.sleep(0.1)

def dhcp(ssh_shell):
    for i in range(1, 5):
        shell_cmd(ssh_shell,  'interface vlan Ge0/2.%d' % i)
        shell_cmd(ssh_shell,  'vlan-id %d' % i)
        shell_cmd(ssh_shell,  'ipv4 address 20.0.%d.1/24' % i)
        shell_cmd(ssh_shell,  'link-interface Ge0/2')
        shell_cmd(ssh_shell,  'enabled true')
        time.sleep(0.1)
        shell_cmd(ssh_shell,  '..')
        shell_cmd(ssh_shell,  '..')

def dhcp_full(ssh_shell):
    for i in range(2, 254):
        shell_cmd(ssh_shell,  'dhcp server subnet 20.0.%d.0/24' % i)
        shell_cmd(ssh_shell,  'default-gateway 20.0.%d.1' % i)
        shell_cmd(ssh_shell,  'dhcp-options dhcp-server-identifier 20.0.%d.1' % i)
        shell_cmd(ssh_shell,  'dhcp-options domain-name-server 114.114.114.114')
        shell_cmd(ssh_shell,  'dhcp-options ntp-server 20.0.%d.1' % i)
        shell_cmd(ssh_shell,  'range 20.0.%d.1 20.0.%d.254' % (i, i))
        shell_cmd(ssh_shell,  'dhcp-options ntp-server 20.0.%d.1' % i)
        shell_cmd_commit(ssh_shell, 'commit')
        shell_cmd(ssh_shell,  '..')
        shell_cmd(ssh_shell,  '..')
        shell_cmd(ssh_shell,  '..')


def dhcp_full_del(ssh_shell):
    for i in range(1, 254):
        shell_cmd(ssh_shell,  'del dhcp server subnet 20.0.%d.0/24' % i)
        
    shell_cmd(ssh_shell,  'commit')
    time.sleep(10)


def login_ssh(hostname, port, username, password):
    try:
        client = paramiko.SSHClient()
        client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        client.connect(hostname, port, username, password, timeout=5)
        ssh_shell = client.invoke_shell()
        print(ssh_shell.recv(4096))
    except:
        ERR = '登录失败: ' + hostname
        print(ERR)
        client.close()
        return
    else:
        print('登录成功')

        cmd = 'nc-cli'
        ssh_shell.sendall(cmd.strip() + '\n')
        time.sleep(1)
        
        cmd = 'edit running'
        ssh_shell.sendall(cmd.strip() + '\n')
        time.sleep(0.1)
        
        cmd = 'vrf main'
        ssh_shell.sendall(cmd.strip() + '\n')
        time.sleep(0.1)
        
        print(ssh_shell.recv(4096).decode('utf-8'))
        time.sleep(0.1)
        
        t1 = time.time()

        for i in range(2, 10):
            dhcp_full(ssh_shell)
            dhcp_full_del(ssh_shell)
            print('第 %d 次' % i)

        #shell_cmd(ssh_shell,  'commit')
        time.sleep(5)

        t2 = time.time()
        time.sleep(1)
        print('time: %d', t2-t1)

        client.close()
        return    



print(GetNowTime())
host = '192.168.3.17'
port = 22

login_ssh(host, port, 'root', 'ruijie@123')
