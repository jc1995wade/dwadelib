#include <linux/module.h>  
#include <linux/kernel.h>  
#include <linux/fs.h>  
#include <linux/miscdevice.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <asm-generic/uaccess.h>

#define CMD_COMMAND 0x1336
#define GPIO_90 90


long hello_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	switch(cmd) {
	case CMD_COMMAND:
		printk("Hello Module hello_ioctl() exced");
		break;
	default:
		printk("Hello Module unknown ioctl cmd");
	}
	
	return 0;
}

ssize_t gpio_read(struct file *filp, char __user *user_buff, size_t size, loff_t *loff)
{
	printk("gpio_read\n");
	return size;
}

ssize_t gpio_write(struct file *filp, const char __user * user_buff, size_t size, loff_t *loff)
{
	int err;
	int data;
	char buff[10] = {0};
	copy_from_user(buff, user_buff, size);

	err = kstrtoint(buff, 10, &data);
	printk("gpio_write data=%d\n", data);
	
	gpio_direction_output(GPIO_90, 0);
	gpio_direction_output(GPIO_90, 1);
	mdelay(data);
	gpio_direction_output(GPIO_90, 0);
	return size;
}

int gpio_open(struct inode * node, struct file * filp)
{
	printk("gpio_open\n");
	return 0;
}

int gpio_release(struct inode * node, struct file * filp)
{
	printk("gpio_release\n");
	return 0;
}


struct file_operations hello_fops = {  //设备的操作函数指针表
	.open = gpio_open,
	.read = gpio_read,
	.write = gpio_write,
	.unlocked_ioctl = hello_ioctl,
	.release = gpio_release
};
 
static struct miscdevice hello_device = {  //注册为misc设备的基本属性
	.minor = MISC_DYNAMIC_MINOR,
	.name = "hello",
	.fops = &hello_fops,
	.mode = 777
};


static int __init hello_begin(void)
{
	int ret = 0;
	ret = misc_register(&hello_device);  //注册为misc设备
	if(ret)
		printk("Failed to register misc device");
	else
		printk("Hello Module successfully loaded");

	gpio_request(GPIO_90, "key_test");

	return ret;
}

static void __exit hello_exit(void)
{
	gpio_free(GPIO_90);
	misc_deregister(&hello_device);  //设备卸载
	printk("Hello module exit");
}

module_init(hello_begin);  //模块初始化函数
module_exit(hello_exit);  //模块卸载函数

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dwade");
MODULE_DESCRIPTION("Hello Kernel Device");
MODULE_VERSION("1.0");
