#include <stdio.h>
#include "sim.h"

int main() {

    //Task set: T1(4,1), T2(5,2), T3(20,5)
Task base[] = {
    {1, 0, 4,  1, 1, 0, 0, 0, 0},   // T1: id,job_id,period,wcet,remaining,actual,deadline,wallclock_accumulated,completed
    {2, 0, 5,  2, 2, 0, 0, 0, 0},   // T2
    {3, 0, 20, 5, 5, 0, 0, 0, 0}    // T3
};
    int n = 3;
    float H = 20;  // hyperperiod

    printf("Starting ccEDF Simulation\n");

    run_simulation(base, n, H);

    return 0;
}