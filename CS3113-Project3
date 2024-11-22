#define _REENTRANT
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 15 // Circular buffer size
#define FILE_NAME "mytest.dat" // Input file name

// Shared data
char buffer[BUFFER_SIZE];
int in = 0, out = 0; // Indices for producer and consumer

// Semaphores
sem_t empty; // Tracks empty slots
sem_t full;  // Tracks filled slots
pthread_mutex_t mutex; // Ensures mutual exclusion

// Function prototypes
void *producer(void *arg);
void *consumer(void *arg);

int main() {
    pthread_t producer_thread, consumer_thread;

    // Initialize semaphores and mutex
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    // Create threads
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Wait for threads to finish
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Clean up
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}

void *producer(void *arg) {
    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char ch;
    while ((ch = fgetc(file)) != EOF) {
        sem_wait(&empty);            // Wait for an empty slot
        pthread_mutex_lock(&mutex); // Enter critical section

        buffer[in] = ch; // Place character in buffer
        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex); // Exit critical section
        sem_post(&full);              // Signal a filled slot
    }

    // Signal end of production
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);

    buffer[in] = '*'; // Special character to indicate EOF
    in = (in + 1) % BUFFER_SIZE;

    pthread_mutex_unlock(&mutex);
    sem_post(&full);

    fclose(file);
    return NULL;
}

void *consumer(void *arg) {
    char ch;
    do {
        sem_wait(&full);             // Wait for a filled slot
        pthread_mutex_lock(&mutex); // Enter critical section

        ch = buffer[out]; // Consume character
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex); // Exit critical section
        sem_post(&empty);             // Signal an empty slot

        if (ch != '*') {
            putchar(ch); // Print the character
            fflush(stdout);
        }

        sleep(1); // Consumer runs slower
    } while (ch != '*');

    return NULL;
}
