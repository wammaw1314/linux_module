#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <asm/io.h>
#include <linux/device.h>
  
#define CDEMO_MAJOR 255					/*预设cdemo的主设备号*/

static int cdemo_major = CDEMO_MAJOR;

/*
设备结构体,此结构体可以封装设备相关的一些信息等信号
量等也可以封装在此结构中,后续的设备模块一般都应该封
装一个这样的结构体,但此结构体中必须包含某些成员;
对于字符设备来说,必须包含struct cdev cdev
struct cdemo_dev
{  
	struct cdev cdev;
	...
};
*/
static struct cdev cdev_demo;

/*
文件打开函数, 上层对此设备调用open时会执行
*/
static int cdemo_open(struct inode *inode, struct file *filp)
{
	printk(KERN_NOTICE "======== cdevdemo_open ");
	return 0;
}

/*
文件释放, 上层对此设备调用close时会执行
*/
static int cdemo_release(struct inode *inode, struct file *filp)
{
	printk(KERN_NOTICE "======== cdevdemo_release ");
	return 0;
}

/*
文件的读操作, 上层对此设备调用read时会执行
*/
static ssize_t cdemo_read(struct file *filp, char __user *buf, size_t count, loff_t *ppos)  
{
	printk(KERN_NOTICE "======== cdevdemo_read ");
	copy_to_user(buf,"cdevdemo_read",13);
    return (ssize_t)13;
}

/*
文件操作结构体, 文中已经讲过这个结构
*/
static const struct file_operations cdemo_fops =
{
	.owner = THIS_MODULE,
	.open = cdemo_open,
	.release = cdemo_release,
	.read = cdemo_read,
};

static int __init cdemo_init(void)
{
	int ret;
	dev_t devno = MKDEV(cdemo_major, 0);
	struct class *cdemo_class;
	printk(KERN_NOTICE "Init cdevdemo");
	/* 申请设备号, 如果申请失败采用动态申请方式 */
	if(cdemo_major)
	{
		printk(KERN_NOTICE "Register cdevdemo");
		ret = register_chrdev_region(devno, 1, "cdemo");
	}
	else
	{
		printk(KERN_NOTICE "Malloc cdevdemo");
		ret = alloc_chrdev_region(&devno, 0, 1, "cdemo");  
		cdemo_major = MAJOR(devno);
	}
	if(ret < 0)
	{
		printk(KERN_NOTICE "Errno cdevdemo");
		return ret;
    }
    cdev_init(&cdev_demo, &cdemo_fops);
    cdev_demo.owner = THIS_MODULE;
    ret = cdev_add(&cdev_demo, devno, 1);
    if (ret)
	{
		printk(KERN_NOTICE "Error add cdevdemo %d", ret);
		unregister_chrdev_region(devno, 1);
		return -1;
	}
/*
下面两行是创建了一个总线类型, 会在/sys/class下生成cdemo目录
执行device_create后会在/dev/下自动生成cdemo设备节点,
如果不调用此函数, 需要手动mknod来创建设备节点后再访问
*/
	cdemo_class = class_create(THIS_MODULE, "cdemo");
	device_create(cdemo_class, NULL, devno, NULL, "cdemo");
	return 0;
}

static void __exit cdemo_exit(void)
{
	printk(KERN_NOTICE "End cdevdemo");
	cdev_del(&cdev_demo);								// 注销cdev
	unregister_chrdev_region(MKDEV(cdemo_major, 0), 1);	// 释放设备号
}

module_init(cdemo_init);
module_exit(cdemo_exit);

MODULE_AUTHOR("YeJoel");
MODULE_LICENSE("GPL");
