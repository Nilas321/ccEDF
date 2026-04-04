#include <stdio.h>
#include "ccedf.h"

#define EPS 1e-6

float compute_utilization(ReadyQueue *rq) {
    float U = 0;
    printf("\n\n");
    for (int i = 0; i < rq->size; i++) {
        Task *t = &rq->tasks[i];
        
        printf("Task %d.%d: actual=%.2f, wcet=%.2f, period=%.2f\n",
               t->id, t->job_id, t->actual, t->wcet, t->period);

        if (t->period <= 0) continue;

        if (t->completed){
            printf("  → Completed, contributes %.2f\n", t->actual / t->period);
            U += t->actual / t->period;
        }
        else{
            printf("  → In progress, contributes %.2f\n", t->wcet / t->period);
            U += t->wcet / t->period;
        }
    }
    return U;
}

int select_task(ReadyQueue *rq) {
    int idx = -1;
    float min_deadline = 1e9;

    for (int i = 0; i < rq->size; i++) {
        Task *t = &rq->tasks[i];

        if (!t->completed && t->remaining > EPS) {
            if (t->deadline < min_deadline) {
                min_deadline = t->deadline;
                idx = i;
            }
        }
    }

    return idx;
}

float select_frequency(float U) {
    float f_min = 0.2f, f_max = 1.0f , f1 = 0.90f;

    if (U <= 0.0f) return f_min;

    if (U > 1.0f) {
        printf("OVERLOAD: U = %.2f\n", U);
    }


    if (U < f_min) return f_min;
    if (f_min < U && U <= f1 ) return f1;
    if (U > f_max) return f_max;

    return f_max;
}