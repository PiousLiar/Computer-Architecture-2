#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("IO-12 Mykyta");
MODULE_DESCRIPTION("Lab3");
MODULE_LICENSE("GPL");

static uint count = 1;
module_param(count, uint, 0664); 
MODULE_PARM_DESC(count, "Count of entries to process"); 

struct custom_data {
  struct list_head node;
  ktime_t time_stamp;
};

static LIST_HEAD(custom_list_head);

static int __init custom_init_function(void) 
{
  int counter;
  
  if (count == 0) {
    pr_warn("count is zero\n"); 
  } else if (count > 10) {
    pr_warn("count more than 10, setting to max limit\n");
    count = 10;
  }

  for (counter = 0; counter < count; counter++) { 
    struct custom_data *item = kmalloc(sizeof(struct custom_data), GFP_KERNEL);
    if (!item) {
      pr_err("Memory allocation failed\n");
      return -ENOMEM;
    }
    item->time_stamp = ktime_get();
    list_add_tail(&item->node, &custom_list_head);
    pr_info("Hello, Mykyta!%d\n", counter);
  }

  return 0;
}

static void __exit custom_exit_function(void) 
{
  struct custom_data *entry, *temp;
  int index = 0; 

  list_for_each_entry_safe(entry, temp, &custom_list_head, node) {
    pr_info("Entry %d; Timestamp = %lld\n", index++, ktime_to_ns(entry->time_stamp));
    list_del(&entry->node);
    kfree(entry);
  }
  pr_info("Module removed successfully\n");
}

module_init(custom_init_function);
module_exit(custom_exit_function);

