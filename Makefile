CC      = gcc
CFLAGS  = -Wall -Wextra -Wpedantic -Werror
INCLUDE = -I./include
LIBS    = ./lib/libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11
TARGET  = main
SRC     = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(INCLUDE) $(LIBS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
