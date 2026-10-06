CC = gcc

CFLAGS = -Wall -Wextra -O2 -I./src

TARGET = resource_monitor

SRC = \
	src/main.c \
	src/resources.c \
	src/cpu.c \
	src/display.c

OBJ = $(SRC:.c=.o)


.PHONY: all clean run


all: $(TARGET)


$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@


src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@


run: $(TARGET)
	./$(TARGET)


clean:
	rm -f $(OBJ) $(TARGET)