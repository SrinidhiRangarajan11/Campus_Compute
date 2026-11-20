# CampusCompute: Intelligent Job Scheduling and Resource Management

**CampusCompute** is an interactive, modular Operating Systems simulator built in C for Linux/Ubuntu. It demonstrates core OS concepts—including dynamic CPU scheduling, multi threaded producer consumer synchronization, POSIX inter process communication (IPC), Banker's deadlock avoidance, process fault detection, and kernel level CPU resource governance through a unified command line interface.

---

## Core Features & OS Primitives

| Module | File | OS Concepts & System Calls Used |
| :--- | :--- | :--- |
| **CLI Dispatcher** | `main.c` | Dynamic command routing and runtime user-input orchestration |
| **CPU Scheduler** | `scheduler.c` | Interactive **FCFS**, **Round Robin** (custom time quantum), **Non-Preemptive Priority**, and algorithm benchmarking (Waiting Time & Turnaround Time) |
| **Concurrent Logger** | `logger.c` | **POSIX Threads (`pthread`)**, **Mutexes (`pthread_mutex_t`)**, and **Condition Variables (`pthread_cond_t`)** implementing a bounded-buffer Producer-Consumer pattern writing to `campus_compute.log` |
| **Inter-Process Communication** | `ipc.c` | **POSIX Shared Memory (`shm_open`, `mmap`, `shm_unlink`)** and **Named Semaphores (`sem_open`, `sem_wait`, `sem_post`, `sem_unlink`)** for synchronized parent-child data exchange |
| **Deadlock Avoidance** | `banker.c` | Dynamic **Banker's Safety Algorithm** computing Need matrices, safe execution sequences, and simulated resource-exhaustion (unsafe state) verification |
| **Fault Tolerance** | `worker.c` | Process creation and exit-status monitoring via **`fork()`**, **`waitpid()`**, and **`WEXITSTATUS()`** to detect normal vs. abnormal worker termination |
| **Resource Governance** | `resource.c` | Kernel resource quota enforcement using **`getrlimit()`** and **`setrlimit(RLIMIT_CPU)`** for soft and hard CPU time bounds |

---

## Project Structure

```text
CampusCompute/
├── campus_compute.h   # Shared data structures, constants, and function prototypes
├── main.c             # CLI argument parser and interactive input entry point
├── scheduler.c        # Dynamic job input helpers, FCFS, Round Robin, Priority, and comparison logic
├── logger.c           # Multi-threaded Producer-Consumer event logger
├── ipc.c              # POSIX shared memory and named semaphore demonstration
├── banker.c           # Interactive Banker's Algorithm for safe/unsafe state detection
├── worker.c           # Parent-child worker failure and exit-code monitoring
├── resource.c         # CPU soft/hard resource limit demonstration
├── Makefile           # Build automation and shared-memory cleanup rules
└── README.md          # Project documentation
```

---

## Prerequisites

* **Operating System:** Linux (Ubuntu recommended) or Windows Subsystem for Linux (WSL Ubuntu)
* **Compiler:** `gcc` with POSIX threads (`-pthread`) and real-time library support
* **Build Tool:** GNU `make`

---

## Build & Cleanup Instructions

1. **Clean previous builds and compile the project:**
   ```bash
   make clean && make
   ```

2. **Remove compiled binaries, object files, logs, and `/dev/shm` artifacts:**
   ```bash
   make clean
   ```

---

## Usage & Available Commands

Run the executable with any of the following command flags:

```bash
./campus_compute <command>
```

| Command | Description | Interactive Input Required? |
| :--- | :--- | :--- |
| `fcfs` | Runs First-Come, First-Served scheduling | Yes (number of jobs, arrival, burst, priority) |
| `rr` | Runs Round Robin scheduling | Yes (job details + time quantum) |
| `priority` | Runs Non-Preemptive Priority scheduling | Yes (job details) |
| `compare` | Benchmarks FCFS, Round Robin, and Priority on the same job set | Yes (job details + time quantum) |
| `logger` | Runs the concurrent Producer-Consumer thread logger | No (writes events to `campus_compute.log`) |
| `ipc` | Runs the Shared Memory & Named Semaphore parent-child demo | No |
| `banker` | Runs Banker's Resource-Safety Algorithm | Yes (process/resource counts, matrices, available vector) |
| `worker` | Runs worker process creation and exit-code failure detection | No |
| `limit` | Demonstrates reading and updating `RLIMIT_CPU` soft/hard bounds | No |
| `all` | Executes every module sequentially in an integrated run | Yes (prompts for scheduling and Banker's inputs) |
| `help` | Displays the CLI help menu | No |

---

## Sample Interactive Walkthrough

### Running Round Robin Scheduling
```bash
./campus_compute rr
```
```text
Enter number of jobs (1 to 10): 3

--- Job J1 ---
Arrival Time: 0
CPU Burst Time: 5
Priority (lower number = higher priority): 2
...
Enter Time Quantum (> 0): 2
```

### Checking Generated Activity Logs
After running `./campus_compute logger` or `./campus_compute all`, inspect the persistent log file:
```bash
cat campus_compute.log
```
