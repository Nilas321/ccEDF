#ifndef CCEDF_H
#define CCEDF_H

#include "rq.h"

typedef struct {
    int task_id;
    float total_actual;
    float total_wcet;
    int jobs_completed;
} TaskStats;

float compute_utilization(ReadyQueue *rq);
int select_task(ReadyQueue *rq);
float select_frequency(float U);

#endif