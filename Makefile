CC = gcc
CFLAGS = -Wall -O2

TARGET = sim

all: $(TARGET)

$(TARGET): main.o sim.o ReadyQueue.o EventQueue.o ccEDF.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o sim.o ReadyQueue.o EventQueue.o ccEDF.o

main.o: main.c sim.h rq.h
	$(CC) $(CFLAGS) -c main.c

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