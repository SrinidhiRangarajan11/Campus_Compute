#ifndef CAMPUS_COMPUTE_H
#define CAMPUS_COMPUTE_H

#define MAX_JOBS 10
#define MAX_PROCS 10
#define MAX_RES 10
#define BUFFER_SIZE 5
#define SHM_NAME "/campus_compute_shm"
#define SEM_NAME "/campus_compute_sem"

// Job structure for FR1 & FR2
typedef struct {
    int job_id;
    int arrival_time;
    int cpu_burst;
    int priority;
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int status; // 0: Waiting, 1: Running, 2: Completed
} Job;

// Input helpers
int input_jobs(Job jobs[], int max_jobs);
int input_quantum();

// Function declarations
void run_job_management();
void run_fcfs(Job jobs[], int n);
void run_round_robin(Job jobs[], int n, int quantum);
void run_priority_scheduling(Job jobs[], int n);
void run_scheduling_comparison(Job jobs[], int n, int quantum);

void run_producer_consumer();
void run_ipc_demo();
void run_banker_algorithm();
void run_worker_failure_demo();
void run_resource_limit_demo();
void show_help();

#endif