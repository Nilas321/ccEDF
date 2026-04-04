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

void rq_init(ReadyQueue *rq);

// Add new job (only if no active job exists)
void rq_add_job(ReadyQueue *rq, Task base, float current_time);

// Complete job
void rq_complete_job(ReadyQueue *rq, int task_id);

// Find active job of a task
int rq_find_active_job(ReadyQueue *rq, int task_id);

#endif