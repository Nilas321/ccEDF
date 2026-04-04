CC = gcc
CFLAGS = -Wall -O2

TARGET = sim
DRIVER ?= main

all: $(TARGET)

$(TARGET): $(DRIVER).o sim.o ReadyQueue.o EventQueue.o ccEDF.o
	$(CC) $(CFLAGS) -o $(TARGET) $(DRIVER).o sim.o ReadyQueue.o EventQueue.o ccEDF.o

$(DRIVER).o: $(DRIVER).c sim.h rq.h
	$(CC) $(CFLAGS) -c $(DRIVER).c

sim.o: sim.c sim.h rq.h eq.h ccedf.h
	$(CC) $(CFLAGS) -c sim.c

ReadyQueue.o: ReadyQueue.c rq.h
	$(CC) $(CFLAGS) -c ReadyQueue.c

EventQueue.o: EventQueue.c eq.h
	$(CC) $(CFLAGS) -c EventQueue.c

ccEDF.o: ccEDF.c ccedf.h rq.h
	$(CC) $(CFLAGS) -c ccEDF.c

clean:
	rm -f *.o $(TARGET)