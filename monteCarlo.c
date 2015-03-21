//
// Jeffrey Zhou
// JZhou46
// CS 361
// Process Creation and Signal Handling
// 9/28/14
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <ctype.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

static unsigned long int evalAttempts = 0;//Evaluations attempts counter
static unsigned long int segFaults = 0;//Seg fault counter
float percentage;//Percentage variable
jmp_buf buf;
int counter = 0;
int report = 10;//Report variable

void sigHandler(int signal){
	if(signal == SIGSEGV){//If signal is Seg Fault signal
		segFaults++;//Increment Seg Fault counter
		if(evalAttempts < ULONG_MAX)
			siglongjmp(buf,1);
		else{
			printf("The final results are: \n");
			printf("The number of segmentation faults counted: %lu\n", segFaults);
			printf("The total number of evaluations attempted: %lu\n", evalAttempts);
			printf("Percentage of segmentation fault attempts: %f\n", percentage);
			exit(signal);
		}
	}
	else if (signal == SIGINT){//If signal is ctrl + c signal
		printf("received SIGINT\n");
		printf("The final results are: \n");
		printf("The number of segmentation faults counted: %lu\n", segFaults);
		printf("The total number of evaluations attempted: %lu\n", evalAttempts);
		printf("Percentage of segmentation fault attempts: %f\n", percentage);
		exit(signal);
	}
	else if (signal == SIGTSTP){//If signal is ctrl + z signal
		printf("received SIGTSTP\n");
		printf("The current results are: \n");
		printf("The number of segmentation faults counted: %lu\n", segFaults);
		printf("The total number of evaluations attempted: %lu\n", evalAttempts);
		printf("Percentage of segmentation fault attempts: %f\n", percentage);
		return;
	}
	else if (signal == SIGALRM){//If signal is alarm signal
		printf("received SIGALRM\n");
		printf("The final results are: \n");
                printf("The number of segmentation faults counted: %lu\n", segFaults);
                printf("The total number of evaluations attempted: %lu\n", evalAttempts);
                printf("Percentage of segmentation fault attempts: %f\n", percentage);
		exit(signal);
	}
	else if (signal == SIGUSR2){//If signal is SIGUSR2 signal
		printf("received SIGUSR2\n");
		printf("The final results are: \n");
		printf("The number of segmentation faults counted: %lu\n", segFaults);
		printf("The total number of evaluations attempted: %lu\n", evalAttempts);
		printf("Percentage of segmentation fault attempts: %f\n", percentage);
		exit(signal);
	}
}

int main(int argc, char *argv[]){
	printf("-------------------------\n");
	printf("  Jeffrey Zhou\n  Jzhou46\n");
	printf("-------------------------\n");
	srand(time(NULL));
	if(argc > 2){
		printf("Invalid amount of arguments\n");
		return -1;	
	}
	else if(argc == 2){
		float sec;//Seconds variable
		sec = atof(argv[1]);//Convert from character to float
		if(sec > 0.0) {
			printf("The maximum number seconds to run before stopping is %f\n", sec);
			alarm(sec);//Set the alarm
		}
		else{
			fprintf(stderr, "invalid seconds\n");
			return -1;
		}
	}
	else if(argc == 1){//In user don't enter a precision
		printf("Program will run until loop ends or precision found\n");
	}
	int *ptr, catch;
	pid_t ppid = getppid();//Get the parent ID
	for(evalAttempts; evalAttempts < ULONG_MAX; evalAttempts++){
		percentage = ((float)segFaults)/(float)evalAttempts;//Compute percentage
		if(evalAttempts == report){
			kill( ppid, SIGUSR1 );//Kill parent
			printf("report variable = %d\n", report);
			report = report * 10;
		}
		if (signal(SIGSEGV, sigHandler) == SIG_ERR)//Send Seg Fault signal
			printf("\ncan't catch SIGSEGV\n");
		if (signal(SIGINT, sigHandler) == SIG_ERR)//Send ctrl + c signal
			printf("\ncan't catch SIGINT\n");			
		if (signal(SIGTSTP, sigHandler) == SIG_ERR)//Send ctrl + z signa
			printf("\ncan't catch SIGTSTP\n");
		if (signal(SIGALRM, sigHandler) == SIG_ERR)//Send alarm signal
			printf("\ncan't catch SIGALRM\n");	
		if (signal(SIGUSR2, sigHandler) == SIG_ERR)//Send SIGUSR2 signal
			printf("\ncan't catch SIGUSR2\n");
		if(sigsetjmp(buf,1) == 0){
			//Cause a seg fault
			ptr = (int*)rand();
			catch = *ptr;
		}
	}
		printf("The final results are: \n");
		printf("The number of segmentation faults counted: %lu\n", segFaults);
		printf("The total number of evaluations attempted: %lu\n", evalAttempts);
		printf("Percentage of segmentation fault attempts: %f\n", percentage);
		int exitVal = (int)percentage;
		printf("Exit value: %d\n", exitVal);	
	return 0;
}
