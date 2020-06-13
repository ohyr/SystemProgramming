#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/time.h>

#include <asm/mach/map.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#define ULTRASONIC_MAJOR_NUMBER	504
#define ULTRASONIC_DEV_NAME		"ultrasonic"

#define GPIO_BASE_ADDR 0x3F200000
#define GPFSEL1 0x04
#define GPSET0 0x1C
#define GPCLR0 0x28
#define GPLEV0 0x34

static void __iomem *gpio_base;
volatile unsigned int *gpsel1;
volatile unsigned int *gpset1;
volatile unsigned int *gpclr1;
volatile unsigned int *gplev1;

//struct timeval start, end;
struct timespec start, end;

unsigned long time;
unsigned long start_time;
unsigned long end_time;
unsigned long distance;

static char buffer[1024];

int ultrasonic_open(struct inode *inode, struct file *filp){
	
	printk(KERN_ALERT "Ultrasonic driver open!!\n");
	
	gpio_base = ioremap(GPIO_BASE_ADDR, 0x60);
	gpsel1 = (volatile unsigned int *)(gpio_base + GPFSEL1);
	gpset1 = (volatile unsigned int *)(gpio_base + GPSET0);
	gpclr1 = (volatile unsigned int *)(gpio_base + GPCLR0);
	gplev1 = (volatile unsigned int *)(gpio_base + GPLEV0);
	
	*gpsel1 |= (1<<24); // GPIO 18 OUT (Trig)
						// GPIO 17 IN  (Echo)
	
	return 0;
}

int ultrasonic_release(struct inode *inode, struct file *filp){
	printk(KERN_ALERT "Ultrasonic driver closed!!\n");
	iounmap((void *)gpio_base);
	return 0;
}

ssize_t ultrasonic_read(struct file *filp, char *buf, size_t size, loff_t *f_pos){
	printk(KERN_ALERT "ultrasonic_read function called\n");
	
	*gpclr1 |= (1 << 18); // Trig low
	ssleep(0.5);
	
	*gpset1 |= (1 << 18); // Trig high during 10us
	ssleep(0.00001);
	*gpclr1 |= (1 << 18);
	
	while((*gplev1 & (1 << 17)) == 0){// Echo low (pulse start)
		//do_gettimeofday(&start);
		getnstimeofday(&start);
	}
	
	while((*gplev1 & (1 << 17)) > 0){// Echo high (pulse end)
		//do_gettimeofday(&end);
		getnstimeofday(&end);
	}
	//start_time = (start.tv_sec*1000000 + start.tv_usec);
	//end_time = (end.tv_sec*1000000 + end.tv_usec);
	
	// ultrasonic_speed = 340m/sec = 34000cm/sec
	// 2*distance = speed * time
	// distance = time * 17000cm/sec

	start_time = ((start.tv_sec % 60)*1000000000 + start.tv_nsec);
	end_time = ((end.tv_sec % 60)*1000000000 + end.tv_nsec);
	
	time = end_time - start_time; //nsec (10^-9)
	distance = time * 17; // 17000cm/sec -> 17cm/msec (10^-3)
	sprintf(buffer, "%lu", distance); // (10^-6)cm
	copy_to_user(buf, buffer, 1024);
	return size;
}

static struct file_operations ultrasonic_fops = {
	.owner = THIS_MODULE,
	.read = ultrasonic_read,
	.open = ultrasonic_open,
	.release = ultrasonic_release
};

int __init ultrasonic_init(void){
	if(register_chrdev(ULTRASONIC_MAJOR_NUMBER, ULTRASONIC_DEV_NAME, &ultrasonic_fops) < 0)
		printk(KERN_ALERT "Ultrasonic initialization fail\n");
	else
		printk(KERN_ALERT "Ultrasonic driver initialization success\n");

	return 0;
}

void __exit ultrasonic_exit(void){
	unregister_chrdev(ULTRASONIC_MAJOR_NUMBER, ULTRASONIC_DEV_NAME);
	printk(KERN_ALERT "Ultrasonic driver exit done\n");
}

module_init(ultrasonic_init);
module_exit(ultrasonic_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yongrok Oh");
MODULE_DESCRIPTION("des");
