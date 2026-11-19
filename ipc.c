#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/wait.h>
#include "campus_compute.h"

void run_ipc_demo() {
    printf("\n=== Starting POSIX Shared Memory & Named Semaphore IPC Demo ===\n");
    
    // Create shared memory object
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed");
        return;
    }
    ftruncate(shm_fd, 1024);

    void* ptr = mmap(0, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap failed");
        return;
    }

    // Create named semaphore
    sem_t* sem = sem_open(SEM_NAME, O_CREAT | O_RDWR, 0666, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open failed");
        return;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        return;
    } else if (pid == 0) {
        // Child Process
        sem_wait(sem);
        sprintf((char*)ptr, "Hello from Child Process via CampusCompute Shared Memory!");
        printf("[Child IPC] Wrote message to shared memory.\n");
        sem_post(sem);
        exit(0);
    } else {
        // Parent Process
        wait(NULL);
        sem_wait(sem);
        printf("[Parent IPC] Read from shared memory: \"%s\"\n", (char*)ptr);
        sem_post(sem);

        // Cleanup
        munmap(ptr, 1024);
        close(shm_fd);
        shm_unlink(SHM_NAME);
        sem_close(sem);
        sem_unlink(SEM_NAME);
        printf("IPC Resources cleaned up successfully.\n\n");
    }
}