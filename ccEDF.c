#include <stdio.h>
#include "ccedf.h"

#define EPS 1e-6

float compute_utilization(ReadyQueue *rq) {
    float U = 0;
    for (int i = 0; i < rq->size; i++) {
        Task *t = &rq->tasks[i];
        
        if (t->period <= 0) continue;
        if (t->completed){
            
            U += t->actual / t->period;
        }
        else{
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
   
    float levels[] = {0.50, 0.60, 0.70, 0.80, 0.90, 1.00};
    int   n        = 6;

    for (int i = 0; i < n; i++) {
        if (levels[i] >= U - 1e-6f)
            return levels[i];
    }
    return 1.00f;  // cap at max if overloaded
}