#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/list.h>


static int __init init_kreportd(void){
	int nr_total;
	int nr_running, nr_interruptible, nr_uninterruptible;
	int nr_stopped, nr_zombie,nr_traced,nr_dead,nr_noninteractive;
	char* pstate;
	struct task_struct *p = &init_task;
	nr_total = 0;
	nr_running = nr_interruptible = nr_uninterruptible = 0;
	nr_stopped = nr_zombie = nr_traced = nr_dead = nr_noninteractive = 0;

	rcu_read_lock();

	printk("name\t\t\tpid\tstate\t\t\t\tpname\n");

	for (p = &init_task; (p = next_task(p)) != &init_task;)
	{

		switch(p->state)
		{
			case TASK_RUNNING:
				nr_running++;
				pstate = "TASK_RUNNING";
				break;
			case TASK_INTERRUPTIBLE:
				nr_interruptible++;
				pstate = "TASK_INTERRUPTIBLE";
				break;
			case TASK_UNINTERRUPTIBLE:
				nr_uninterruptible++;
				pstate = "TASK_UNINTERRUPTIBLE";
				break;
			case TASK_STOPPED:
				nr_stopped++;
				pstate = "TASK_STOPPED";
				break;
			case EXIT_ZOMBIE:
				nr_zombie++;
				pstate = "TASK_ZOMBIE";
				break;
			case EXIT_DEAD:
				nr_dead++;
				pstate = "EXIT_DEAD";
				break;
			case TASK_TRACED:
				nr_traced++;
				pstate = "TASK_TRACED";
				break;
			default:
				pstate = "UNKNOWN";
				break;
		}
		nr_total++;
		printk("%-24s\t%d\t%-24s\t%-s\n",
				 p->comm, p->pid, pstate, p->parent->comm);
	}


	printk("TASK_TOTAL:             %4d\n", nr_total);
	printk("TASK_RUNNING:           %4d\n", nr_running);
	printk("TASK_INTERRUPTIBLE:     %4d\n", nr_interruptible);
	printk("TASK_UNINTERRUPTIBLE:   %4d\n", nr_uninterruptible);
	printk("TASK_STOPPED:           %4d\n", nr_stopped);	
	printk("TASK_ZOMBIE:            %4d\n", nr_zombie);
	printk("TASK_TRACED:            %4d\n", nr_traced);	
	printk("EXIT_DEAD:     		    %4d\n", nr_dead);

	rcu_read_unlock();

	return 0;

}


static void __exit cleanup_kreportd(void)
{
	printk("<1>Goodbye!\n");
}

module_init(init_kreportd);
module_exit(cleanup_kreportd);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Liangze Hu");
MODULE_DESCRIPTION("kthread");
