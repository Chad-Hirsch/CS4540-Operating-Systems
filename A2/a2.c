/*
 * CS 4540 Assignment 2 - Scheduling
 *
 * PROGRAM    : a2.c
 * AUTHOR     : Chad Hirsch
 * DATE       : 2/13/2019
 * CLASS      : Operating Systems (CS 4540)
 * DESCRIPTION: This is a simulation of the CPU scheduling
 */

#include "a2.h"
#include<stdio.h>
#include<stdlib.h>
#define MAX 50
#define P_COUNT 48

void insert_in_cpu(int id,int runTime,int timeProcess, int quantum,int cpucount);//CP queue
void exit_from_cpu(int data);//The exit function for exiting from burst to either i/o or exit
void check(int);//Additional function for queue checking
void create();//Creating queue
void display_Pqueue();//For Displaying CPU queue
void display_Pqueue();//For Displaying i/o queue
void printStats(process a[], os system);//The required printing format

void insert_in_io(int data);//The I/O queue after exiting from CPU queue
void exit_from_io(int data);//The exiting of processes from I/O
void check1(int);

int insert(int item,int itprio,int p_id,int tp,int curr_p,int rqt,int cpuTime,int runTime,int quantum); //All arguments to perform specific operation
int del(int id,int cpuTime);//process_id deletion according to priority
int display();//Display the process entered in ready queue

struct node//structure for priority ready queue node
{
    int priority;
    int curprior;
    int id;
    int tp;
    int quantum;
    int runTimeForCpu;
    int currentCpuCount;
    int cpu;
    int info;
    struct node *next;
}*start=NULL,*q,*temp,*new;


typedef struct node N;//Defining structure as N

ui cpuQueue[MAX];//the CPU Queue
ui waitingQueue[MAX];//the I/O Queue
int front, rear;
int countD=0;//For counting processes


