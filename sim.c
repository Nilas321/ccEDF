#include <stdio.h>
#include "rq.h"
#include "eq.h"
#include "ccedf.h"

void generate_releases(EventQueue *eq, Task base[], int n, float H) {
    for (int i = 0; i < n; i++) {
        float t = 0;

        while (t < H) {
            Event e;
            e.time = t;
            e.type = TASK_RELEASE;
            e.task_id = base[i].id;

            eq_push(eq, e);
            t += base[i].period;
        }
    }
}

int pop_events_at_time(EventQueue *eq, Event batch[], float time) {
    int count = 0;

    while (eq->size > 0 && eq->events[0].time == time) {
        batch[count++] = eq_pop(eq);
    }

    return count;
}

// 🔹 Find next strictly future time
float get_next_time(EventQueue *eq, float current_time) {
    for (int i = 0; i < eq->size; i++) {
        if (eq->events[i].time > current_time)
            return eq->events[i].time;
    }
    return -1;
}

void run_simulation(Task base[], int n, float H) {

    EventQueue eq;
    ReadyQueue rq;

    eq_init(&eq);
    rq_init(&rq);

    generate_releases(&eq, base, n, H);

    float current_time = 0;

    TaskStats stats[n];
    for (int i = 0; i < n; i++) {
    stats[i].task_id = base[i].id;
    stats[i].total_actual = 0;
    stats[i].total_wcet = 0;
    stats[i].total_slack    = 0;      // ← missing
    stats[i].jobs_completed = 0;
    }

    while (eq.size > 0 && current_time <= H) {

        current_time = eq.events[0].time;

        printf("\n=============================\n");
        printf("TIME = %.2f\n", current_time);

        Event batch[50];
        int count = pop_events_at_time(&eq, batch, current_time);

        for (int i = 0; i < count; i++) {
            if (batch[i].type == TASK_RELEASE) {
                rq_add_job(&rq, base[batch[i].task_id - 1], current_time);
            }
        }

        // 🔹 Inner loop: keep making decisions at current_time
        //    until rq is empty or a future release pulls us forward
         while (1) {

            // Pop any releases that have come due at current_time
            Event batch2[50];
            int count2 = pop_events_at_time(&eq, batch2, current_time);
            for (int i = 0; i < count2; i++) {
                if (batch2[i].type == TASK_RELEASE) {
                    rq_add_job(&rq, base[batch2[i].task_id - 1], current_time);
                }
            }

            float U = compute_utilization(&rq);
            float f = select_frequency(U);
            int idx = select_task(&rq);

            printf("\nUtilization = %.2f | Frequency = %.2f\n", U, f);
            printf("\n=============================\n");

            if (idx == -1) {
                printf("CPU IDLE\n");
                break;
            }

            Task *t = &rq.tasks[idx];

            printf("Selected Task %d.%d (deadline=%.2f)\n",
                   t->id, t->job_id, t->deadline);

            float next_time = get_next_time(&eq, current_time);
            float completion_time = current_time + (t->remaining / f);

            float stop_time;
            if (next_time < 0 || completion_time <= next_time) {
                stop_time = completion_time;
            } else {
                stop_time = next_time;
            }

            float delta = stop_time - current_time;

            if (delta <= 0) {
                printf(" Zero time progress, skipping\n");
                break;
            }

            printf("Running T%d.%d from %.2f → %.2f\n",
                   t->id, t->job_id, current_time, stop_time);

            float work_done = f * delta;
            t->actual += work_done;
            t->wallclock_accumulated += delta;
            current_time = stop_time;


            if (t->remaining <= work_done + 1e-6) {
                t->remaining = 0;
                printf("→ Task %d.%d completes at %.2f (actual=%.2f)\n",
                       t->id, t->job_id, current_time, t->actual);
                
                stats[t->id - 1].total_actual    += t->actual;
                stats[t->id - 1].total_wcet      += base[t->id - 1].wcet;
                stats[t->id - 1].total_wallclock += t->wallclock_accumulated;
                stats[t->id - 1].total_slack     += (t->deadline - current_time);
                stats[t->id - 1].jobs_completed++;

                       
                rq_complete_job(&rq, t->id);



                // stay in inner loop, pop new releases and re-decide
            } else {
                t->remaining -= work_done;
                printf("→ Task %d.%d preempted, remaining=%.2f\n",
                       t->id, t->job_id, t->remaining);
                
                break;
            }
        }
    }

   printf("\n╔══════════════════════════════════════════════════════════════════════╗\n");
    printf(  "║                       SIMULATION DASHBOARD                          ║\n");
    printf(  "╚══════════════════════════════════════════════════════════════════════╝\n");
    printf("%-6s %-6s %-8s %-10s %-12s %-10s %-10s\n",
           "Task", "Jobs", "WCET", "Work Done", "Wall-Clock", "Overhead", "Avg Slack");
    printf("──────────────────────────────────────────────────────────────────────\n");

    float total_actual    = 0;
    float total_wallclock = 0;
    float total_wcet      = 0;
    float total_slack     = 0;

    for (int i = 0; i < n; i++) {
        float overhead  = stats[i].total_wallclock - stats[i].total_actual;
        float avg_slack = stats[i].jobs_completed > 0
                        ? stats[i].total_slack / stats[i].jobs_completed : 0;

        printf("T%-5d %-6d %-8.2f %-10.2f %-12.2f %-10.2f %-10.2f\n",
               stats[i].task_id,
               stats[i].jobs_completed,
               stats[i].total_wcet,
               stats[i].total_actual,
               stats[i].total_wallclock,
               overhead,
               avg_slack);

        total_actual    += stats[i].total_actual;
        total_wallclock += stats[i].total_wallclock;
        total_wcet      += stats[i].total_wcet;
        total_slack     += stats[i].total_slack;
    }

    float total_overhead = total_wallclock - total_actual;
    printf("──────────────────────────────────────────────────────────────────────\n");
    printf("%-6s %-6s %-8.2f %-10.2f %-12.2f %-10.2f\n",
           "TOTAL", "",
           total_wcet, total_actual, total_wallclock, total_overhead);

    printf("\nWork Utilisation  (WCET / H)       : %.2f%%\n", (total_actual    / H) * 100.0f);
    printf("Wall-Clock Usage  (wallclock / H)  : %.2f%%\n", (total_wallclock / H) * 100.0f);
    printf("Freq Overhead     (overhead / WCET): %.2f%%\n",
           (total_actual > 0) ? (total_overhead / total_actual) * 100.0f : 0);
    printf("Idle Time         (H - wallclock)  : %.2f units\n", H - total_wallclock);
    printf("══════════════════════════════════════════════════════════════════════\n");
}