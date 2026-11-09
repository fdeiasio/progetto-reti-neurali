CC       = gcc
CFLAGS   = -Wall -Wextra -Wpedantic -Werror -MMD -MP
INCLUDE  = -I./inc -I./raylib/include
LIBS     = ./raylib/lib/libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11

BUILDDIR = build
BINDIR   = bin

GAME     = $(BINDIR)/game
TRAIN    = $(BINDIR)/train

# Simulation code shared by both executables (no rendering)
CORE_OBJ  = $(BUILDDIR)/core.o
GAME_OBJ  = $(BUILDDIR)/game.o $(BUILDDIR)/rendering.o $(CORE_OBJ)
TRAIN_OBJ = $(BUILDDIR)/training.o $(CORE_OBJ)

all: $(GAME) $(TRAIN)

$(GAME): $(GAME_OBJ) | $(BINDIR)
	$(CC) $^ -o $@ $(LIBS)

$(TRAIN): $(TRAIN_OBJ) | $(BINDIR)
	$(CC) $^ -o $@ $(LIBS)

$(BUILDDIR)/%.o: src/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(BUILDDIR) $(BINDIR):
	mkdir -p $@

clean:
	rm -rf $(BUILDDIR) $(BINDIR)

run: $(GAME)
	./$(GAME)

-include $(wildcard $(BUILDDIR)/*.d)

.PHONY: all clean run