int main (void)
{

    int timeUnit=0;//For time increments in whenever process goes in ready queue
    int tp=0;
    ui q = 70;
    srand(time(NULL));//For generating/seeding different starting times
    process a[48];
    os b[48];
    ui queue[48],io[48],cputime[48],prior[48];
    ui timeCountP[48];//Time counter for each process
    int i =0,process_id[48];//To track procedure id's
    ui priority[48],readyQueueTime[48],waitingQueueTime[48],execTime[48];//Variables to fetch data from file
    a[48].runTime;//Run time for a process in CPU
    b[48].quantum = q;//Quantum of each process which is  70
    ui waitSum = 90; // total time in wait queue
	ui waitCount = 56; // how many times in wait queue (for average)
	ui waitMin = 40; // smallest time in wait queue
	ui waitMax=54;



    FILE* in_file = fopen("a2TestFile.txt", "r"); // read only

    if(! in_file ) // equivalent to saying if ( in_file == NULL )
    {
        perror(NULL);// It overrides the standard errors and have its own functionality
        exit(-1);
    }
    // Going through all the file if perror() dosen't find an error
    for(i=0;i<48;i++)
    {
        fscanf(in_file,"%i %i %i %i" , &prior[i],&queue[i],&io[i],&cputime[i]);//Collecting in COLUMN format

        priority[i] = prior[i];
        readyQueueTime[i] = queue[i];
        waitingQueueTime[i] = io[i];
        execTime[i] = cputime[i];
    }
    printf("Priority\treadyQueueTime\tI/OQueueTime\tCPUtime\n");//Printing the information of processes entered
    for(int j=0;j<48;j++)
    {
        printf("%i\t\t%i\t\t%i\t\t%i\n",priority[j],readyQueueTime[j],waitingQueueTime[j],execTime[j]);
    }
    printf("\n\n\n");

    //Assigning random starting rime to each process
    int startingTime[48];

    for(int i=0;i<48;i++)
    {
        startingTime[i]= rand() % 20;
        printf(" Process %d starting time is %d\n",i+1,startingTime[i]);//Process id with associated starting time
        process_id[i] = i+1;
    }
    //Assigning values to process structure
    for(int i=0;i<48;i++)
    {
        a[i].priority = priority[i];
        a[i].cpu = readyQueueTime[i];
        a[i].io = waitingQueueTime[i];
        a[i].runTime = execTime[i];


        printf("Priority of process %d is %i and ready time is %i and waiting "
               "time is %i\n",process_id[i] ,a[i].priority,a[i].cpu,a[i].io);//Assigned values printing
    }

    //Sorting process on basis of starting time
    int temp=0;
    int temp1=0;
    int temp2=0;
    for(int i=0;i<48;i++)
    {
        for(int j=i;j>=1;j--)
        {
            if(startingTime[j]<startingTime[j-1])
            {
                temp=startingTime[j-1];
                startingTime[j-1]=startingTime[j];
                startingTime[j]=temp;

                temp1 = process_id[j-1];
                process_id[j-1] = process_id[j];
                process_id[j] = temp1;
            }
            else if(startingTime[j]==startingTime[j-1])
            {
                if(startingTime[j]<startingTime[j-1])
                {
                    temp=startingTime[j-1];
                    startingTime[j-1]=startingTime[j];
                    startingTime[j]=temp;

                    temp2 = process_id[j-1];
                    process_id[j-1] = process_id[j];
                    process_id[j] = temp2;
                }
            }
        }
    }

    //Displaying Sorted processes according to arrival time
    printf("\n\n\n");
    int count=0;
    for(int i=0;i<48;i++)
    {
        printf("starting time is %d with process id %d priority=%d\n",startingTime[i],process_id[i],priority[i]);
        count++;
    }

    printf("\nTotal number of process are %d",count);//Total number of processes

    //-------------------------------------------------------//
    //Implementing Priority  queue by linked lists
    for(i=0;i<48;i++)
    {
        //insert(startingTime[i],priority[i],process_id[i],tp);
        timeUnit++;//Time unit for tracking how many times it is called
        a[i].curPrior = priority[i];
        timeCountP[i]  = insert(startingTime[i],priority[i],process_id[i],tp,a[i].curPrior,readyQueueTime[i],cputime[i],a[i].runTime,b[i].quantum);
    }

    printf("Priorities after sorting:\n\n");
    int quan = display();
    b[48].quantum = q;//The value of quantum is stored

    printf("\nGoing to CPU QUEUE in this order\n");
    for(int i=0;i<48;i++)
    {
        process_id[i]=del(process_id[i],cputime[i]);
    }
    printf("\n\n\n");

    //The counter Time
    printf("TimeUnit = %d" , timeUnit);
    //Assigning waiting times for each process to structures
    for(int i=0; i<48;i++)
    {
        a[i].waitSum  = waitSum;
        a[i].waitMin = waitMin;
        a[i].waitMax = waitMax;
        a[i].waitCount = waitCount;
    }

    printf("\n\n\n");

    //For final printing
    printStats(a,b[48]);
    fclose(in_file);

    return 0;
}

