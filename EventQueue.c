#include "eq.h"

void eq_init(EventQueue *eq) {
    eq->size = 0;
}

void eq_push(EventQueue *eq, Event e) {
    int i = eq->size - 1;

    while (i >= 0) {

        if (eq->events[i].time > e.time) {
            eq->events[i + 1] = eq->events[i];
        }

        else if (eq->events[i].time == e.time) {

            if (eq->events[i].type == TASK_RELEASE &&
                e.type == TASK_COMPLETE) {

                eq->events[i + 1] = eq->events[i];
            } else {
                break;
            }
        }

        else {
            break;
        }

        i--;
    }

    eq->events[i + 1] = e;
    eq->size++;
}

Event eq_pop(EventQueue *eq) {
    Event e = eq->events[0];

    for (int i = 1; i < eq->size; i++) {
        eq->events[i - 1] = eq->events[i];
    }

    eq->size--;

    return e;
}