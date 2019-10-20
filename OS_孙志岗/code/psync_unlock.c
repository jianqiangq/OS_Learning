#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define STEP 100

unsigned long add_counter(FILE* fp) {
    unsigned long counter = 0;
    rewind(fp); //将文件指针指向文件开始位置
    fread(&counter, sizeof(counter), 1, fp);
    counter++;
    rewind(fp);
    fwrite(&counter, sizeof(counter), 1, fp);
    return counter;
}

int main(void) { // linux多个进程允许访问同一个文件.进程同时访问共享的数据，可能会破坏数据。
    FILE *fp;
    fp = fopen("counter.dat", "w+b");
    unsigned long counter;
    pid_t pid;
    pid = fork();
    if(pid == 0) { //child
        for(;;) {
            counter = add_counter(fp);
            if(counter % 100 == 0) printf("child counter %lu\n", counter);
        }
    }
    if(pid > 0) { //parent
        for(;;) {
            counter = add_counter(fp);
            if(counter % 100 == 0) printf("parent counter\t\t %lu \n", counter);
        }
    }
}