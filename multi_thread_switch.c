#include "coro.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

coro_context main_ctx;
coro_context parallel_ctx;

coro_context ctxa;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

coro_context *current_main_ctx;


void parallel_coro_func(void *arg);

void* thread_main_func(void* arg)
{
    pthread_t pid = pthread_self();
    printf("subthread:%lu\n", pid);
    void *stk = malloc(8192);
    coro_create(&parallel_ctx, &parallel_coro_func, NULL, stk, 8192);
    coro_context ctx;
    coro_transfer(&ctx, &parallel_ctx);
}

void parallel_coro_func(void *arg)
{
    while(1) {
        pthread_mutex_lock(&mutex);
        current_main_ctx = &parallel_ctx;
        coro_transfer(&parallel_ctx, &ctxa);
        pthread_mutex_unlock(&mutex);
        usleep(1000*10);
    }
}

void coro_funca(void *arg) 
{
    pthread_t ptid;
    while(1) {

        int c = 0;
        printf("----------start loop---------\n");

        ++c;
        ptid = pthread_self();  
        printf("coro_funca, thread: %lu, count:%d\n", ptid, c);
        coro_transfer(&ctxa, current_main_ctx);

        ++c;
        ptid = pthread_self();  
        printf("coro_funca, thread: %lu, count:%d\n", ptid, c);
        coro_transfer(&ctxa, current_main_ctx);

        ++c;
        ptid = pthread_self();  
        printf("coro_funca, thread: %lu, count:%d\n", ptid, c);
        coro_transfer(&ctxa, current_main_ctx);

        ++c;
        ptid = pthread_self();  
        printf("coro_funca, thread: %lu, count:%d\n", ptid, c);
        coro_transfer(&ctxa, current_main_ctx);

        printf("----------end loop---------\n\n");
    }
}

int main(void)
{
    void *stka = malloc(8192);
    coro_create(&ctxa, &coro_funca, NULL, stka, 8192);

    pthread_t ptid1;     
    if(pthread_create(&ptid1, NULL, &thread_main_func, NULL) != 0) {
        exit(0); 
    }
    pthread_t mpid = pthread_self();
    printf("main thread:%lu\n", mpid);
    while(1) {
        pthread_mutex_lock(&mutex);
        current_main_ctx = &main_ctx;
        coro_transfer(&main_ctx, &ctxa);
        pthread_mutex_unlock(&mutex);
        usleep(1000*10);
    }
    return 0;
}
