#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/tty.h>
#include <linux/string.h>
#include <linux/slab.h>
#include "pname.h"

asmlinkage int pnametoid(char* process_name) {
    struct task_struct *task;
    char* buffer;
    int ret;
    buffer = kmalloc(256, GFP_KERNEL);
    copy_from_user(buffer, process_name, 256);
    printk("Query: %s\n", buffer);
    ret = -1;
    for_each_process(task){
        if(strcmp(task->comm,buffer) == 0){
            ret = (int)task_pid_nr(task);
            printk("PID = %d\n", ret);
        }
    }
    kfree(buffer);
    return ret;
}