int display()
{
    temp=start;
    if(start==NULL)
    printf("QUEUE IS EMPTY\n");//If queue is empty
    else
    {
        printf("QUEUE IS:\n");
        if(temp!=NULL)
        for(temp=start;temp!=NULL;temp=temp->next)//Until All processes DONE printing
        {
            printf("\nStarting time = %d priority =%d and current priority is %d and process id is %d\n",temp->info,temp->priority,temp->curprior,temp->id);
            //temp=temp->next;
        }
    }
    return new->quantum;
}
int del(int id,int cpuTime)//Deleting is actually sending them to CPU burst
{
    int tempId=0;
    if(start==NULL)
    {
        printf("\nQUEUE UNDERFLOW\n");
    }
    else
    {
        //Keeping  track of process ID, priority and their arrival times
        new=start;
        printf("\n starting time=%d process id is %d and current priority is %d\n",new->info,new->id,new->curprior);
        start=start->next;
        //free(start);
    }
    //Inserting into CPU queue
    insert_in_cpu(new->id,new->runTimeForCpu,new->tp,new->quantum,new->currentCpuCount);
    return new->id;
}
int insert(int item,int itprio,int p_id,int tp,int curr_p,int rqt,int cpuTime,int runTime,int quantum)
{
    //All operations for processes
    process c[48];
    os o[48];
    c[48].curCpu;
    int incPrior=0;
    tp++;
    o[48].wait = tp;
    quantum  = 70;
    o[48].quantum = quantum;
    //IF time is 30 increase priority//
    if(tp == 30)
    {
        incPrior =  new->curprior;
        new ->priority = incPrior;
    }
    if(tp == rqt)//If ready queue time is over it moves to CPU
    {
        del(new->id,new->cpu);
    }
    new=(N*)malloc(sizeof(N));

    //Fetching all processes information that's required
    new->info=item;
    new->id=p_id;
    new->tp = tp;
    new->currentCpuCount = c[48].curCpu;
    new->quantum = quantum;
    new->runTimeForCpu = runTime;
    new->cpu= cpuTime;
    new->curprior = curr_p;
    new->priority=itprio;
    new->next=NULL;

    if(start==NULL )
    {
        //new->next=start;
        start=new;
    }
    else if(start!=NULL&&itprio>=start->priority)
    {
        new->next=start;
        start=new;
    }
    else
    {
        q=start;
        while(q->next != NULL && q->next->priority>=itprio)
        {
            q=q->next;
        }
        new->next=q->next;
        q->next=new;
    }
   return tp;
}
//Initializing for Queue
void create()
{
    front = rear = -1;
}
void insert_in_cpu(int data,int runTime,int timeProcess,int quantum,int cpucount)
{
    //Inserting process now in CPU Queue
    if (rear >= MAX - 1)
    {
        printf("\nQueue overflow no more elements can be inserted");
        return;
    }
    if ((front == -1) && (rear == -1))
    {
        front++;
        rear++;
        cpuQueue[rear] = data;
        return;
    }
    else
        check(data);
    rear++;
    cpucount++; // How many times a process enters in CPU
    if(cpucount == quantum)//If CPU count == QUANTUM which is equal to 70
    {
        runTime = runTime - cpucount;//If the count = 70 it preempts
        exit_from_cpu(data);// And moves process to I/O queue
    }
    else if(cpucount == runTime)
    {
        exit_from_cpu(data);//It exits the process completely
    }
}
// Function to check priority and place element
void check(int data)
{
    int i,j;

    for (i = 0; i <= rear; i++)
    {
        if (data >= cpuQueue[i])
        {
            for (j = rear + 1; j > i; j--)
            {
                cpuQueue[j] = cpuQueue[j-1];

            }
            cpuQueue[i] = data;
            return;
        }
    }
    cpuQueue[i] = data;
}
void exit_from_cpu(int data)
{
    //All process exits to either I/O or just finishes
    int i;
    printf("\n%d",data);
    if ((front==-1) && (rear==-1))
    {
        printf("\nQueue is empty no elements to delete");
        return;
    }

    for (i = 0; i <= rear; i++)
    {
        if (data == cpuQueue[i])
        {
            for (; i < rear; i++)
            {
                cpuQueue[i] = cpuQueue[i+1];
//                insert_in_io();
            }

        cpuQueue[i] = -99;
        rear--;

        if (rear == -1)
            front = -1;
        return;
        }
    }
    printf("\n%d not found in queue to delete", data);
}

