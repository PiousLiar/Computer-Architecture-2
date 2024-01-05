// SPDX-License-Identifier: GPL-2-Clause
#include <hello1.h>

MODULE_AUTHOR("IO-12 Mykyta");
MODULE_DESCRIPTION("Lab4");
MODULE_LICENSE("GPL");

struct timing_data {
    struct list_head list_node;
    ktime_t begin_time;
    ktime_t finish_time;
};

static struct list_head head_node = LIST_HEAD_INIT(head_node);

int print_hello(uint count) 
{
    struct timing_data *item, *temp_item; 
    uint index; 

  
    if (count == 0) {
        pr_err("Invalid 'count': must be greater than 0\n");
        return -EINVAL;
    } else if (count < 5) {
        pr_info("'count' is less than 5\n");
    } else if (count <= 10) {
        pr_warn("'count' is between 5 and 10\n");
    } else { 
        pr_err("'count' exceeds 10\n");
        return -EINVAL;
    }

    for (index = 0; index < count; index++) {
        item = kmalloc(sizeof(struct timing_data), GFP_KERNEL);
        if (!item) { // Checking if kmalloc returned NULL
            goto handle_error;
        }
        item->begin_time = ktime_get();
        pr_info("Hello, Mykyta!%d\n", index);
        item->finish_time = ktime_get();
        list_add_tail(&item->list_node, &head_node);
    }
    return 0;

handle_error: 
    pr_err("Memory allocation failed\n");
    list_for_each_entry_safe(item, temp_item, &head_node, list_node) {
        list_del(&item->list_node);
        kfree(item);
    }
    return -ENOMEM;
}
EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void) 
{
    pr_info("hello1 module initializing\n");
    return 0;
}

static void __exit hello1_exit(void) 
{
    struct timing_data *item, *temp_item; 

    list_for_each_entry_safe(item, temp_item, &head_node, list_node) {
        pr_info("Execution duration: %lld ns\n", ktime_to_ns(item->finish_time - item->begin_time));
        list_del(&item->list_node);
        kfree(item);
    }
    BUG_ON(!list_empty(&head_node));
    pr_info("hello1 module removed\n");
}

module_init(hello1_init);
module_exit(hello1_exit);

