#include <stdio.h>
#include "rq.h"

static int job_counter[50] = {0};

void rq_init(ReadyQueue *rq) {
    rq->size = 0;
}

int rq_find_task(ReadyQueue *rq, int task_id) {
    for (int i = 0; i < rq->size; i++) {
        if (rq->tasks[i].id == task_id) {
            return i;
        }
    }
    return -1;
}

void rq_add_job(ReadyQueue *rq, Task base, float current_time) {

    int idx = rq_find_task(rq, base.id);

    if (idx != -1) {

        Task *t = &rq->tasks[idx];

        if (!t->completed && t->remaining > 1e-4) {
            printf("DEADLINE MISS: Task %d | miss count now %d\n",
           base.id, stats[base.id - 1].deadline_misses + 1);
           
            stats[base.id - 1].deadline_misses++;
        if (t->actual > 1e-6f) {
        stats[base.id - 1].total_actual    += t->actual;
        stats[base.id - 1].total_wallclock += t->wallclock_accumulated;
        }
    }
        stats[base.id - 1].jobs_released++;

        job_counter[base.id]++;
        t->job_id = job_counter[base.id];

        t->remaining = base.wcet;
        t->completed = 0;
        t->deadline = current_time + base.period;
        t->actual = 0;
        t->wallclock_accumulated = 0;

        printf(" Reset T%d.%d (deadline=%.2f)\n",
               t->id, t->job_id, t->deadline);

        return;
    }
    stats[base.id - 1].jobs_released++;
    Task t = base;

    job_counter[base.id]++;
    t.job_id = job_counter[base.id];

    t.remaining = t.wcet;
    t.completed = 0;
    t.deadline = current_time + t.period;
    t.actual = 0;
    t.wallclock_accumulated = 0;  // ← was missing

    rq->tasks[rq->size++] = t;

    printf("Added T%d.%d (deadline=%.2f)\n",
           t.id, t.job_id, t.deadline);
}

void rq_complete_job(ReadyQueue *rq, int task_id) {

    int idx = rq_find_task(rq, task_id);

    if (idx == -1) return;

    Task *t = &rq->tasks[idx];

    if (!t->completed) {

        t->completed = 1;
        t->remaining = 0;

        printf("Completed T%d.%d (actual=%.2f)\n",
               t->id, t->job_id, t->actual);
    }
}