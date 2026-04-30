#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 5

typedef struct {
    int buffer[BUFFER_SIZE];
    int in;
    int out;
    int count;
    pthread_mutex_t mutex;
    sem_t empty;   // Signals when buffer has empty slots
    sem_t full;    // Signals when buffer has items
} Buffer;

Buffer shared_buffer;

// Producer thread function
void* producer(void* arg) {
    int producer_id = *(int*)arg;
    
    for (int i = 0; i < 3; i++) {
        int item = producer_id * 10 + i;
        
        // Wait until buffer has empty slots
        sem_wait(&shared_buffer.empty);
        
        // Lock the buffer
        pthread_mutex_lock(&shared_buffer.mutex);
        
        // Add item to buffer
        shared_buffer.buffer[shared_buffer.in] = item;
        printf("Producer %d produced item %d at index %d\n", 
               producer_id, item, shared_buffer.in);
        
        shared_buffer.in = (shared_buffer.in + 1) % BUFFER_SIZE;
        shared_buffer.count++;
        
        // Unlock the buffer
        pthread_mutex_unlock(&shared_buffer.mutex);
        
        // Signal that buffer has items
        sem_post(&shared_buffer.full);
        
        sleep(1);  // Simulate production time
    }
    
    printf("Producer %d finished\n", producer_id);
    pthread_exit(NULL);
}

// Consumer thread function
void* consumer(void* arg) {
    int consumer_id = *(int*)arg;
    
    for (int i = 0; i < 3; i++) {
        // Wait until buffer has items
        sem_wait(&shared_buffer.full);
        
        // Lock the buffer
        pthread_mutex_lock(&shared_buffer.mutex);
        
        // Remove item from buffer
        int item = shared_buffer.buffer[shared_buffer.out];
        printf("Consumer %d consumed item %d from index %d\n", 
               consumer_id, item, shared_buffer.out);
        
        shared_buffer.out = (shared_buffer.out + 1) % BUFFER_SIZE;
        shared_buffer.count--;
        
        // Unlock the buffer
        pthread_mutex_unlock(&shared_buffer.mutex);
        
        // Signal that buffer has empty slots
        sem_post(&shared_buffer.empty);
        
        sleep(2);  // Simulate consumption time
    }
    
    printf("Consumer %d finished\n", consumer_id);
    pthread_exit(NULL);
}

int main() {
    printf("=== Producer-Consumer Problem ===\n");
    printf("Buffer Size: %d\n\n", BUFFER_SIZE);
    
    // Initialize buffer
    shared_buffer.in = 0;
    shared_buffer.out = 0;
    shared_buffer.count = 0;
    
    // Initialize synchronization primitives
    pthread_mutex_init(&shared_buffer.mutex, NULL);
    sem_init(&shared_buffer.empty, 0, BUFFER_SIZE);  // Buffer is initially empty
    sem_init(&shared_buffer.full, 0, 0);              // No items initially
    
    // Create threads
    pthread_t producers[2], consumers[2];
    int prod_ids[2] = {1, 2};
    int cons_ids[2] = {1, 2};
    
    // Create 2 producers
    for (int i = 0; i < 2; i++) {
        pthread_create(&producers[i], NULL, producer, &prod_ids[i]);
    }
    
    // Create 2 consumers
    for (int i = 0; i < 2; i++) {
        pthread_create(&consumers[i], NULL, consumer, &cons_ids[i]);
    }
    
    // Wait for all threads to complete
    for (int i = 0; i < 2; i++) {
        pthread_join(producers[i], NULL);
        pthread_join(consumers[i], NULL);
    }
    
    printf("\n=== All threads finished ===\n");
    printf("Final buffer count: %d\n", shared_buffer.count);
    
    // Cleanup
    pthread_mutex_destroy(&shared_buffer.mutex);
    sem_destroy(&shared_buffer.empty);
    sem_destroy(&shared_buffer.full);
    
    return 0;
}
