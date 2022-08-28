#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/pid.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Batuhan Yalcin");

void DFS_search(struct task_struct *task);
void BFS_search(struct task_struct *task);



//Initalize given_pid
static int given_pid = 0;
//Inıtılize search type
static char* search_type;
//Takes parameters
module_param(given_pid,int,0);
module_param(search_type,charp,0);



static int custom_module_init(void) 
{
    //Print sitation 
    printk(KERN_INFO "Module is loading\n");
    printk(KERN_INFO "Root PID: %d\n",given_pid);
    if(strcmp(search_type,"-d")==0){
            printk(KERN_INFO "Search_type: is DFS\n");
   	}if(strcmp(search_type,"-b")==0){
            printk(KERN_INFO "Search_type: is BFS\n");
    }

    struct pid *pid_struct;
    struct task_struct *task;
	
 		//Task find
		pid_struct = find_get_pid(given_pid);
		task = pid_task(pid_struct, PIDTYPE_PID);
    if(task == NULL) {
			printk(KERN_ERR "There is no task with given pid\n");
			return 0;
		}

    if(strcmp(search_type,"-d")==0){
        DFS_search(task);
    }else if(strcmp(search_type,"-b")==0){
        BFS_search(task);
    }  
    return 0;
}

static void custom_module_exit(void) {
	printk(KERN_INFO "Removing Module...\n");
}

void DFS_search(struct task_struct *task)
{   
    struct task_struct *child;
    struct list_head *list;

    printk("Name: %s, PID: %d\n", task->comm, task->pid);
    
    //Go over the list recursively
    list_for_each(list, &task->children) {
            child = list_entry(list, struct task_struct, sibling);
            DFS_search(child);
        }
        
    
}



void BFS_search(struct task_struct *task)
{   
    struct task_struct *child;
    struct list_head *list;

    printk("Name: %s, PID: %d\n", task->comm, task->pid);
    list_for_each(list, &task->children) {
        //Go over the child recursively
        BFS_search(child);
        child = list_entry(list, struct task_struct, sibling);
    }
}


module_init(custom_module_init);
module_exit(custom_module_exit);
