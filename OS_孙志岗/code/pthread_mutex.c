
#include <pthread.h>
#include <stdio.h>

unsigned long value;
unsigned long parent_value;
unsigned long child_value;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* adder(void* param) {

    for(;;) {
        pthread_mutex_lock(&mutex);
        value++;
        pthread_mutex_unlock(&mutex);
        child_value++;
    }
}


int main(void) {
    pthread_t tid_adder;
    pthread_create(&tid_adder, NULL, adder, NULL);
    for(;;) {
        pthread_mutex_lock(&mutex);
        value++;
        pthread_mutex_unlock(&mutex);
        parent_value++;
        if(value % 100000 == 0) {
            printf("V:%lu P:%lu C:%lu \n", value, parent_value, child_value);
            printf("DIFF:%ld\n", (parent_value + child_value) - value);
        }
        // pthread_mutex_unlock(&mutex); 为了printf结果正确pthread_mutex_unlock(&mutex)放在这个地方

    }
}