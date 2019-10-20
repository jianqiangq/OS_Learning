
// 线程创建
#include <stdio.h>
#include <pthread.h>
#define true 1

int thread_exit = 0;

void *adder(void *param);

int main() {
    unsigned long long value = 0;
    pthread_t tid_adder;
    pthread_create(&tid_adder, NULL, adder, (void *)&value);
    while(true) {
        printf("%s %lld\n", "value is ", value);
        if(value > 1000000000)  //效率低，cpu频繁检测value值
            thread_exit = 1;
    }
    return 0;
}

void *adder(void *param) {
    while(!thread_exit) {
        ++(*(int *)param);
    }
    return NULL;
}

