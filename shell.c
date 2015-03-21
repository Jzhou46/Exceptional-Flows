//
// Jeffrey Zhou
// JZhou46
// CS 361
// Process Creation and Signal Handling
// 9/28/14
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>

pid_t pid;//Child
jmp_buf buf;
int counter = 1;
int cmdLine = 3;//Precision input from command line

void sigHandler(int signal){
	if(signal == SIGUSR1){//Signal for Seg Fault
		printf("received SIGUSR1\n");
		counter++;//Increment counter
		if(counter > cmdLine){
			kill(pid, SIGUSR2);//Kill child
			printf("COUNTER NOW RESETED\n");
			counter = 1;//Reset counter		
		}
	}
	else if (signal == SIGINT){//Ctrl + c signal
		sigignore(signal);//Ignore it
	}
	else if (signal == SIGTSTP){//Ctrl + z signal
		sigignore(signal);//Ignore it
		return;
	}
}
//Function to parse a line
void parseLine(char *line, char*args[]){
	line[strlen(line)-1] = ' ';
	while(*line && (*line == ' '))
		line++;

	char *token;
	const char delim[2] = " ";
	int i = 0;
	token = strtok(line, delim);
	while( token != NULL ) {//Break up the lines into tokens
		args[i] = token;//Set the argument index equal to the token
		token = strtok(NULL, delim);
		i++;
	}
	args[i] = NULL;
}
//Function to execute commands
void execCmd(char *args[]){
	struct rusage rusage;
	struct timeval time;

	if(args[0] == NULL)
		return; 
	pid = fork();
	if(pid < 0) {
		perror("Fork failed\n");
		exit(-1);
	}
	else if (pid == 0){
		//Child work happens here
		execvp(args[0], args);
		printf("pid = %d\n", pid);
		exit(0);
	}
	else {
		//Parent work continues here
		printf("Child PID = %d\n", pid);	
		int tmp;
		int status = 1;
		int options = 0;
		if (signal(SIGUSR1, sigHandler) == SIG_ERR)//Send SIGUSR1 signal
			printf("\ncan't catch SIGUSR1\n");	
		wait4(pid, &status, options, &rusage);//Wait for the chile
		printf("page faults: %ld\n", rusage.ru_majflt);
		printf("signals received: %ld\n", rusage.ru_nsignals);
		if (WIFEXITED(status)) 
			printf("status = %d, %s\n", status, "WIFEXITED");
		if ((WEXITSTATUS(status))) 
			printf("WEXITSTATUS: %d\n", tmp);
		if (WIFSIGNALED(status)) 
			printf("%s\n", "WIFSIGNALED");       
		if ((tmp = WTERMSIG(status))) 
			printf("WTERMSIG: %d\n", tmp);
		if (WIFSTOPPED(status)) 
			printf("%s\n", "WIFSTOPPED");
		if (WSTOPSIG(status)) 
			printf("%s\n", "WSTOPSIG");

	}
}


int main(int argc, char*argv[], char*envp[]){
	printf("-------------------------\n");
	printf("  Jeffrey Zhou\n  Jzhou46\n");	
	printf("-------------------------\n");
	char *line = NULL;
	size_t size = 100;
	char *args[512];
	if(argc == 2){
		cmdLine = atoi(argv[1]);//Convert second argument to integer
		if (cmdLine < 1){//If input is less than 1
			printf("Invalid precision\n");
			return -1;
		} 
		printf("Precision was set to %d\n", cmdLine);
	}
	else{
		printf("No precision was set by the user. Precision is defaulted to 3\n");
	}
	line = (char *) malloc(size*sizeof(char*));//Allocate space for the line
	while(1){
		printf("SHELL$> ");//User prompt
		int lineSize =  getline(&line, &size, stdin);//Gets a line
		parseLine(line, args);//Pass the line here to parse
		if(strcmp(line, "exit") == 0){//If user enters "exit" 
			printf("Program exited\n");//Exit message
			return 0;//Terminates program
		}
		else{
			if (signal(SIGINT, sigHandler) == SIG_ERR)//Send SIGINT signal
				printf("\ncan't catch SIGINT\n");
			if (signal(SIGTSTP, sigHandler) == SIG_ERR)//Send SIGTSTP
				printf("\ncan't catch SIGTSTP\n");
			execCmd(args);//Execute the command
		}
	}
	free(line);//Deallocate line
	return 0;
}
