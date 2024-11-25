// Your Name: Roberto Cervantes
// Username: cs017

#define _REENTRANT // Ensures thread-safe behavior with POSIX libraries
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 15 // Circular buffer size
#define EOF_MARKER '*' // Special character to signal EOF to the consumer

// Shared buffer and indices for circular buffer
char buffer[BUFFER_SIZE];
int in = 0, out = 0; // 'in' is the producer's index, 'out' is the consumer's index

// Semaphores for synchronization
sem_t sem_items;    // Counts the number of items in the buffer
sem_t sem_space;    // Tracks available space in the buffer
sem_t sem_mutex;    // Protects the critical section (access to the buffer)

// Producer thread function
void* producer(void* arg) {
    // Open the input file
    FILE* fp = fopen("mytest.dat", "r");
    if (!fp) { // Check if file opening failed
        perror("Failed to open file"); // Print error if file can't be opened
        pthread_exit(NULL); // Exit the thread if file opening fails
    }

    char newChar; // Variable to hold each character read from the file
    while (fscanf(fp, "%c", &newChar) != EOF) { // Read characters one by one
        // Wait for available space in the buffer
        sem_wait(&sem_space);

        // Lock access to the buffer
        sem_wait(&sem_mutex);

        // Place the character into the circular buffer
        buffer[in] = newChar;
        in = (in + 1) % BUFFER_SIZE; // Increment 'in' index circularly

        // Unlock the buffer and signal that an item has been added
        sem_post(&sem_mutex);
        sem_post(&sem_items);
    }

    // Place the EOF_MARKER in the buffer to signal the consumer
    sem_wait(&sem_space); // Wait for space in the buffer
    sem_wait(&sem_mutex); // Lock access to the buffer

    buffer[in] = EOF_MARKER; // Add the EOF_MARKER to the buffer
    in = (in + 1) % BUFFER_SIZE; // Increment 'in' index circularly

    sem_post(&sem_mutex); // Unlock the buffer
    sem_post(&sem_items); // Signal that an item has been added

    // Close the file
    fclose(fp);

    pthread_exit(NULL); // Exit the producer thread
}

// Consumer thread function
void* consumer(void* arg) {
    char readChar; // Variable to store each character read from the buffer

    do {
        // Wait for an item in the buffer
        sem_wait(&sem_items);

        // Lock access to the buffer
        sem_wait(&sem_mutex);

        // Read the character from the circular buffer
        readChar = buffer[out];
        out = (out + 1) % BUFFER_SIZE; // Increment 'out' index circularly

        // Unlock the buffer and signal available space
        sem_post(&sem_mutex);
        sem_post(&sem_space);

        // Check if the character is not the EOF_MARKER
        if (readChar != EOF_MARKER) {
            printf("%c", readChar); // Print the character
            fflush(stdout); // Flush the output buffer to display immediately

            sleep(1); // Simulate slower consumption by sleeping for 1 second
        }
    } while (readChar != EOF_MARKER); // Continue until EOF_MARKER is encountered

    pthread_exit(NULL); // Exit the consumer thread
}

int main() {
    // Initialize semaphores
    sem_init(&sem_items, 0, 0);          // Start with 0 items in the buffer
    sem_init(&sem_space, 0, BUFFER_SIZE); // Buffer starts with full space available
    sem_init(&sem_mutex, 0, 1);          // Mutex starts unlocked (value of 1)

    // Thread declarations
    pthread_t producer_thread, consumer_thread;
    pthread_attr_t attr; // Thread attributes

    pthread_attr_init(&attr); // Initialize thread attributes
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM); // Set thread scope

    // Create producer and consumer threads
    pthread_create(&producer_thread, &attr, producer, NULL);
    pthread_create(&consumer_thread, &attr, consumer, NULL);

    // Wait for both threads to complete
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Destroy semaphores
    sem_destroy(&sem_items);
    sem_destroy(&sem_space);
    sem_destroy(&sem_mutex);

    // Print a completion message
    printf("\nProgram completed successfully.\n");
    return 0;
}
