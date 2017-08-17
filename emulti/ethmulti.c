#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

#define GMAC_MULTI_ON	_IO('l', 0x01)

MODULE_AUTHOR("YeJoel");
MODULE_LICENSE("GPL");

static int GMac_Multi_En(int en)
{
        u32 addr = (u32) 0xb34b0004;
        u32 data = readl((void *)addr);
	if (en)
	{
		data |= 0x00000010;
	}
	else
	{
		data &= 0xffffffef;
	}
        writel(data,(void *)addr);
//        printk(KERN_ALERT "data1:%08x\n",data);
//        data = readl((void *)addr);
//        printk(KERN_ALERT "data2:%08x\n",data);
	return 0;
}

static long emulti_ioctl(struct file *file,
                unsigned int cmd,
                unsigned long arg)
{
	switch (cmd)
	{
		case GMAC_MULTI_ON:
			GMac_Multi_En(1);
			break;
		default:
                	return -EINVAL; 
	}
	return 0;
}

static int emulti_open(struct inode *inode, struct file *filp)
{
        return 0;
}

static int emulti_release(struct inode *inode, struct file *file)
{
        return 0;
}

static struct file_operations emulti_fops =
{
    .owner      = THIS_MODULE,
    .unlocked_ioctl = emulti_ioctl,
    .open = emulti_open,
    .release = emulti_release,
};

static struct miscdevice emulti_miscdev =
{
    .minor      = MISC_DYNAMIC_MINOR,
    .name       = "emulti",
    .fops       = &emulti_fops
};


static int __init emulti_init(void)
{
	int ret = 0;
	GMac_Multi_En(1);
	ret = misc_register(&emulti_miscdev);
	if (ret)
	{
		printk (KERN_ERR "cannot register emulti (err=%d)\n", ret);
		return -1;
	}
	return 0;
}

static void __exit emulti_exit(void)
{
	misc_deregister(&emulti_miscdev);
}

module_init(emulti_init);
module_exit(emulti_exit);

