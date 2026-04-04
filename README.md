# ccEDF Scheduler Simulator

A discrete-event simulation of the **Consumption-Constrained Earliest Deadline First (ccEDF)** scheduling algorithm with **Dynamic Voltage and Frequency Scaling (DVFS)**.

---

## Overview

ccEDF extends the classic EDF real-time scheduler by dynamically adjusting CPU frequency based on current utilization. Running at a lower frequency saves energy at the cost of extra wall-clock time — this simulator models and measures that tradeoff across a full hyperperiod.

---

## Build & Run

### Compile
```bash
make
```

### Run default driver (`main.c`)
```bash
./sim
```

### Run a custom driver
```bash
make DRIVER=Driver
./sim
```

> The `DRIVER` variable sets which `.c` file contains `main()`. Omit the `.c` extension.

### Clean build artifacts
```bash
make clean
```

---

## Project Structure
```
.
├── main.c          # Default driver (basic task set)
├── Driver.c        # Test driver (6 test cases)
├── sim.c           # Core simulation loop + dashboard
├── sim.h           # Public interface for run_simulation()
├── rq.c            # Ready queue implementation
├── rq.h            # Task and ReadyQueue structs
├── eq.c            # Event queue implementation
├── eq.h            # Event structs
├── ccedf.c         # ccEDF policy: task selection, utilization, frequency
├── ccedf.h         # TaskStats struct, extern stats[], ccEDF interface
└── Makefile
```

---

## How It Works

### Task Model
Each task is periodic with implicit deadlines (deadline = period):
```
T(period, wcet) — e.g. T1(4, 1) means period=4, wcet=1
```

### Scheduling Policy
1. At each decision point, compute utilization `U = Σ(wcet / period)`
2. Select the lowest frequency `f` from the DVFS table such that `f >= U`
3. Run the task with the earliest deadline (EDF)
4. Wall-clock time for a job = `wcet / f`

### DVFS Frequency Levels
```
{ 0.50, 0.60, 0.70, 0.80, 0.90, 1.00 }
```

---

## Dashboard

After each simulation, a summary dashboard is printed:
```
Task   Released Done   WCET   Work Done  Wall-Clock  Overhead  Avg Slack  Misses
T1     5        5      5.00   5.00       5.56        0.56      2.84       0
T2     4        4      8.00   8.00       8.89        0.89      2.19       0
T3     1        1      5.00   5.00       5.56        0.56      0.00       0

Work Utilisation  (WCET / H)       : 90.00%
Wall-Clock Usage  (wallclock / H)  : 100.00%
Freq Overhead     (overhead / WCET): 11.11%
Idle Time         (H - wallclock)  : 0.00 units
```

### Column Definitions

| Column | Meaning |
|--------|---------|
| Released | Total jobs released within hyperperiod H |
| Done | Jobs that fully completed before their deadline |
| WCET | Total CPU budget consumed by completed jobs |
| Work Done | Actual CPU units done including partial/incomplete jobs |
| Wall-Clock | Real elapsed time including frequency overhead |
| Overhead | `Wall-Clock - Work Done` — penalty of running below f=1.0 |
| Avg Slack | Average `deadline - completion_time` for completed jobs |
| Misses | Jobs that did not complete before their deadline |

---

## Test Cases (Driver.c)

| Test | Task Set | U | f | Notes |
|------|----------|---|---|-------|
| 1 | T1(4,1) T2(5,2) T3(20,5) | 0.90 | 0.90 | Baseline |
| 2 | T1(4,1) T2(8,2) T3(20,3) | 0.58 | 0.60 | Light load |
| 3 | T1(2,1) T2(4,1) T3(8,2)  | 1.00 | 1.00 | Full utilization boundary |
| 4 | T1(5,2)                   | 0.40 | 0.50 | Single task |
| 5 | T1(3,1) T2(6,2)           | 0.67 | 0.70 | Tight deadlines |
| 6 | T1(2,1) T2(3,2) T3(5,2)  | 1.57 | 1.00 | Overloaded — deadline misses expected |

---

## Assumptions

- Implicit deadlines: `deadline = period` for all tasks
- Tasks are preemptible at any release point
- Frequency is selected once per decision point and held until the next event
- Simulation runs for exactly one hyperperiod `H = lcm(periods)`
- Jobs incomplete at `H` are flagged as deadline misses
