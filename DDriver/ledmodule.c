#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/module.h>
#include <asm/io.h>
#include <linux/timer.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/mm.h>

#include <linux/io.h>
//#include <mach/platform.h>

#include "utils.h"
#define DEVICE_NAME "led0"
#define CLASS_NAME "ledClass"

MODULE_LICENSE("GPL");

static struct class* ledDevice_class = NULL;
static dev_t ledDevice_majorminor;
static struct cdev c_dev;  // Character device structure


static struct class *s_pDeviceClass;
struct GpioRegisters *s_pGpioRegisters;

static const int LedGpioPinRed = 17;
static const int LedGpioPinGreen = 27;

ssize_t led_device_write(struct file *pfile, const char __user *pbuff, size_t len, loff_t *off) { 
	struct GpioRegisters *pdev; 
	pr_alert("%s: called (%u)\n",__FUNCTION__,len);

	
	if(unlikely(pfile->private_data == NULL))
		return -EFAULT;

	pdev = (struct GpioRegisters *)pfile->private_data;
	if (pbuff[0]=='0'){
		SetGPIOOutputValue(pdev, LedGpioPinRed, 0);
		SetGPIOOutputValue(pdev, LedGpioPinGreen, 0);
	}
	else if (pbuff[0]=='1'){
		SetGPIOOutputValue(pdev, LedGpioPinRed, 0);
		SetGPIOOutputValue(pdev, LedGpioPinGreen, 1);
	}
	else if (pbuff[0]=='2'){
		SetGPIOOutputValue(pdev, LedGpioPinRed, 1);
		SetGPIOOutputValue(pdev, LedGpioPinGreen, 0);
	}
	else if (pbuff[0]=='3'){
		SetGPIOOutputValue(pdev, LedGpioPinRed, 1);
		SetGPIOOutputValue(pdev, LedGpioPinGreen, 1);
	}else{
		SetGPIOOutputValue(pdev, LedGpioPinGreen, 0);
		SetGPIOOutputValue(pdev, LedGpioPinRed, 0);
	}
	
	
	return len;
}

int led_device_close(struct inode *p_inode, struct file * pfile){
	
	pr_alert("%s: called\n",__FUNCTION__);
	pfile->private_data = NULL;
	return 0;
}


int led_device_open(struct inode* p_indode, struct file *p_file){

	pr_alert("%s: called\n",__FUNCTION__);
	p_file->private_data = (struct GpioRegisters *) s_pGpioRegisters;
	//p_file->private_data = (struct GpioRegisters *) s_pGpioRegisters2;
	return 0;
	
}


static struct file_operations ledDevice_fops = {
	.owner = THIS_MODULE,
	.write = led_device_write,
	.release = led_device_close,
	.open = led_device_open,
};

static int __init ledModule_init(void) {
	int ret;
	struct device *dev_ret;

	pr_alert("%s: called\n",__FUNCTION__);

	if ((ret = alloc_chrdev_region(&ledDevice_majorminor, 0, 1, DEVICE_NAME)) < 0) {
		return ret;
	}

	if (IS_ERR(ledDevice_class = class_create(THIS_MODULE, CLASS_NAME))) {
		unregister_chrdev_region(ledDevice_majorminor, 1);
		return PTR_ERR(ledDevice_class);
	}
	if (IS_ERR(dev_ret = device_create(ledDevice_class, NULL, ledDevice_majorminor, NULL, DEVICE_NAME))) {
		class_destroy(ledDevice_class);
		unregister_chrdev_region(ledDevice_majorminor, 1);
		return PTR_ERR(dev_ret);
	}

	cdev_init(&c_dev, &ledDevice_fops);
	c_dev.owner = THIS_MODULE;
	if ((ret = cdev_add(&c_dev, ledDevice_majorminor, 1)) < 0) {
		printk(KERN_NOTICE "Error %d adding device", ret);
		device_destroy(ledDevice_class, ledDevice_majorminor);
		class_destroy(ledDevice_class);
		unregister_chrdev_region(ledDevice_majorminor, 1);
		return ret;
	}


	s_pGpioRegisters = (struct GpioRegisters *)ioremap(GPIO_BASE, sizeof(struct GpioRegisters));
	
	pr_alert("map to virtual adresse: 0x%x\n", (unsigned)s_pGpioRegisters);
	
	SetGPIOFunction(s_pGpioRegisters, LedGpioPinRed, 0b001); //Output
	SetGPIOFunction(s_pGpioRegisters, LedGpioPinGreen, 0b001); //Output
	
	return 0;
}

static void __exit ledModule_exit(void) {
	
	pr_alert("%s: called\n",__FUNCTION__);
	
	SetGPIOFunction(s_pGpioRegisters, LedGpioPinRed, 0);
	SetGPIOFunction(s_pGpioRegisters, LedGpioPinGreen, 0); 
	iounmap(s_pGpioRegisters);
	cdev_del(&c_dev);
	device_destroy(ledDevice_class, ledDevice_majorminor);
	class_destroy(ledDevice_class);
	unregister_chrdev_region(ledDevice_majorminor, 1);
}

module_init(ledModule_init);
module_exit(ledModule_exit);
