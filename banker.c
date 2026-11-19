#include <stdio.h>
#include <stdlib.h>
#include "campus_compute.h"

static int check_safe_state(int n, int m, int allocation[][MAX_RES], int need[][MAX_RES], int available[], int safe_seq[]) {
    int visited[MAX_PROCS] = {0};
    int work[MAX_RES];
    for (int j = 0; j < m; j++) work[j] = available[j];

    int count = 0;
    while (count < n) {
        int found = 0;
        for (int p = 0; p < n; p++) {
            if (!visited[p]) {
                int j;
                for (j = 0; j < m; j++) {
                    if (need[p][j] > work[j]) break;
                }
                if (j == m) {
                    for (int k = 0; k < m; k++) {
                        work[k] += allocation[p][k];
                    }
                    safe_seq[count++] = p;
                    visited[p] = 1;
                    found = 1;
                }
            }
        }
        if (!found) break;
    }
    return (count == n);
}

void run_banker_algorithm() {
    printf("\n=== Running Banker's Resource-Safety Algorithm ===\n");
    int n = 0, m = 0;

    do {
        printf("Enter number of processes (1 to %d): ", MAX_PROCS);
        if (scanf("%d", &n) != 1) {
            while (getchar() != '\n');
            n = 0;
        }
    } while (n < 1 || n > MAX_PROCS);

    do {
        printf("Enter number of resource types (1 to %d): ", MAX_RES);
        if (scanf("%d", &m) != 1) {
            while (getchar() != '\n');
            m = 0;
        }
    } while (m < 1 || m > MAX_RES);

    int allocation[MAX_PROCS][MAX_RES];
    int maximum[MAX_PROCS][MAX_RES];
    int need[MAX_PROCS][MAX_RES];
    int available[MAX_RES];

    printf("\nEnter Allocation Matrix (%d x %d):\n", n, m);
    for (int i = 0; i < n; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < m; j++) {
            scanf("%d", &allocation[i][j]);
        }
    }

    printf("\nEnter Maximum Demand Matrix (%d x %d):\n", n, m);
    for (int i = 0; i < n; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < m; j++) {
            scanf("%d", &maximum[i][j]);
            need[i][j] = maximum[i][j] - allocation[i][j];
            if (need[i][j] < 0) need[i][j] = 0;
        }
    }

    printf("\nEnter Available Resources (%d values): ", m);
    for (int j = 0; j < m; j++) {
        scanf("%d", &available[j]);
    }

    int safe_seq[MAX_PROCS];
    if (check_safe_state(n, m, allocation, need, available, safe_seq)) {
        printf("\nSystem is in a SAFE state.\nSafe Sequence: ");
        for (int i = 0; i < n; i++) {
            printf("P%d%s", safe_seq[i], (i == n - 1) ? "" : " -> ");
        }
        printf("\n\n");
    } else {
        printf("\nSystem is in an UNSAFE state! No safe execution sequence exists.\n\n");
    }

    char choice;
    printf("Test an artificially reduced Available vector? (y/n): ");
    scanf(" %c", &choice);
    if (choice == 'y' || choice == 'Y') {
        int test_avail[MAX_RES];
        printf("Enter reduced Available Resources (%d values): ", m);
        for (int j = 0; j < m; j++) {
            scanf("%d", &test_avail[j]);
        }
        if (check_safe_state(n, m, allocation, need, test_avail, safe_seq)) {
            printf("[Result] System remained SAFE.\nSafe Sequence: ");
            for (int i = 0; i < n; i++) {
                printf("P%d%s", safe_seq[i], (i == n - 1) ? "" : " -> ");
            }
            printf("\n\n");
        } else {
            printf("[Result] Unsafe State Verified: No safe execution sequence exists under these resource constraints.\n\n");
        }
    }
}