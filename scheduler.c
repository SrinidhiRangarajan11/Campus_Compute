#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "campus_compute.h"

int input_jobs(Job jobs[], int max_jobs) {
    int n = 0;
    do {
        printf("Enter number of jobs (1 to %d): ", max_jobs);
        if (scanf("%d", &n) != 1) {
            while (getchar() != '\n');
            n = 0;
        }
    } while (n < 1 || n > max_jobs);

    for (int i = 0; i < n; i++) {
        jobs[i].job_id = i + 1;
        printf("\n--- Job J%d ---\n", jobs[i].job_id);
        printf("Arrival Time: ");
        scanf("%d", &jobs[i].arrival_time);
        printf("CPU Burst Time: ");
        scanf("%d", &jobs[i].cpu_burst);
        printf("Priority (lower number = higher priority): ");
        scanf("%d", &jobs[i].priority);

        jobs[i].remaining_time = jobs[i].cpu_burst;
        jobs[i].completion_time = 0;
        jobs[i].turnaround_time = 0;
        jobs[i].waiting_time = 0;
        jobs[i].status = 0;
    }
    return n;
}

int input_quantum() {
    int q = 0;
    do {
        printf("Enter Time Quantum (> 0): ");
        if (scanf("%d", &q) != 1) {
            while (getchar() != '\n');
            q = 0;
        }
    } while (q <= 0);
    return q;
}

// FCFS Scheduling
void run_fcfs(Job jobs[], int n) {
    Job sorted[MAX_JOBS];
    memcpy(sorted, jobs, sizeof(Job) * n);

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (sorted[j].arrival_time > sorted[j + 1].arrival_time ||
                (sorted[j].arrival_time == sorted[j + 1].arrival_time && sorted[j].job_id > sorted[j + 1].job_id)) {
                Job temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }

    int current_time = 0;
    float total_wt = 0, total_tat = 0;

    printf("\n--- FCFS Scheduling Execution Order ---\n");
    printf("Job ID\tArrival\tBurst\tCompletion\tWaiting\tTurnaround\n");

    for (int i = 0; i < n; i++) {
        if (current_time < sorted[i].arrival_time) {
            current_time = sorted[i].arrival_time;
        }
        sorted[i].waiting_time = current_time - sorted[i].arrival_time;
        sorted[i].completion_time = current_time + sorted[i].cpu_burst;
        sorted[i].turnaround_time = sorted[i].completion_time - sorted[i].arrival_time;
        current_time = sorted[i].completion_time;

        total_wt += sorted[i].waiting_time;
        total_tat += sorted[i].turnaround_time;

        printf("J%-5d\t%-7d\t%-5d\t%-10d\t%-7d\t%-10d\n",
               sorted[i].job_id, sorted[i].arrival_time, sorted[i].cpu_burst,
               sorted[i].completion_time, sorted[i].waiting_time, sorted[i].turnaround_time);
    }
    printf("\nAverage Waiting Time: %.2f", total_wt / n);
    printf("\nAverage Turnaround Time: %.2f\n\n", total_tat / n);
}

// Round Robin Scheduling
void run_round_robin(Job jobs[], int n, int quantum) {
    int current_time = 0, completed = 0;
    float total_wt = 0, total_tat = 0;
    int rem_burst[MAX_JOBS];
    int comp_time[MAX_JOBS] = {0};
    int wait_time[MAX_JOBS] = {0};
    int tat_time[MAX_JOBS] = {0};

    for (int i = 0; i < n; i++) {
        rem_burst[i] = jobs[i].cpu_burst;
    }

    printf("\n--- Round Robin Scheduling (Quantum = %d) ---\n", quantum);
    printf("Execution Timeline: ");

    while (completed < n) {
        int done = 1;
        for (int i = 0; i < n; i++) {
            if (rem_burst[i] > 0 && jobs[i].arrival_time <= current_time) {
                done = 0;
                if (rem_burst[i] > quantum) {
                    current_time += quantum;
                    rem_burst[i] -= quantum;
                    printf("[J%d] ", jobs[i].job_id);
                } else {
                    current_time += rem_burst[i];
                    wait_time[i] = current_time - jobs[i].arrival_time - jobs[i].cpu_burst;
                    if (wait_time[i] < 0) wait_time[i] = 0;
                    tat_time[i] = current_time - jobs[i].arrival_time;
                    comp_time[i] = current_time;
                    total_wt += wait_time[i];
                    total_tat += tat_time[i];
                    rem_burst[i] = 0;
                    completed++;
                    printf("[J%d(Done)] ", jobs[i].job_id);
                }
            }
        }
        if (done) {
            current_time++;
        }
    }

    printf("\n\nJob ID\tCompletion\tWaiting\tTurnaround\n");
    for (int i = 0; i < n; i++) {
        printf("J%-5d\t%-10d\t%-7d\t%-10d\n",
               jobs[i].job_id, comp_time[i], wait_time[i], tat_time[i]);
    }
    printf("\nAverage Waiting Time: %.2f", total_wt / n);
    printf("\nAverage Turnaround Time: %.2f\n\n", total_tat / n);
}

// Non-Preemptive Priority Scheduling
void run_priority_scheduling(Job jobs[], int n) {
    int completed = 0, current_time = 0;
    int is_completed[MAX_JOBS] = {0};
    float total_wt = 0, total_tat = 0;

    printf("\n--- Non-Preemptive Priority Scheduling (Tie-breaker: Arrival Time, then Job ID) ---\n");
    printf("Job ID\tPriority\tArrival\tBurst\tCompletion\tWaiting\tTurnaround\n");

    while (completed < n) {
        int idx = -1;
        int highest_priority = 999999;

        for (int i = 0; i < n; i++) {
            if (jobs[i].arrival_time <= current_time && !is_completed[i]) {
                if (jobs[i].priority < highest_priority) {
                    highest_priority = jobs[i].priority;
                    idx = i;
                } else if (jobs[i].priority == highest_priority) {
                    if (jobs[i].arrival_time < jobs[idx].arrival_time) {
                        idx = i;
                    } else if (jobs[i].arrival_time == jobs[idx].arrival_time && jobs[i].job_id < jobs[idx].job_id) {
                        idx = i;
                    }
                }
            }
        }

        if (idx != -1) {
            if (current_time < jobs[idx].arrival_time) {
                current_time = jobs[idx].arrival_time;
            }
            current_time += jobs[idx].cpu_burst;
            int ct = current_time;
            int tat = ct - jobs[idx].arrival_time;
            int wt = tat - jobs[idx].cpu_burst;
            is_completed[idx] = 1;
            completed++;

            total_wt += wt;
            total_tat += tat;

            printf("J%-5d\t%-8d\t%-7d\t%-5d\t%-10d\t%-7d\t%-10d\n",
                   jobs[idx].job_id, jobs[idx].priority, jobs[idx].arrival_time,
                   jobs[idx].cpu_burst, ct, wt, tat);
        } else {
            current_time++;
        }
    }
    printf("\nAverage Waiting Time: %.2f", total_wt / n);
    printf("\nAverage Turnaround Time: %.2f\n\n", total_tat / n);
}

void run_scheduling_comparison(Job jobs[], int n, int quantum) {
    printf("\n=== Running Scheduling Comparison on %d Jobs (Quantum = %d) ===\n", n, quantum);
    run_fcfs(jobs, n);
    run_round_robin(jobs, n, quantum);
    run_priority_scheduling(jobs, n);
}