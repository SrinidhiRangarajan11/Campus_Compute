#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "campus_compute.h"

void show_help() {
    printf("\n============================================================\n");
    printf("           CAMPUSCOMPUTE COMMAND-LINE INTERFACE             \n");
    printf("============================================================\n");
    printf("Usage: ./campus_compute [command]\n");
    printf("Available Commands / Menu Options:\n");
    printf("  fcfs          - Run First-Come, First-Served Scheduling\n");
    printf("  rr            - Run Round Robin Scheduling\n");
    printf("  priority      - Run Non-Preemptive Priority Scheduling\n");
    printf("  compare       - Run Scheduling Comparison\n");
    printf("  logger        - Run Producer-Consumer Thread Activity Logger\n");
    printf("  ipc           - Run Shared Memory & Semaphore IPC\n");
    printf("  banker        - Run Banker's Safety Algorithm\n");
    printf("  worker        - Run Worker Process Failure Detection Demo\n");
    printf("  limit         - Run Resource Limit (setrlimit) Demo\n");
    printf("  all           - Run Integrated Demonstration (All Features)\n");
    printf("  help          - Display this help message\n");
    printf("============================================================\n\n");
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        show_help();
        printf("Error: No command provided. Please enter a valid command.\n");
        return 1;
    }

    Job jobs[MAX_JOBS];
    int n = 0, quantum = 2;

    if (strcmp(argv[1], "fcfs") == 0) {
        n = input_jobs(jobs, MAX_JOBS);
        run_fcfs(jobs, n);
    } else if (strcmp(argv[1], "rr") == 0) {
        n = input_jobs(jobs, MAX_JOBS);
        quantum = input_quantum();
        run_round_robin(jobs, n, quantum);
    } else if (strcmp(argv[1], "priority") == 0) {
        n = input_jobs(jobs, MAX_JOBS);
        run_priority_scheduling(jobs, n);
    } else if (strcmp(argv[1], "compare") == 0) {
        n = input_jobs(jobs, MAX_JOBS);
        quantum = input_quantum();
        run_scheduling_comparison(jobs, n, quantum);
    } else if (strcmp(argv[1], "logger") == 0) {
        run_producer_consumer();
    } else if (strcmp(argv[1], "ipc") == 0) {
        run_ipc_demo();
    } else if (strcmp(argv[1], "banker") == 0) {
        run_banker_algorithm();
    } else if (strcmp(argv[1], "worker") == 0) {
        run_worker_failure_demo();
    } else if (strcmp(argv[1], "limit") == 0) {
        run_resource_limit_demo();
    } else if (strcmp(argv[1], "all") == 0) {
        n = input_jobs(jobs, MAX_JOBS);
        quantum = input_quantum();
        run_scheduling_comparison(jobs, n, quantum);
        run_producer_consumer();
        run_ipc_demo();
        run_banker_algorithm();
        run_worker_failure_demo();
        run_resource_limit_demo();
        printf("=== Integrated Execution Completed Successfully ===\n");
    } else if (strcmp(argv[1], "help") == 0) {
        show_help();
    } else {
        printf("Error: Unknown command '%s'. Use './campus_compute help' for valid options.\n", argv[1]);
        return 1;
    }

    return 0;
}