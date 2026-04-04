#ifndef EQ_H
#define EQ_H

typedef enum {
    TASK_RELEASE,
    TASK_COMPLETE
} EventType;

typedef struct {
    float time;
    EventType type;
    int task_id;
} Event;

typedef struct {
    Event events[100];
    int size;
} EventQueue;

void eq_init(EventQueue *eq);

// 🔥 Must maintain ascending time order
// 🔥 For same time: TASK_COMPLETE must come before TASK_RELEASE
void eq_push(EventQueue *eq, Event e);

Event eq_pop(EventQueue *eq);

#endif