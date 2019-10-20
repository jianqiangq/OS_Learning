#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* thread1(void *param);
void* thread2(void *param);

unsigned long value = 0;

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

int main(void) {
	pthread_t tid_1, tid_2;
	pthread_create(&tid_1, NULL, thread1, NULL);
	pthread_create(&tid_2, NULL, thread2, NULL);
	for(;;) {
		printf("%lu\n", value);
		sleep(1);
	}
}

// 下面两个线程产生死锁 

void* thread1(void *param) {
	for(;;) {
	    pthread_mutex_lock(&mutex1);
	    pthread_mutex_lock(&mutex2);
	    value++;
	    pthread_mutex_unlock(&mutex1);
	    pthread_mutex_unlock(&mutex2);	
	}
	
}

// void* thread2(void *param) {
// 	for(;;) {
// 		pthread_mutex_lock(&mutex2);
// 	    pthread_mutex_lock(&mutex1);
// 	    value++;
// 	    pthread_mutex_unlock(&mutex2);
// 	    pthread_mutex_unlock(&mutex1);
// 	}
// }


 // 防止死锁办法：申请锁1和锁2的顺序相同，某一个线程拿到了锁1的话，这个线程一定可以拿到锁2。这样的话这个线程可以顺利执行完并最终释放锁1和锁2.
void* thread2(void *param) {
	for(;;) {
	    pthread_mutex_lock(&mutex1);
	    pthread_mutex_lock(&mutex2);
	    value++;
	    pthread_mutex_unlock(&mutex2);
	    pthread_mutex_unlock(&mutex1);
	}
}
