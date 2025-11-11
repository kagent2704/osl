#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t mutex, wrt;       // Semaphores
int sharedvar = 99;     // Shared variable
int readcount = 0;      // Number of readers currently reading

// Reader thread function
void* reader(void* arg) {
    int id = *((int*)arg);
    free(arg);  // Free the allocated memory for ID

    // Entry section
    sem_wait(&mutex);
    readcount++;
    if (readcount == 1)        // First reader locks writers
        sem_wait(&wrt);
    sem_post(&mutex);

    // Critical section (reading)
    printf("Reader %d is reading sharedvar = %d\n", id, sharedvar);
    sleep(1); // Simulate reading
    printf("Reader %d finished reading\n", id);

    // Exit section
    sem_wait(&mutex);
    readcount--;
    if (readcount == 0)        // Last reader unlocks writers
        sem_post(&wrt);
    sem_post(&mutex);

    pthread_exit(NULL);
}

// Writer thread function
void* writer(void* arg) {
    int id = *((int*)arg);
    free(arg);

    // Entry section
    printf("Writer %d is trying to enter CS\n", id);
    sem_wait(&wrt);

    // Critical section (writing)
    printf("Writer %d has entered CS\n", id);
    sharedvar++;
    printf("Writer %d changed sharedvar to %d\n", id, sharedvar);
    sleep(2); // Simulate writing
    printf("Writer %d is leaving CS\n", id);

    sem_post(&wrt);

    pthread_exit(NULL);
}

int main() {
    int n_readers, n_writers;
    printf("Enter number of readers and writers: ");
    scanf("%d %d", &n_readers, &n_writers);

    pthread_t rtid[n_readers], wtid[n_writers];

    // Initialize semaphores
    sem_init(&mutex, 0, 1);
    sem_init(&wrt, 0, 1);

    // Create writer threads
    for (int i = 0; i < n_writers; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&wtid[i], NULL, writer, id);
    }

    // Create reader threads
    for (int i = 0; i < n_readers; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&rtid[i], NULL, reader, id);
    }

    // Wait for writers to finish
    for (int i = 0; i < n_writers; i++)
        pthread_join(wtid[i], NULL);

    // Wait for readers to finish
    for (int i = 0; i < n_readers; i++)
        pthread_join(rtid[i], NULL);

    // Destroy semaphores
    sem_destroy(&mutex);
    sem_destroy(&wrt);

    return 0;
}
