/*
 * CS 4540 Assignment 4
 *
 * PROGRAM    : client.c
 * AUTHOR     : Chad Hirsch
 * DATE       : 3/17/2019
 * CLASS      : Operating Systems (CS 4540)
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


struct msgbuff { //structure to receive and send the data
    int qid;
    char buf [200];
    double totalamount;
    double amount;
    char response[200];
    long message_type;
};

int menu()
{
	int x;
	printf("\n1-Deposit");
	printf("\n2-WithDraw");
	printf("\n3-Charge");
	printf("\n4-Transfer");
	printf("\n5-Balance Check");
	printf("\n6-Shutdown");
	printf("\nEnter you choice   ");
	scanf("%d",&x);

	if(x==1 ||x==2|| x==3||x==4 ||x==5 ||x==6)
		return x;
	else
		return -1;
}

int action(int x,double *amount)
{
	struct msgbuff message;
	if(x==1)
	{
		printf("\nEnter amount to deposit  ");
		scanf("%lf",&(*amount));
		return 1;
	}
	if(x==2)
	{
		printf("\nEnter amount to withdraw  ");
		scanf("%lf",&(*amount));
		return 1;
	}
	if(x==3)
	{
		printf("\nEnter amount to Charge  ");
		scanf("%lf",&(*amount));
		return 1;
	}
	if(x==4)
	{
		printf("\nEnter amount to Transfer  ");
		scanf("%lf",&(*amount));
		return 1;
	}
	if(x==5)
	{
		return 1;
	}
	if(x==6)
	{
		return 1;
	}

	return 0;
}
int main (int argc, char **argv)
{
    key_t server_queue_key;
    int server_qid, myqid;
    struct msgbuff my_message, return_message;

    // create my client queue for receiving messages from server
    if ((myqid = msgget (IPC_PRIVATE, 0660)) == -1) {
        perror ("msgget: myqid");
        exit (1);
    }


	server_queue_key = ftok("prog_file",65);

    if ((server_qid = msgget (server_queue_key, 0)) == -1) {
        perror ("msgget: server_qid");
        exit (1);
    }

    my_message.message_type = 1;
    my_message.qid = myqid;



    while (1) {

	int x=-1;
	while(x==-1)
		x=menu(); //show the menu

	my_message.buf[0]=x+'0';

	while(action(x,&my_message.amount)==0){ //

	}

        // remove newline from string
        int length = strlen (my_message.buf);
        if (my_message.buf [length - 1] == '\n')
           	my_message.buf [length - 1] = '\0';

        // send message to server
//The msgsnd system call is used to sending messages to a System
//V message queue.
        if (msgsnd (server_qid, &my_message, sizeof (struct msgbuff), 0) == -1) {
            perror ("client: msgsnd");
            exit (1);
        }

	if(x==6)
		exit(0);//exit if the shutdown option is use
        // read response from server
//The msgrcv system call is for receiving messages from a System
//V message queue identified by msqid.
        if (msgrcv (myqid, &return_message, sizeof (struct msgbuff), 0, 0) == -1) {
            perror ("client: msgrcv");
            exit (1);
        }

        // process return message from server
        printf ("\nMessage received from server: %s \n Total Amount Now %lf\n\n", return_message.response,return_message.totalamount);

    }
    // remove message queue
	//msgctl, we can do control operations on a message queue
	//identified by msqid.
    if (msgctl (myqid, IPC_RMID, NULL) == -1) {
            perror ("client: msgctl");
            exit (1);
    }

    printf ("Client: bye\n");

    exit (0);
}
