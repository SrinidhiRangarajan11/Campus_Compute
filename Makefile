CC = gcc
CFLAGS = -Wall -Wextra -pthread
TARGET = campus_compute
OBJS = main.o scheduler.o logger.o ipc.o banker.o worker.o resource.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c campus_compute.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) *.o campus_compute.log /dev/shm/campus_compute_shm /dev/shm/sem.campus_compute_sem

.PHONY: all clean