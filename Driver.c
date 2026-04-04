#include <stdio.h>
#include "sim.h"

// Helper to print a test header
static void print_test(int num, const char *desc, float H) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════╗\n");
    printf("║  TEST %-3d: %-42s║\n", num, desc);
    printf("║  Hyperperiod: %-38.2f ║\n", H); 
    printf("╚══════════════════════════════════════════════════════╝\n");
}

int main() {

    // ─── TEST 1: Original set, U=0.90 ────────────────────────────
    print_test(1, "T1(4,1) T2(5,2) T3(20,5) | U=0.90", 20.0f);
    {
        Task base[] = {
            {.id=1, .period=4,  .wcet=1, .remaining=1},
            {.id=2, .period=5,  .wcet=2, .remaining=2},
            {.id=3, .period=20, .wcet=5, .remaining=5},
        };
        run_simulation(base, 3, 20.0f);
    }

    // ─── TEST 2: Light load, U=0.58 ──────────────────────────────
    print_test(2, "T1(4,1) T2(8,2) T3(20,3) | U=0.58", 40.0f);
    {
        Task base[] = {
            {.id=1, .period=4,  .wcet=1, .remaining=1},
            {.id=2, .period=8,  .wcet=2, .remaining=2},
            {.id=3, .period=20, .wcet=3, .remaining=3},
        };
        run_simulation(base, 3, 40.0f);
    }

    // ─── TEST 3: Heavy load, U=1.00 (boundary) ───────────────────
    print_test(3, "T1(2,1) T2(4,1) T3(8,2) | U=1.00", 8.0f);
    {
        Task base[] = {
            {.id=1, .period=2, .wcet=1, .remaining=1},
            {.id=2, .period=4, .wcet=1, .remaining=1},
            {.id=3, .period=8, .wcet=2, .remaining=2},
        };
        run_simulation(base, 3, 8.0f);
    }

    // ─── TEST 4: Single task ──────────────────────────────────────
    print_test(4, "T1(5,2) only | U=0.40", 10.0f);
    {
        Task base[] = {
            {.id=1, .period=5, .wcet=2, .remaining=2},
        };
        run_simulation(base, 1, 10.0f);
    }

    // ─── TEST 5: Two tasks, tight deadlines ───────────────────────
    print_test(5, "T1(3,1) T2(6,2) | U=0.67", 6.0f);
    {
        Task base[] = {
            {.id=1, .period=3, .wcet=1, .remaining=1},
            {.id=2, .period=6, .wcet=2, .remaining=2},
        };
        run_simulation(base, 2, 6.0f);
    }

    // ─── TEST 6: Overloaded, U > 1.0 (expect deadline misses) ────
    print_test(6, "T1(2,1) T2(3,2) T3(5,2) | U=1.57", 30.0f);
    {
        Task base[] = {
            {.id=1, .period=2, .wcet=1, .remaining=1},
            {.id=2, .period=3, .wcet=2, .remaining=2},
            {.id=3, .period=5, .wcet=2, .remaining=2},
        };
        run_simulation(base, 3, 30.0f);
    }

    return 0;
}