void insert_in_io(int data)//Insertion of all processes to I/O Queue
{
    process d[48];
    //All information fetching that is required for I/0 Queue tracking
    int totalioTime = d[48].io;
    int waitLimit = d[48].wait;
    int currio = d[48].curIo;
    d[48].ioTotal;
    int waitingCount = d[48].waitCount;
    waitingCount++;
    int sum = d[48].waitSum;
    sum = sum + waitingCount;

    currio++;//Count for i/o queue

    if (rear >= MAX - 1)
    {
        printf("\nQueue overflow no more elements can be inserted");
        return;
    }
    if ((front == -1) && (rear == -1))
    {
        front++;
        rear++;
        waitingQueue[rear] = data;
        return;
    }
    else
        check(data);
    rear++;
    currio++; // How many times a process enters in CPU
    if(currio == waitLimit)//If wait limit is over it exits the I/O queue
    {
        totalioTime - totalioTime - currio;
        exit_from_cpu(data);//It exits the process completely
    }
}
void exit_from_io(int data)//From exiting i/o queue processes moves again to ready queue to complete burst time
{
    int i;
    printf("\n%d",data);
    if ((front==-1) && (rear==-1))
    {
        printf("\nQueue is empty no elements to delete");
        return;
    }

    for (i = 0; i <= rear; i++)
    {
        if (data == waitingQueue[i])
        {
            for (; i < rear; i++)
            {
                waitingQueue[i] = waitingQueue[i+1];
//                insert_in_io();
            }

        waitingQueue[i] = -99;
        rear--;

        if (rear == -1)
            front = -1;
        return;
        }
    }
    printf("\n%d not found in queue to delete", data);
}
void display_pqueue()//For printing CPU Queue
{

    if ((front == -1) && (rear == -1))
    {
        printf("\nQueue is empty");
        return;
    }

    for (; front <= rear; front++)
    {
        countD++;
        printf(" %d ", cpuQueue[front]);
    }

    front = 0;
}
void display_pqueue1()//For displaying I/0 queue
{

    if ((front == -1) && (rear == -1))
    {
        printf("\nQueue is empty");
        return;
    }

    for (; front <= rear; front++)
    {
        countD++;
        printf(" %d ", waitingQueue[front]);
    }

    front = 0;
}
/*
After the loop stops and all process have exited call function provided to print the statistics for
(1)the OS setup for time limits
(2) each process’s stats including total, min and max times
*/

/*
printStats: Hardin's printStats.c source file that he sent everyone to use doesn't have this "avgs[i].average = 49.2+i; avgs[i].priority = 7+i/8;" part in it, and his sorts the processes before printing.
*/
/*void printStats(process a[], os system) {//The finalized print stats Format
	int i, j;
	struct averages {
	     double average;
	      ui priority;
        } avg, avgs[P_COUNT];
    printf("\n INSIDE printStats. \n");
	printf("\nTime quantum %i,  Maximum Wait %u\n", system.quantum, system.wait);
	printf("\nAverage Wait   Priority\n");
    // end for each process
	for (i = 0; i < P_COUNT; i++) {
            avgs[i].average = 49.2+i;
            avgs[i].priority = 7+i/8;
		printf("%12.2lf   %8u\n", avgs[i].average, avgs[i].priority);
	}
}
*/
void printStats(process a[], os system) {
	int i, j;
	struct averages { double average; ui priority; } avg, avgs[P_COUNT];
	printf("\nTime quantum %u,  Maximum Wait %u\n", system.quantum, system.wait);
	printf("\nAverage Wait   Priority\n");
	for (i = 0; i < P_COUNT; i++) {
		avg.priority = a[i].priority;
		avg.average = (double)a[i].waitSum / (double)a[i].waitCount;
		// sorted insert
		j = i - 1;
		while (j >= 0 && avgs[j].average > avg.average) {
			avgs[j + 1] = avgs[j];
			j--;
		}
		avgs[j + 1] = avg;
	} // end for each process
	for (i = 0; i < P_COUNT; i++) {
		printf("%12.2lf   %8u\n", avgs[i].average, avgs[i].priority);
	}
}
