// sudo insmod my_device.ko
// sudo rmmod my_device
// dmesg | tail
// cat /proc/my_device

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/device.h>

MODULE_LICENSE("GPL");

dev_t dev = 0;
static struct class *dev_class;
static struct cdev my_device_cdev;

static int len = 1;
static int w = 0;
static short size_of_message;
static char msg[256];

static int __init my_device_init(void);
static void __exit my_device_exit(void);

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static int open_proc(struct inode*, struct file*);
static int release_proc(struct inode*, struct file*);
static ssize_t read_proc(struct file*, char __user*, size_t, loff_t*);
static ssize_t write_proc(struct file*, const char*, size_t, loff_t*);

static struct file_operations proc_fops = {
	.open = open_proc,
	.read = read_proc,
	.write = write_proc,
	.release = release_proc,
};

static struct file_operations fops = {
	.read=device_read,
	.write=device_write,
	.open=device_open,
	.release=device_release,
};

static int open_proc(struct inode* inode, struct file* file)
{
	printk(KERN_INFO "otwarto plik proc\n");
	return 0;
}

static int release_proc(struct inode *inode, struct file* file)
{
	printk(KERN_INFO "zreleasowano plik proc\n");
	return 0;
}

static ssize_t write_proc(struct file* filp, const char* buff, size_t len, loff_t* off)
{
	return 0;
}

static ssize_t read_proc(struct file* flip, char __user * buff, size_t length, loff_t* off)
{
	printk("my_device: Odczytano plik proc\n");
	if (len){
	len=0;
	} else
	{
	len=1;
	return 0;
	}
	sprintf(msg,"my_device: Wynik zapisany w pliku /proc/my_device to: %d\n",w);
	size_of_message=strlen(msg);
	copy_to_user(buff,msg,size_of_message);
	return length;;
}

static int __init my_device_init(void)
{
	if((alloc_chrdev_region(&dev,0,1, "my_device")) < 0){
	printk(KERN_INFO "my_device: Nie mozna przydzielic numeru major\n");
	return -1;
	}
	printk(KERN_INFO "my_device: Zarejestrowano urzadzenie z numerami: Major = %d, Minor = %d\n",MAJOR(dev),MINOR(dev));

	cdev_init(&my_device_cdev,&fops);

	if((cdev_add(&my_device_cdev,dev,1)) < 0){
	printk(KERN_INFO "my_device: Nie mozna dodac urzadzenia do systemu\n");
	goto r_class;
	}

	if((dev_class = class_create(THIS_MODULE, "my_device")) == NULL){
	printk(KERN_INFO "my_device: Nie mozna stworzyc struct class\n");
	goto r_class;
	}

	if((device_create(dev_class,NULL,dev,NULL,"my_device")) == NULL){
	printk(KERN_INFO "my_device: Nie mozna stworzyc urzadzenia my_device\n");
	goto r_device;
	}
	
	proc_create("my_device",0666,NULL,&proc_fops);
	printk(KERN_INFO "my_device: Udalo sie stworzyc sterownik!\n");
	return 0;

r_device:
	class_destroy(dev_class);
r_class:
	unregister_chrdev_region(dev,1);
	return-1;
}

void __exit my_device_exit(void)
{
	remove_proc_entry("my_device",NULL);
	device_destroy(dev_class,dev);
	class_destroy(dev_class);
	cdev_del(&my_device_cdev);
	unregister_chrdev_region(dev,1);
	printk(KERN_INFO "my_device: Urzadzenie usuniete\n");
}

static int device_open(struct inode *inode, struct file *f)
{
	static int counter = 0;
	counter++;
	printk(KERN_INFO "my_device: Otwarto urzadzenie %d raz \n", counter);
	return 0;
}

static int device_release(struct inode *inode, struct file *f)
{
	printk(KERN_INFO "my_device: Modul zamkniety!");
	return 0;
}

static ssize_t device_read(struct file *filp, char *buffer, size_t len, loff_t *offset)
{
	copy_to_user(buffer, msg, size_of_message);
	printk(KERN_INFO "my_device: Odczytano liczbe %s\n", msg);
	return 0;
}

static ssize_t device_write(struct file *filp, const char *buffer, size_t len, loff_t *offset)
{
	sprintf(msg, "%s", buffer);
	size_of_message = strlen(msg);
	kstrtoint(msg, 10, &w);
	printk(KERN_INFO "my_device: Otrzymalem liczbe %d od prog2\n",w);
	w = (w*w) %1024;
	printk(KERN_INFO "my_device: Wynik obliczen to: %d\n", w);
	return 0;
}

module_init(my_device_init);
module_exit(my_device_exit);
