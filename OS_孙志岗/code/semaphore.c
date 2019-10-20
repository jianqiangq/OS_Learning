// 生产者消费者问题
// 该程序调试未成功

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <assert.h>
#include <unistd.h>


#define LOOP_TIMES 1000
#define P_NUMBER 10
#define C_NUMBER 20
#define BUFFER_SIZE 5
#define BUFFER_FILE "buffer.dat"
#define FULL_SEM_NAME "full"
#define EMPTY_SEM_NAME "empty"
#define MUTEX_SEM_NAME "mutex"

sem_t *empty;
sem_t *mutex;
sem_t *full;

FILE *file;

struct buf {
    int in;
    int out; // 消费者下次取的位置
    int value; // 生产者下次放的位置
    int buffers[BUFFER_SIZE];
};

typedef struct buf buffer_t;

// 生产者
void producer(int id) {
    int i;
    buffer_t buffer;
    int rval;

    for (i = 0; i < LOOP_TIMES/P_NUMBER; i++) { // 每个生产者的数量保持平均
        sem_wait(empty);
        sem_wait(mutex); // 互斥锁

        fseek(file, 0, SEEK_SET);
        rval = fread(&buffer, sizeof(buffer), 1, file);
        if(rval == 0) {
            perror(BUFFER_FILE);
            exit(1);
        }

        buffer.buffers[buffer.in++] = buffer.value++;
        buffer.in %= BUFFER_SIZE;
        fseek(file, 0, SEEK_SET);
        fwrite(&buffer, sizeof(buffer), 1, file);
        fflush(file);

        sem_post(mutex);
        sem_post(full);

    }
}

// 消费者
void consumer(int id) {
    int i;
    buffer_t buffer;
    int result;
    int rval;

    for (i = 0; i < LOOP_TIMES/C_NUMBER; i++) { //消费者怎么知道生产者生产结束？这里投机取巧用LOOP_TIMES确定生产者生产的数量（更好的方法是使用消息机制message（对应Linux中的信号signal和kill））
        sem_wait(full);
        sem_wait(mutex);

        fseek(file, 0, SEEK_SET);
        rval = fread(&buffer, sizeof(buffer), 1, file);

        result = buffer.buffers[buffer.out++];
        buffer.out %= BUFFER_SIZE;

        fseek(file, 0 ,SEEK_SET);
        fwrite(&buffer, sizeof(buffer), 1, file);
        fflush(file);

        printf("%d: %d\n", id, result);
        fflush(stdout);

        sem_post(mutex);
        sem_post(empty);
    }
}

int main(void) {
    int i;
    pid_t pid[P_NUMBER + C_NUMBER];
    int zeros[sizeof(buffer_t)] = {0};
    file = fopen(BUFFER_FILE, "w+b");
    if (file == NULL) {
        perror(BUFFER_FILE);
        return 1;
    }
    fwrite(zeros, sizeof(zeros), 1, file);
    fflush(file); //fwrite写在当前进程的缓冲区里面，将缓冲区内容写到文件中

    sem_unlink(FULL_SEM_NAME);  //删除信号量，上一次程序错误推出，没有释放信号量，下一次连续运行时，打开信号量导致信号量打不开（存在就不打开）或者打开但是初始值为上一次运行剩下的值，导致出错
    sem_unlink(EMPTY_SEM_NAME);
    sem_unlink(MUTEX_SEM_NAME);

    full = sem_open(FULL_SEM_NAME, 0, O_CREAT, 0777, 0);
    empty = sem_open(EMPTY_SEM_NAME, 0,O_CREAT, 0777, BUFFER_SIZE);
    mutex = sem_open(MUTEX_SEM_NAME, 0, O_CREAT, 0777, 1);
    assert(full != NULL && empty != NULL && mutex != NULL); //断言，用于调试。表达式为假，则程序强制被推出，并打印出错位置信息

    for (i = 0; i < P_NUMBER; i++) {
        if (!(pid[i] = fork())) {
            producer(i);
            return 0; // 子进程到此返回
        }
    }

    for (i = 0; i < C_NUMBER; i++) {
        if (!(pid[i] = fork())) {
            consumer(i);
            return 0;
        }
    }

    for(i = 0; i < P_NUMBER + C_NUMBER; i++) {
        wait(NULL); // 父进程等待子进程，回收子进程资源
    }

    sem_unlink(FULL_SEM_NAME);
    sem_unlink(EMPTY_SEM_NAME);
    sem_unlink(MUTEX_SEM_NAME);
    fclose(file);

    return 0;


}








