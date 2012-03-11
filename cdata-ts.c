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

#define	MSG(m...)	printk(KERN_INFO "CDATA-TS: " m "\n")
#define	MSG2(a,b)	printk(KERN_INFO "CDATA-TS: " a "\n", b)
#define	DEV_MAJOR	121
#define	DEV_NAME	"debug"

void cdata_bh(unsigned long);
DECLARE_TASKLET(my_tasklet, cdata_bh, NULL);
struct input_dev ts_input;

int x, y;

int ts_input_open(struct input_dev *dev)
{
	input_report_abs(dev, ABS_X, x);
	input_report_abs(dev, ABS_Y, y);
	return 0;
}
void ts_input_close(struct input_dev *dev)
{
}

int cdata_ts_handler(int irq, void *priv, struct pt_regs *reg)
{
	static int i=0;
	MSG2("in cdata_ts_handler(%d)", i++);
	tasklet_schedule(&my_tasklet);		
	
	/* FIXME: read (x,y) from ADC */
	x = 100;
	y = 100;
	
	return 0;
}

void cdata_bh(unsigned long priv)
{
	static int i=0;
	MSG2("cdata_bh(%d)", i++);
	//while(1);
}

static int cdata_ts_open(struct inode *inode, struct file *filp)
{
	u32 reg;
	u32 cdata;
	
	MSG(DEV_NAME " is open");
	MSG2("minor number = %d", MINOR(inode->i_rdev));
#if 0
	reg = GPGCON;
	reg |= 0xff000000;
	GPGCON = reg;
#else
	set_gpio_ctrl(GPIO_YPON);
	set_gpio_ctrl(GPIO_YMON);
	set_gpio_ctrl(GPIO_XPON);
	set_gpio_ctrl(GPIO_XMON);
#endif

	ADCTSC = DOWN_INT | XP_PULL_UP_EN | XP_AIN | XM_HIZ | YP_AIN | YM_GND | XP_PST(WAIT_INT_MODE);

	/* Request touch panel IRQ */
	if(request_irq(IRQ_TC, cdata_ts_handler, 0, "cdata-ts", (void *)cdata)) 
	{
		MSG("request irq failed.");
		return -1;
	}	

	/* handling input device */
	ts_input.name = "cdata-ts";
	ts_input.open = ts_input_open;
	ts_input.close = ts_input_close;
	// capability
	ts_input.absbit[0] = BIT(ABS_X) | BIT(ABS_Y);
	
	input_register_device(&ts_input);

	return 0;
}

static int cdata_ts_close(struct inode *inode, struct file *filp)
{
	MSG(DEV_NAME " is close");
	
	return 0;
}

static ssize_t cdata_ts_read(struct file *filp, char *buf, size_t size, loff_t *off)
{
	MSG(DEV_NAME " is reading");
	return 0;
}

static ssize_t cdata_ts_write(struct file *filp, const char *buf, size_t size, 
			loff_t *off)
{
	return 0;
}

static int cdata_ts_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	return 0;
}

int cdata_ts_mmap(struct file *filp, struct vm_area_struct *vma)
{
	return 0;	
}

static struct file_operations cdata_ts_fops = {
	owner:		THIS_MODULE,
	open:		cdata_ts_open,
	release:	cdata_ts_close,
	read:		cdata_ts_read,
	write:		cdata_ts_write,
	ioctl:		cdata_ts_ioctl,
	mmap:		cdata_ts_mmap,
};

static struct miscdevice cdata_ts_misc = {
	minor:	CDATA_TS_MINOR,
	name:	"cdata-ts",
	fops:	&cdata_ts_fops,
};

static int cdata_ts_init_module(void)
{
	MSG("CDATA-TS v0.1.0");

	if(misc_register(&cdata_ts_misc) < 0)
	{
		MSG("Couldn't register a device.");
		return -1;
	}
	else
	{
		MSG("CDATA_TS is inited.");
		return 0;
	}
	
}

static void cdata_ts_cleanup_module(void)
{
	MSG("unregister device.");
	misc_deregister(&cdata_ts_misc);
}

module_init(cdata_ts_init_module);
module_exit(cdata_ts_cleanup_module);

MODULE_LICENSE("GPL");
