CC      = gcc
CFLAGS  = -Wall -Wextra -Wpedantic -Werror
INCLUDE = -I./inc -I./raylib/include
LIBS    = ./raylib/lib/libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11
BUILDDIR = build
BINDIR   = bin
TARGET  = $(BINDIR)/main
SRC     = $(wildcard src/*.c)
OBJ     = $(patsubst src/%.c,$(BUILDDIR)/%.o,$(SRC))

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

$(BUILDDIR)/%.o: src/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -f $(TARGET) $(OBJ)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
