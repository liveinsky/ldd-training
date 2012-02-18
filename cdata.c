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
#include "cdata_ioctl.h"

#define	MSG(m...)	printk(KERN_INFO "CDATA: " m "\n")
#define	MSG2(a,b)	printk(KERN_INFO "CDATA: " a "\n", b)
#define	DEV_MAJOR	121
#define	DEV_NAME	"debug"

struct cdata_t {
	unsigned long *fb;
};

static int lcd_set(unsigned long *fb, unsigned long color, int pixel)
{
	int i=0;

	if((pixel > 320*240) || (pixel == 0))
		pixel = 320*240;

	for(i = 0; i < pixel; i++)
	{
		writel(color, fb++);
	}

	return 0;
}

static int cdata_open(struct inode *inode, struct file *filp)
{
	struct cdata_t *data;

	MSG(DEV_NAME " is open");
	MSG2("minor number = %d", MINOR(inode->i_rdev));

	data = (struct cdata_t *) kmalloc(sizeof(struct cdata_t), GFP_KERNEL);
	data->fb = ioremap(0x33F00000, 320*240*4);
	filp->private_data = (void *)data;
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
	char *buf_in;
	MSG("CDATA is writting");
#if 0
	for(i=0;i<50000;i++)
	{
		MSG2("CDATA is writting %dnd", i);
		current->state = TASK_UNINTERRUPTIBLE;
		//current->state = TASK_INTERRUPTIBLE;
		schedule();
	}
#endif

	MSG("kmalloc for buf_in[].");
	buf_in = kmalloc(size, GFP_KERNEL);
	memcpy(buf_in, buf, size);

	for(i=0; i<size; i++)
	{
		printk(KERN_INFO "CDATA: the write buf[%d] = 0x%x\n", i, buf_in[i]);
	}	
	
	MSG("free buf_in[].");
	kfree(buf_in);

	return 0;
}

static int cdata_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	int n;
	unsigned long *fb;
	unsigned long color=0;
	
	//fix me: lock for fb
	fb = ((struct cdata_t *)filp->private_data)->fb;
	//fix me: unlock for fb
	
	switch(cmd)
	{
		case CDATA_CLEAR:
			n = *((int *) arg); // fixme
			MSG2("clear pixl = %d.", n);
			color = 0xFFFFFF;
			break;
		case CDATA_BLACK:
			color = 0x0;
			break;
		case CDATA_WHITE:
			color = 0xFFFFFF;
			break;
		case CDATA_RED:
			color = 0xFF0000;
			break;
		case CDATA_GREEN:
			color = 0x00FF00;
			break;
		case CDATA_BLUE:
			color = 0xFF;
			break;
		default:
			break;
	}
			
	lcd_set(fb, color, 0);

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
	unsigned long *fb;
#if 0
	int i=0, count=320*240, count1=count*4;
#endif
	MSG("CDATA v0.1.0");
	MSG("	Copyright (C) 2004 www.jollen.org");
#if 0
	fb = ioremap(0x33F00000, count1);
	for(i = 0; i < count; i++)
	{
		writel(0x00ff00, fb++);
	}
#endif
	fb = ioremap(0x33F00000, 320*240*4);
	lcd_set(fb, 0x00ff00, 0);
	
	if(register_chrdev(DEV_MAJOR, DEV_NAME, &cdata_fops) < 0)
	{
		MSG("Couldn't register a device.");
		return -1;
	}
	else
	{
		MSG("CDATA is inited.");
		return 0;
	}
	
}

static void cdata_cleanup_module(void)
{
	MSG("unregister device.");
	unregister_chrdev(DEV_MAJOR, DEV_NAME);
}

module_init(cdata_init_module);
module_exit(cdata_cleanup_module);

MODULE_LICENSE("GPL");
