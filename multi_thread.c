#define _XOPEN_SOURCE
#define _BSD_SOURCE
#define _SVID_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>


pthread_mutex_t mutex;
volatile int total_counts = 0;


// Calculate the times in which points are in the circle.
void thread(void *each_loop_num);

int main(int argc, char *argv[]) {
    if(argc != 3){
        printf("Invalid arguments\n");
        return 0;
    }
    
    int totalLoop = atoi(argv[1]);
    int threadCounts = atoi(argv[2]);
    int eachLoopCounts = totalLoop / threadCounts;

    pthread_t id[ threadCounts ];
    pthread_mutex_init(&mutex, NULL);//initialize the mutex

    /* pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg); */
    for(int i = 0; i < threadCounts; i++)
        pthread_create(&id[i], NULL, (void*)thread, &eachLoopCounts);
    for(int i = 0; i < threadCounts; i++)
        pthread_join(id[i], NULL);
    
    /* print out the result */
    printf("pi: %.10f\n", 4.0 * total_counts / totalLoop);
    return 0;
}

void thread(void *each_loop_counts) {
    int localCounts = 0;
    double x, y;
    struct drand48_data randBuff;

    srand48_r(time(NULL), &randBuff);
    for(int i = 0; i < *(int*)each_loop_counts; i++) {
        drand48_r(&randBuff, &x); /* To gaurantee the thread-safe */
        drand48_r(&randBuff, &y); /* To gaurantee the thread-safe */
        if((x*x + y*y) < 1)
            localCounts++;
    }

    pthread_mutex_lock(&mutex);
    total_counts += localCounts;
    pthread_mutex_unlock(&mutex);
}