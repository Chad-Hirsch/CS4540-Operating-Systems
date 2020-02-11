/*
 * CS 4540 Assignment 4
 *
 * PROGRAM    : server.c
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

#define QUEUE_PERMISSIONS 0660

struct msgbuff {
    int qid;
    char buf [200];
    double totalamount;
    double amount;
    char response[200];
    long message_type;
};

//deposit amount
double Deposit(double *totalamount,double amount)
{
	*totalamount+=amount;
	return 1;
}
//withdraw amount
double Withdraw(double *totalamount,double withdrawamount)
{
	if(withdrawamount<*totalamount){
		*totalamount-=withdrawamount;
		return 1;
	}
	return -1;
}
//charge amount
double Charge(double *totalamount,double charge)
{
	if(charge<*totalamount){
		*totalamount-=charge;
		return 1;
	}
	return -1;
}
//transfer amount
double Transfer(double *totalamount,double transferamount)
{
	if(transferamount<*totalamount){
		*totalamount-=transferamount;
		return 1;

	}
	return -1;
}
//exit the program
void shutdown(int count)
{
	printf("Total Msgs Received is %d\n",count);
	exit(0);//exit program
}

int savechattofile(char *data)
{
	FILE *out=fopen("log.txt","a"); //open file in append
	if(out==NULL){
		printf("Could not open the file");//exit if file not

//exit
		return -1;
	}
	fprintf(out,"%s",data); //save data to file
	fclose(out);
	return 1;

}


int responsetoclient(char *messaagefromclient,char **preparemessage,double *totalamount,double newamount,int qid,int *count)
{
	char msg[200];
	if(*messaagefromclient==49)
	{
		if(Deposit(&(*totalamount),newamount)==-1){
			strcpy(*preparemessage,"Amount could not deposit");
			return -1;
		}
			strcpy(*preparemessage,"OK");
			if(sprintf(msg, "\nDeposit from  %d \n", qid)==2){printf("Error in making String\n");}

			// process message
			printf("\nDeposit Request  From  %d\n",qid);
			savechattofile(msg);
			*count=*count+1;

			return 1;
	}
	if(*messaagefromclient==50)
	{
		if(Withdraw(&(*totalamount),newamount)==-1)
		{
			strcpy(*preparemessage,"Not enough funds");
			return -1;
		}
			strcpy(*preparemessage,"OK");
		if(sprintf(msg, "\nWithdraw from  %d \n", qid)==2){printf("Error in making String\n");}

		// process message
		printf("\nWithdraw Request  From  %d\n",qid);
		savechattofile(msg);
			*count=*count+1;
		return 1;
	}
	if(*messaagefromclient==51)
	{
		if(Charge(&(*totalamount),newamount)==-1)
		{

			strcpy(*preparemessage,"Not enough funds");
			return -1;
		}
			strcpy(*preparemessage,"OK");
			if(sprintf(msg, "\nCharge from  %d \n", qid)==2){printf("Error in making String\n");}

			// process message
			printf("\nCharge Request  From  %d\n",qid);
			savechattofile(msg);
			*count=*count+1;
			return 1;
	}
	if(*messaagefromclient==52)
	{
		if(Transfer(&(*totalamount),newamount)==-1)
		{

			strcpy(*preparemessage,"Not enough funds");
			return -1;
		}
			strcpy(*preparemessage,"OK");
			if(sprintf(msg, "\nTransfer from  %d \n", qid)==2){printf("Error in making String\n");}

			// process message
			printf("\nTransfer Request  From  %d\n",qid);
			savechattofile(msg);
			*count=*count+1;
			return 1;
	}
	if(*messaagefromclient==53)
	{
		strcpy(*preparemessage,"OK");
		if(sprintf(msg, "\nBalance Inquiry from  %d \n", qid)==2){printf("Error in making String\n");}

		// process message
		printf("\nBalance Inquiry  From  %d\n",qid);
		savechattofile(msg);
			*count=*count+1;
		return 1;
	}
	if(*messaagefromclient==54)
	{
		shutdown(*count);
	}
}
int main (int argc, char **argv)
{
    key_t msg_queue_key;
    int qid;
    struct msgbuff message;
	double totalamount=500;
//create fork new messagae queue
	msg_queue_key=ftok("prog_file",65);
    if ((qid = msgget (msg_queue_key, IPC_CREAT | QUEUE_PERMISSIONS)) == -1) {
        perror ("msgget");
        exit (1);
    }

    printf ("Server: Hello, World!\n");
	int countva=0;
	char *preparemessage;
    while (1) {
        // read an incoming message
//The msgsnd system call is used to sending messages to

  //aSystem
//V message queue.
        if (msgrcv (qid, &message, sizeof (struct msgbuff), 0, 0) == -1) {
            perror ("msgrcv");
            exit (1);
        }

	printf ("Server: message received.\n");
	responsetoclient(message.buf,&preparemessage,&totalamount,message.amount,qid,&countva);
	message.totalamount=totalamount;
//copy the *pointer char to char array
	strncpy(message.response,preparemessage,sizeof message.response-1);

        int client_qid = message.qid;
        message.qid = qid;

        // send reply message to client
        if (msgsnd (client_qid, &message, sizeof (struct msgbuff), 0) == -1) {
            perror ("msgget");
            exit (1);
        }

        printf ("Server: response sent to client.\n");
    }
}
