#ifndef CCEDF_H
#define CCEDF_H

#include "rq.h"

typedef struct {
    int task_id;
    float total_actual;    // work units done (= WCET if no miss)
    float total_wallclock; // real elapsed time spent running
    float total_wcet;
    float total_slack;     // sum of (deadline - completion_time)
    int jobs_completed;
} TaskStats;
float compute_utilization(ReadyQueue *rq);
int select_task(ReadyQueue *rq);
float select_frequency(float U);

#endif