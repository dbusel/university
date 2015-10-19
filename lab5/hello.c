#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/delay.h>


struct timer_list timer;

//The kobject is similar to the Object class in object-oriented languages such as C# or Java. It provides basic facilities, 
//such as reference counting, a name, and a parent pointer, allowing the creation of a hierarchy of objects
static struct kobject *sys_kobj;

static long delay;

static void timer_function(unsigned long data)
{
    //kernel's formatted print function
    //KERN_INFO - An informational message
    printk(KERN_INFO "Hello, World!\n");
    //Updates the expiration time of a timer
    //delay*HZ - converts from seconds to a unit of jiffies
    mod_timer(&timer, jiffies + delay * HZ);
    if (delay == 0) {
      del_timer(&timer);
    }
}
/*
 * The "delay" file where a static variable is read from and written to.
 */
static ssize_t sys_show(struct kobject *kobj, struct kobj_attribute *attr,
      char *buf)
{
  //Write formatted data to string
  return sprintf(buf, "%ld\n", delay);
}

//
static ssize_t sys_store(struct kobject *kobj, struct kobj_attribute *attr,
       const char *buf, size_t count)
{
  //Conversion to signed integer
  kstrtol(buf, 10, &delay);
  if (delay == 0) {
      del_timer(&timer);
    }
  //timer initialization
  init_timer(&timer);
  //The expires field represents the jiffies value when the timer is expected to run
  timer.expires = jiffies;
  //zero is passed to the timer handler
  timer.data = 0;  
  //function to run when timer expires                  
  timer.function = timer_function;
  //enabling timer
  add_timer(&timer);
  return count;
}

//Sysfs attributes
//__ATTR(_name, _mode, _show, _store)
//664 - all can read, owner or group can edit
static struct kobj_attribute sys_attribute =
  __ATTR(delay, 0664, sys_show, sys_store);

/*
 * Create a group of attributes so that we can create and destroy them all
 * at once.
 */
static struct attribute *attrs[] = {
  &sys_attribute.attr,
  NULL, /* need to NULL terminate the list of attributes */
};

/*
 * An unnamed attribute group will put all of the attributes directly in
 * the kobject directory.  If we specify a name, a subdirectory will be
 * created for the attributes with the directory being the name of the
 * attribute group.
 */
static struct attribute_group attr_group = {
  .attrs = attrs,
};

//loading module
static int __init sys_init(void)
{
  int retval;
  
  /*
   * Create a simple kobject with the name of "kobject_example",
   * located under /sys/kernel/
   *
   * As this is a simple directory, no uevent will be sent to
   * userspace.  That is why this function should not be used for
   * any type of dynamic kobjects, where the name and number are
   * not known ahead of time.
   */
  
  
  //create a struct kobject dynamically and register it with sysfs(file system)
  sys_kobj = kobject_create_and_add("hello", kernel_kobj);
  if (!sys_kobj)
    return -ENOMEM;

  /* Create the files associated with this kobject */
  retval = sysfs_create_group(sys_kobj, &attr_group);
  if (retval)
    kobject_put(sys_kobj);

  return retval;
}

//unloading module
static void __exit sys_exit(void)
{
    //disabling timer
    del_timer(&timer);
    //is made when that initial reference is no longer needed
    kobject_put(sys_kobj);
}

module_init(sys_init);
module_exit(sys_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Busel DV");