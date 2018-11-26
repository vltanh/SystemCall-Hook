#include <asm/unistd.h>
#include <asm/cacheflush.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <asm/pgtable_types.h>
#include <linux/highmem.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/moduleparam.h>
#include <linux/unistd.h>
#include <linux/slab.h>
#include <asm/cacheflush.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("1612838_1612849");

void **system_call_table_addr;

asmlinkage int (*original_open) (const char*, int, int);
asmlinkage int (*original_write) (struct file*, const char*, int);

asmlinkage int my_open (const char* file, int flags, int mode) {
    char* buf = kmalloc(256, GFP_KERNEL);
    copy_from_user(buf, file, 256);
    printk(KERN_INFO "%s opened %s", current->comm, buf);
    kfree(buf);
    return original_open(file, flags, mode);
}

asmlinkage int my_write (struct file* filp, const char* file, int count) {
    char* buf = kmalloc(256, GFP_KERNEL);
    copy_from_user(buf, file, 256);
    printk(KERN_INFO "%s wrote %d (bytes) in %s", current->comm, count, filp->f_path.dentry->d_iname);
    kfree(buf);
    return original_write(filp, file, count);
}

int make_rw(unsigned long address){
    unsigned int level;
    pte_t *pte = lookup_address(address, &level);
    if(pte->pte &~_PAGE_RW){
        pte->pte |=_PAGE_RW;
    }
    return 0;
}

int make_ro(unsigned long address){
    unsigned int level;
    pte_t *pte = lookup_address(address, &level);
    pte->pte = pte->pte &~_PAGE_RW;
    return 0;
}

static int __init entry_point(void){
    printk(KERN_INFO "Captain Hook loaded successfully..\n");

    system_call_table_addr = (void*)0xffffffff81801440;

    make_rw((unsigned long)system_call_table_addr);

    original_open = system_call_table_addr[__NR_open];
    system_call_table_addr[__NR_open] = my_open;
    
    original_write = system_call_table_addr[__NR_write];
    system_call_table_addr[__NR_write] = my_write;

    return 0;
}

static int __exit exit_point(void){
    printk(KERN_INFO "Unloaded Captain Hook successfully\n");
    system_call_table_addr[__NR_open] = original_open;
    system_call_table_addr[__NR_write] = original_write;
    make_ro((unsigned long)system_call_table_addr);
    return 0;
}

module_init(entry_point);
module_exit(exit_point);
