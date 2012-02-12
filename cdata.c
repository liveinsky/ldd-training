#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/vmalloc.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/miscdevice.h>
#include <linux/input.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#define	MSG(m...)	printk(KERN_INFO "CDATA: " m "\n")
#define	MSG2(a,b)	printk(KERN_INFO "CDATA: " a "\n", b)
#define	DEV_MAJOR	121
#define	DEV_NAME	"debug"

static int cdata_open(struct inode *inode, struct file *filp)
{
	MSG(DEV_NAME " is open");
/*
	int i=0;
	for(i=0;i<500000;i++)
	{
		current->state = TASK_UNINTERRUPTIBLE;
		schedule();
	}
*/	
	MSG2("minor number = %d", MINOR(inode->i_rdev));
	return 0;
}

static int cdata_close(struct file *filp, const char *buf, size_t size, loff_t *off)
{
	MSG(DEV_NAME " is close");
	return 0;
}

static ssize_t cdata_read(struct file *filp, char *buf, size_t size, loff_t *off)
{
	MSG(DEV_NAME " is reading");
	return 0;
}

static ssize_t cdata_write(struct file *filp, const char *buf, size_t size, 
			loff_t *off)
{
	unsigned int i=0;
	MSG("CDATA is writting");
#if 1
	for(i=0;i<500000;i++)
	{
		MSG2("CDATA is writting %dnd", i);
		current->state = TASK_UNINTERRUPTIBLE;
		schedule();
	}
#endif
	return 0;
}

static int cdata_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	return 0;
}

static struct file_operations cdata_fops = {
	owner:		THIS_MODULE,
	open:		cdata_open,
	release:	cdata_close,
	read:		cdata_read,
	write:		cdata_write,
	ioctl:		cdata_ioctl,
};

static int cdata_init_module(void)
{
	MSG("CDATA v0.1.0");
	MSG("	Copyright (C) 2004 www.jollen.org");

	if(register_chrdev(DEV_MAJOR, DEV_NAME, &cdata_fops) < 0)
	{
		MSG("Couldn't register a device.");
		return -1;
	}
	
	return 0;
}

static void cdata_cleanup_module(void)
{
	MSG("unregister device.");
	unregister_chrdev(DEV_MAJOR, DEV_NAME);
}

module_init(cdata_init_module);
module_exit(cdata_cleanup_module);

MODULE_LICENSE("GPL");
