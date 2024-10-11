CC = gcc
CFLAGS = -Wall -pthread -lm
SRC = main.c linked_list_mutex.c linked_list_serial.c linked_list.c linked_list_lock.c
OBJ = $(SRC:.c=.o)
EXEC = main

# Default target to build the executable
all: $(EXEC)

# Rule to compile the main program
$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $(OBJ) $(CFLAGS)

# Clean object files and executable
clean:
	rm -f $(OBJ) $(EXEC)

# Rule to run main with "mutex"
run_mutex: $(EXEC)
	./$(EXEC) mutex

# Rule to run main with "rwlock"
run_rwlock: $(EXEC)
	./$(EXEC) rwlock

# Rule to run main with "serial"
run_serial: $(EXEC)
	./$(EXEC) serial

# Run all cases in sequence
run_all_cases: $(EXEC)
	./$(EXEC) serial 0.99 0.005 0.005
	./$(EXEC) mutex 0.99 0.005 0.005
	./$(EXEC) rwlock 0.99 0.005 0.005
	./$(EXEC) serial 0.90 0.05 0.05
	./$(EXEC) mutex 0.90 0.05 0.05
	./$(EXEC) rwlock 0.90 0.05 0.05
	./$(EXEC) serial 0.50 0.25 0.25
	./$(EXEC) mutex 0.50 0.25 0.25
	./$(EXEC) rwlock 0.50 0.25 0.25

# Rule to compile individual object files
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)
