#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/random.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/kdev_t.h>

#define DEVICE_NAME "Temperature_Sensor"
#define CLASS_NAME "temp"

static int major_number;
static int device_open = 0;
static char temperature_message[256];
static struct class* tempsensorClass = NULL;
static struct device* tempsensorDevice = NULL;

static int generate_random_temperature(int min, int max) {
    unsigned int random;
    int range = max - min + 1;
    get_random_bytes(&random, sizeof(random));
    return min + (random % range);
}

static int dev_open(struct inode *inodep, struct file *filep) {
    if (device_open)
        return -EBUSY;
    device_open++;
    return 0;
}

static int dev_release(struct inode *inodep, struct file *filep) {
    device_open--;
    return 0;
}

static ssize_t dev_read(struct file *filep, char __user *buffer, size_t len, loff_t *offset) {
    int temperature, error_count = 0, msg_len;
    char condition[32];

    if (*offset > 0)
        return 0;

    temperature = generate_random_temperature(-10, 50);

    if (temperature >= 35)
        snprintf(condition, sizeof(condition), "Extreme Hot");
    else if (temperature >= 25)
        snprintf(condition, sizeof(condition), "Sunny");
    else if (temperature >= 18)
        snprintf(condition, sizeof(condition), "Warm");
    else if (temperature >= 10)
        snprintf(condition, sizeof(condition), "Cloudy");
    else if (temperature >= 5)
        snprintf(condition, sizeof(condition), "Cool");
    else if (temperature >= 0)
        snprintf(condition, sizeof(condition), "Cold");
    else
        snprintf(condition, sizeof(condition), "Freezing");

    snprintf(temperature_message, sizeof(temperature_message), "%d %s\n", temperature, condition);
    msg_len = strlen(temperature_message);

    if (len < msg_len)
        return -EINVAL;

    error_count = copy_to_user(buffer, temperature_message, msg_len);
    if (error_count == 0) {
        ssleep(2);
        return msg_len;
    } else {
        return -EFAULT;
    }
}

static ssize_t dev_write(struct file *filep, const char __user *buffer, size_t len, loff_t *offset) {
    return -EINVAL;
}

static struct file_operations fops = {
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

static int __init tempsensor_init(void) {
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0)
        return major_number;

    tempsensorClass = class_create(CLASS_NAME);
    if (IS_ERR(tempsensorClass)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        return PTR_ERR(tempsensorClass);
    }

    tempsensorDevice = device_create(tempsensorClass, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(tempsensorDevice)) {
        class_destroy(tempsensorClass);
        unregister_chrdev(major_number, DEVICE_NAME);
        return PTR_ERR(tempsensorDevice);
    }

    printk(KERN_INFO "Temperature Sensor: Module loaded with major number %d\n", major_number);
    return 0;
}

static void __exit tempsensor_exit(void) {
    device_destroy(tempsensorClass, MKDEV(major_number, 0));
    class_destroy(tempsensorClass);
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "Temperature Sensor: Module unloaded\n");
}

module_init(tempsensor_init);
module_exit(tempsensor_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tharun_Kumar_M");
MODULE_DESCRIPTION("Temperature Sensor Simulator with Weather Symbols");
MODULE_VERSION("1.0");

