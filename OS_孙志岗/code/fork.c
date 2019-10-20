#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	pid_t pid;
	pid = fork();
	if(pid < 0) {
		exit(-1);
	}
	else if(pid > 0) {
		printf("%s\n", "parent process");
		wait(NULL);
		printf("%s\n", "sun process ended");
	}
	else {
		for(int i = 0; i < 10; i++) {
			printf("%s %d\n", "sun process", i);
		}
	}
}
