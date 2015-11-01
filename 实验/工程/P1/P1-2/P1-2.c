#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

#define BUFFER_SIZE 8

typedef struct{
	pthread_t id;
	int loop_time;
	struct tm *time;

}item;

item buffer[BUFFER_SIZE];
sem_t empty, full;
pthread_mutex_t mutex;

int in = 0, out = 0;

void  Producer(void *arg);
void  Consumer(void *arg);

int  main(int argc, char *argv[])
{
	pthread_t producer[3];
	pthread_t consumer[5];
	
	
	if(sem_init(&full,0,0) != 0 || sem_init(&empty,0,8) != 0) {
		printf("Semaphore initialization failed!");
		exit(1);
	}
	if(pthread_mutex_init(&mutex,NULL) != 0){
		printf("Mutex initialization failed!");
		exit(1);
	}

	//创建生产者进程
	pthread_create(&producer[0],NULL,(void *)Producer, NULL);
	pthread_create(&producer[1],NULL,(void *)Producer, NULL);
	pthread_create(&producer[2],NULL,(void *)Producer, NULL);
	//创建消费者进程
	pthread_create(&consumer[0],NULL,(void *)Consumer, NULL); 
	pthread_create(&consumer[1],NULL,(void *)Consumer, NULL); 
	pthread_create(&consumer[2],NULL,(void *)Consumer, NULL); 
	pthread_create(&consumer[3],NULL,(void *)Consumer, NULL); 
	pthread_create(&consumer[4],NULL,(void *)Consumer, NULL); 



pthread_join(producer[0],NULL); /*等待生产者线程结束*/
pthread_join(producer[1],NULL); 
pthread_join(producer[2],NULL); 

pthread_join(consumer[0],NULL); /*等待消费者线程结束*/
pthread_join(consumer[1],NULL); 
pthread_join(consumer[2],NULL); 
pthread_join(consumer[3],NULL); 
pthread_join(consumer[4],NULL); 


sem_destroy(&full);
sem_destroy(&empty);
pthread_mutex_destroy(&mutex);
	exit(0);
}

void  Producer(void *arg) /*生产者线程执行代码*/
{
	int i;
	time_t ltime;
	item nextProduced;
	for(i = 1 ; i <= 5 ; i++ ){
		usleep(750000);
		sem_wait(&empty);
		pthread_mutex_lock(&mutex);
		nextProduced.loop_time = i;
		time(&ltime);
		nextProduced.time = localtime(&ltime);
		nextProduced.id   = pthread_self();		

		buffer[in] = nextProduced;
		in = (in+1) % BUFFER_SIZE;
		pthread_mutex_unlock(&mutex);
		sem_post(&full);
	}
     
}

void Consumer(void *arg) /*消费者线程执行代码*/
{
	int i;
	item nextConsumed;
	for(i=1;i<=3;i++){
		usleep(750000);
		sem_wait(&full);
		pthread_mutex_lock(&mutex);
		nextConsumed = buffer[out];
		out = (out + 1) % BUFFER_SIZE;
		printf("ID为%lu的生产者线程在第%d次循环产生这条消息，消息时间为%d年%d月%d日，%d:%d:%d\n",nextConsumed.id,nextConsumed.loop_time,1900+nextConsumed.time->tm_year,1+nextConsumed.time->tm_mon,1+nextConsumed.time->tm_mday,nextConsumed.time->tm_hour,nextConsumed.time->tm_min,nextConsumed.time->tm_sec);
		pthread_mutex_unlock(&mutex);
		sem_post(&empty);  
		}
	
}
