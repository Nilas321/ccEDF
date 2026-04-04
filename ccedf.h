#ifndef CCEDF_H
#define CCEDF_H

#include "rq.h"

float compute_utilization(ReadyQueue *rq);
int select_task(ReadyQueue *rq);
float select_frequency(float U);

#endif