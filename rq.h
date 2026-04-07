#ifndef RQ_H
#define RQ_H

typedef struct {
    int id;
    int job_id; 

    float period;
    float wcet;
    float remaining;
    float actual;
    float deadline;
    float wallclock_accumulated;
    int completed;
} Task;

typedef struct {
    Task tasks[50];
    int size;
} ReadyQueue;


typedef struct {
    int task_id;
    float total_actual;    // work units done (= WCET if no miss)
    float total_wallclock; // real elapsed time spent running
    float total_wcet;
    float total_slack;     // sum of (deadline - completion_time)
    int jobs_completed;
    int deadline_misses;
    int jobs_released;
} TaskStats;

#define MAX_TASKS 50
extern TaskStats stats[MAX_TASKS];
extern int stats_n;

void rq_init(ReadyQueue *rq);

// Add new job (only if no active job exists)
void rq_add_job(ReadyQueue *rq, Task base, float current_time);

// Complete job
void rq_complete_job(ReadyQueue *rq, int task_id);

#endif