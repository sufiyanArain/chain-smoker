
#include<linux/delay.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kthread.h>
#include<linux/sched.h>
#include<linux/time.h>
#include<linux/timer.h>
#include<linux/semaphore.h>



static struct semaphore Smoker[3];								// three semaphores  
static struct semaphore tempSem[3];								// three semaphores used to handle deadlock

static struct task_struct *Stid[3];							// three threads 1st tobbacoo_smoker , 2nd Matches_smoker ,3rd Paper_smoker
static struct task_struct *Atid;							//  Agent thread used  to solve deadlock

int count0=0,count1=0,count2=0;								
int AllDone=0;
int Members[3]={0};																	
char our_thread[8] = "thread1";

int Tobbacoo_smoker(void* fyou)
{	
	printk("-Debug: inside Tobbacoo_smoker.\n");										
	while(count1<=1)								// Each smoker has two turns 									
	{	
		down(&Smoker[0]);							//  takes its semaphore 
		down(&tempSem[0]);							// wait for its turn given vy agent


		up(&Smoker[2]);								// if its turn than free both materials from its original thread			
		up(&Smoker[1]);


		printk("Tobacco holder is waiting for other two constraints\n");
		msleep(10);
		down(&Smoker[1]);							//  occupy all materials so that it can smoke
		down(&Smoker[2]);
		printk("Tabacco Holder is smoking\n");
		msleep(10);
		printk("Tobacco holder has finished smoking\n");
		
		// sem_post(&Smoker[1]);
		// sem_post(&Smoker[2]);
		up(&Smoker[0]);								// free own semaphore so that it can occupy again 				
		count1++;

	}

	do_exit(0);
	return 0;
}


int Matches_smoker(void *fyou)
{	

	printk("-Debug: inside Matches_smoker.\n");
	while(count0 <= 1)
	{	
		down(&Smoker[1]);	
		down(&tempSem[1]);	

		
		up(&Smoker[0]);
		up(&Smoker[2]);

		printk("Matches holder is waiting for other two constraints\n");
		msleep(10);
		
		down(&Smoker[0]);
		down(&Smoker[2]);

		printk("Matches Holder is smoking\n");
		msleep(10);
		printk("Matches holder has finished smoking\n");
	
		
		// sem_post(&Smoker[0]);
		// sem_post(&Smoker[2]);
		up(&Smoker[1]);
		count0++;
	}
do_exit(0);
	return 0;
}



int Paper_smoker(void *fyou)
{	
	
	printk("-Debug: inside Paper_smoker.\n");

	while(count2 <= 1)
	{
		down(&Smoker[2]);	
		down(&tempSem[2]);	

		up(&Smoker[0]);
		up(&Smoker[1]);
	
		printk("Paper holder is waiting for other two constraints\n");
		msleep(10);
		
		down(&Smoker[0]);
		down(&Smoker[1]);
		
		printk("Paper Holder is smoking\n");
		msleep(10);
		printk("Paper holder has finished smoking\n");

		
		up(&Smoker[2]);
		count2++;
	
	}
do_exit(0);
	return 0;
}





int Agent(void *fyou)
{	
	int i,j;
	msleep(10);
	
	for(i=0;i<6;i++)	
	{	
		
		int x = i%3;					// selects which threads turn 
		printk("Random: %d\n", x);
				
				
				up(&tempSem[x]);	// gives permission to only one to perform its work and stop other twos
			
		
	
		msleep(50);

		
}

	AllDone = 1;

do_exit(0);
	return 0;
}



asmlinkage long sys_helloworld(void)
{

	int i;
		for (i = 0; i < 3; ++i)
			sema_init(&Smoker[i],1);			// initialize all to 1
	

		for (i = 0; i < 3; ++i)
			sema_init(&tempSem[i],0);
	
		Atid = kthread_create(Agent,(void*)0,our_thread);
		if((Atid))
		{
			wake_up_process(Atid);
			
		}
	

	Stid[0] = kthread_create(Tobbacoo_smoker,(void*)0,our_thread);
		if((Stid[0]))
		{
			wake_up_process(Stid[0]);
			
		}

	Stid[1] = kthread_create(Matches_smoker,(void*)0,our_thread);
		if((Stid[1]))
		{
			wake_up_process(Stid[1]);
			
			
		}
		
	Stid[2] = kthread_create(Paper_smoker,(void*)0,our_thread);
		if((Stid[2]))
		{
			wake_up_process(Stid[2]);
			printk(KERN_INFO "in if");
			
		}
			


	
	msleep(2000);

	printk("After msleep();\n");	

	return 0;
}
