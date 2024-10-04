#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_STUDENTS 1000000 // Adjust as needed
#define NUM_THREADS 4

typedef struct {
    int *array;
    int start;
    int end;
    int search;
    int *count;
    pthread_mutex_t *mutex;
} ThreadData;

void *searchThread(void *arg) {
    ThreadData *threadData = (ThreadData *)arg;
    int *array = threadData->array;
    int start = threadData->start;
    int end = threadData->end;
    int search = threadData->search;
    int *count = threadData->count;
    pthread_mutex_t *mutex = threadData->mutex;

    int localCount = 0;
    for (int i = start; i < end; i++) {
        if (array[i] == search) {
            localCount++;
        }
    }

    pthread_mutex_lock(mutex);
    *count += localCount;
    pthread_mutex_unlock(mutex);

    return NULL;
}

int main() {
    int scores[MAX_STUDENTS];
    int search;
    int totalStudents;
    int totalThreads;
    int segmentSize;
    pthread_t threads[NUM_THREADS];
    ThreadData threadData[NUM_THREADS];
    pthread_mutex_t mutex;

    // Generate random scores
    printf("Enter the total number of students: ");
    scanf("%d", &totalStudents);
    for (int i = 0; i < totalStudents; i++) {
        scores[i] = rand() % 101; // Random score between 0 and 100
    }

    // Get search target
    printf("Enter the search mark: ");
    scanf("%d", &search);

    // Determine number of threads and segment size
    totalThreads = NUM_THREADS;
    segmentSize = totalStudents / totalThreads;

    // Initialize mutex
    pthread_mutex_init(&mutex, NULL);

    // Create threads and assign work
    for (int i = 0; i < totalThreads; i++) {
        threadData[i].array = scores;
        threadData[i].start = i * segmentSize;
        threadData[i].end = (i + 1) * segmentSize;
        threadData[i].search = search;
        threadData[i].count = &totalStudents;
        threadData[i].mutex = &mutex;
        pthread_create(&threads[i], NULL, searchThread, (void *)&threadData[i]);
    }

    // Join threads
    for (int i = 0; i < totalThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print result
    printf("Number of students who scored %d: %d\n", search, totalStudents);

    pthread_mutex_destroy(&mutex);

    return 0;
}
