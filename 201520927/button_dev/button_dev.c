#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/delay.h>

#include <asm/mach/map.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#define BUTTON_MAJOR_NUMBER	503
#define BUTTON_DEV_NAME		"button_dev"

#define GPIO_BASE_ADDR 0x3F200000
#define GPFSEL1 0x04
#define GPFSEL2 0x08
#define GPSET0 0x1C
#define GPCLR0 0x28
#define GPLEV0 0x34

static void __iomem *gpio_base;
volatile unsigned int *gpsel1;
volatile unsigned int *gpsel2;
volatile unsigned int *gpset1;
volatile unsigned int *gpclr1;
volatile unsigned int *gplev1;

int button_open(struct inode *inode, struct file *filp){
	
	printk(KERN_ALERT "BUTTON driver open!!\n");
	
	gpio_base = ioremap(GPIO_BASE_ADDR, 0x60);
	gpsel1 = (volatile unsigned int *)(gpio_base + GPFSEL1);
	gpsel2 = (volatile unsigned int *)(gpio_base + GPFSEL2);
	gpset1 = (volatile unsigned int *)(gpio_base + GPSET0);
	gpclr1 = (volatile unsigned int *)(gpio_base + GPCLR0);
	gplev1 = (volatile unsigned int *)(gpio_base + GPLEV0);
	
	*gpsel2 |= (1<<3);	// GPIO21 -> OUT
	*gpset1 |= (1<<21);	// GPIO21 -> 1
	
	return 0;
}

int button_release(struct inode *inode, struct file *filp){
	printk(KERN_ALERT "BUTTON driver closed!!\n");
	iounmap((void *)gpio_base);
	return 0;
}

ssize_t button_read(struct file *filp, char *buf, size_t size, loff_t *f_pos){
	*gpset1 |= (1<<21);	// GPIO21 -> 1
	
	//int val = 0;
	int val2 = 0;
	//val = *gplev1 & (1<<20); // GPIO20 
	val2 = *gplev1 & (1<<21); // GPIO21
	//printk("%d, %d\n",val,val2);
	//copy_to_user(buf, &val, sizeof(int)); // GPIO20
	copy_to_user(buf, &val2, sizeof(int)); // GPIO21
	
	return size;
}

static struct file_operations button_fops = {
	.owner = THIS_MODULE,
	.open = button_open,
	.read = button_read,
	.release = button_release
};

int __init button_init(void){
	if(register_chrdev(BUTTON_MAJOR_NUMBER, BUTTON_DEV_NAME, &button_fops) < 0)
		printk(KERN_ALERT "BUTTON driver initialization fail\n");
	else
		printk(KERN_ALERT "BUTTON driver initialization success\n");

	return 0;
}

void __exit button_exit(void){
	unregister_chrdev(BUTTON_MAJOR_NUMBER, BUTTON_DEV_NAME);
	printk(KERN_ALERT "BUTTON driver exit done\n");
}

module_init(button_init);
module_exit(button_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yongrok Oh");
MODULE_DESCRIPTION("des");
