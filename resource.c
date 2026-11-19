#include <stdio.h>
#include <sys/resource.h>
#include <unistd.h>
#include "campus_compute.h"

void run_resource_limit_demo() {
    printf("\n=== Resource Limit Demonstration (setrlimit) ===\n");
    struct rlimit old_lim, new_lim;

    // Get current CPU time limit
    if (getrlimit(RLIMIT_CPU, &old_lim) == 0) {
        printf("Current CPU limit: soft = %ld, hard = %ld\n", (long)old_lim.rlim_cur, (long)old_lim.rlim_max);
    }

    // Set new temporary CPU limit
    new_lim.rlim_cur = 10; // 10 seconds
    new_lim.rlim_max = 20; // 20 seconds
    if (setrlimit(RLIMIT_CPU, &new_lim) == 0) {
        printf("Successfully applied new CPU time limit (Soft: 10s, Hard: 20s).\n");
    } else {
        perror("setrlimit failed");
    }

    if (getrlimit(RLIMIT_CPU, &old_lim) == 0) {
        printf("Verified updated CPU limit: soft = %ld, hard = %ld\n\n", (long)old_lim.rlim_cur, (long)old_lim.rlim_max);
    }
}