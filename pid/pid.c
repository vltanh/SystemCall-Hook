#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/tty.h>
#include <linux/string.h>
#include <linux/slab.h>
#include "pid.h"

asmlinkage int pidtoname(int pid, char* buf, int len) {
    struct task_struct *task;
    int ret;
    printk("Query PID: %d\n", pid);
    ret = -1;
    for_each_process(task){
        if((int)task_pid_nr(task) == pid){
            int true_len = strlen(task->comm);
            if (true_len + 1 > len) {
            	ret = true_len;
            }
            else {
            	copy_to_user(buf, task->comm, true_len + 1);
            	printk("Process name: %s\n", task->comm);
            	ret = 0;
            }
        }
    }
    return ret;
}
