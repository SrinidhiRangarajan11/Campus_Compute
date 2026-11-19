#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "campus_compute.h"

void run_worker_failure_demo() {
    printf("\n=== Worker Failure Detection Demo ===\n");

    // 1. Normal Completion
    pid_t pid1 = fork();
    if (pid1 == 0) {
        printf("[Worker Child 1] Running successfully...\n");
        exit(0);
    } else {
        int status;
        waitpid(pid1, &status, 0);
        if (WIFEXITED(status)) {
            printf("[Parent] Worker 1 terminated normally with exit code: %d\n", WEXITSTATUS(status));
        }
    }

    // 2. Simulated Failure
    pid_t pid2 = fork();
    if (pid2 == 0) {
        printf("[Worker Child 2] Encountered simulated resource exception, exiting abnormally...\n");
        exit(99); // Non-zero simulated error exit
    } else {
        int status;
        waitpid(pid2, &status, 0);
        if (WIFEXITED(status)) {
            printf("[Parent] Detected Worker 2 abnormal/simulated failure exit status: %d\n\n", WEXITSTATUS(status));
        }
    }
}