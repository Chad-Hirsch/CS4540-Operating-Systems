/*
 * CS 4540 Assignment 3
 *
 * PROGRAM    : main.c
 * AUTHOR     : Chad Hirsch
 * DATE       : 3/4/2019
 * CLASS      : Operating Systems (CS 4540)
 */
#include <time.h>
#include <semaphore.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include "main.h"

//mother process called 'main'
int main(void){
	// sem_open both semaphores
	sem_t semS, semK;
	//fork 9 processes
	pthread_t threadIds[9];
	int nthreads = 9;
	thArgs args[9];

	int i = 0, ret = 0;

	//Init both semaphores
	initSemaphore(&semS);
	initSemaphore(&semK);

  // spawn 9 threads
  for(i = 0; i < nthreads; i++){
		args[i].index = i+1;
		args[i].semK = &semK;
		args[i].semS = &semS;

		pthread_create(&threadIds[i], NULL, process, &args[i]);
	}

	for(i=0; i < nthreads; i++){
		pthread_join(threadIds[i], NULL);
	}

	closeSemaphores(&semS, &semK);


return ret;

//End Mother process


}


void *process(void *thdArgStruct){

thArgs *args = (thArgs *)thdArgStruct;
int index = args->index;
sem_t *semS = args->semS;
sem_t *semK = args->semK;
void *ret = NULL;

// sem_open both semaphores
	int deadlocks = 0;
	char *input = (char *)calloc(sizeof(char), 128); // For getting input from the command line]
	struct timespec sleepTime = {.tv_nsec = 100000, .tv_sec = 0};

    // Sleep for a sec because I'm not getting deadlocks.
    nanosleep(&sleepTime, NULL);

// 	until quit, while not q
	while(input[0] != 'q'){
	// Even
	if( index % 2 == 0){
		deadlocks += getSemaphores(semS, semK, index);
	// Else Odd Opposite Order
	}else{
		deadlocks += getSemaphores(semK, semS, index);
	}

	//Hardin mentioned that we should sleep in order to ensure a deadlock.
	nanosleep(&sleepTime, NULL);

	printf("Enter < 80 characters or q to quit: ");

    if(fgets(input, 80, stdin) != NULL){ //read keyboard
        printf("-> %s\n", input);// echo what was typed with id before and after echo
	}else{
		printf("\n");
		input[0] = ' ';
	}

    returnSemaphore(semS);  //sem_post or give back both semaphores
	returnSemaphore(semK);
	}
	printf("This process %d, had %d deadlocks \n", index, deadlocks);
	free(input);
	return(ret);
}

// returns count of how many times recovered from deadlock
int getSemaphores(sem_t *semA, sem_t *semB, int index){ // This counts how many times it recovered from a deadlock and returns that count.
	int deadlocks = 0, waitResult;
	struct timespec waitTime, sleepTime;
	srand(time(NULL));
    sem_wait(semA);
    getWaitTimeNano(&waitTime); // This gets a randomized wait time and second semaphore.
	waitResult = sem_timedwait(semB, &waitTime);
	while(waitResult != 0){
		deadlocks++; //Increment Deadlock Counter
		returnSemaphore(semA); //Return the Semaphore we have
        getWaitTimesecs(&sleepTime);
		sleep(sleepTime.tv_sec);
        sem_wait(semA); // Get the first semaphore back.
		getWaitTimeNano(&waitTime); // Update the timeStruct
		waitResult = sem_timedwait(semB, &waitTime); // And try waiting again.
	}
	return(deadlocks);
}



void getWaitTimesecs(struct timespec *tSpec){
	int secs;
    secs = rand() % 5 + 1;
    tSpec->tv_nsec = 0;
	tSpec->tv_sec = secs;
}

void getWaitTimeNano(struct timespec *tSpec){
	long nanoSecs;
    nanoSecs = rand() % 1000 * 10000;
	tSpec->tv_sec = 0;
	tSpec->tv_nsec = nanoSecs;
}

void initSemaphore(sem_t *semaphore){
	if( sem_init(semaphore, 0, 1) ){
		perror("The request to initiate semaphore failed. \n");
		exit(1);
	}
}

sem_t *openSemaphore(sem_t *semaphore, char *semaphoreName){

	semaphore = sem_open(semaphoreName, O_CREAT, 0644, 1);
	if(semaphore == SEM_FAILED){
		perror("Semaphore has failed to open properly.\n");
		exit(1);
	}
	return(semaphore);
}

void returnSemaphore(sem_t *sema){
	int error = 0;

	error += sem_post(sema);

	if(error){
		perror("The semaphore was unable to be returned from the process. \n");
		exit(1);
	}

}

void closeSemaphores(sem_t *semS, sem_t *semK){
	int error = 0;

	error += sem_destroy(semS);
	error += sem_destroy(semK);
	if(error){
		perror("The Semaphore was unable to close. \n");
		exit(1);
	}
}
