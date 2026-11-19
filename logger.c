#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "campus_compute.h"

#define QUEUE_SIZE 5

typedef struct {
    int buffer[QUEUE_SIZE];
    int head;
    int tail;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
} BoundedQueue;

static BoundedQueue q = { .head = 0, .tail = 0, .count = 0 };

void* producer(void* arg) {
    FILE* log_file = fopen("campus_compute.log", "w");
    for (int i = 1; i <= 5; i++) {
        pthread_mutex_lock(&q.mutex);
        while (q.count == QUEUE_SIZE) {
            pthread_cond_wait(&q.not_full, &q.mutex);
        }
        q.buffer[q.tail] = i * 100; // Event code
        q.tail = (q.tail + 1) % QUEUE_SIZE;
        q.count++;
        
        printf("[Producer] Generated Event: Job Log ID %d\n", i * 100);
        if (log_file) {
            fprintf(log_file, "LOG_EVENT: Created job batch item %d\n", i * 100);
            fflush(log_file);
        }

        pthread_cond_signal(&q.not_empty);
        pthread_mutex_unlock(&q.mutex);
        usleep(100000);
    }
    if (log_file) fclose(log_file);
    return NULL;
}

void* consumer(void* arg) {
    for (int i = 1; i <= 5; i++) {
        pthread_mutex_lock(&q.mutex);
        while (q.count == 0) {
            pthread_cond_wait(&q.not_empty, &q.mutex);
        }
        int item = q.buffer[q.head];
        q.head = (q.head + 1) % QUEUE_SIZE;
        q.count--;

        printf("[Consumer] Processed & Logged Event: %d\n", item);

        pthread_cond_signal(&q.not_full);
        pthread_mutex_unlock(&q.mutex);
        usleep(150000);
    }
    return NULL;
}

void run_producer_consumer() {
    pthread_mutex_init(&q.mutex, NULL);
    pthread_cond_init(&q.not_full, NULL);
    pthread_cond_init(&q.not_empty, NULL);

    pthread_t prod, cons;
    printf("\n=== Starting Producer-Consumer Concurrent Logger ===\n");
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    pthread_mutex_destroy(&q.mutex);
    pthread_cond_destroy(&q.not_full);
    pthread_cond_destroy(&q.not_empty);
    printf("Producer-Consumer threads terminated cleanly and logged successfully.\n\n");
}