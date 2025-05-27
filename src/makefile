# Compiler and flags
CC ?= gcc
CFLAGS ?= -g -Wall -Werror -Wextra
LIBS ?= -lncursesw

# Directories
SRC_DIR := .
BIN_DIR := bin

ifndef EXEC
EXEC := ./main
else
EXEC := ../$(EXEC)
endif

# Automatically find all source files (*.c) recursively
SRCS := $(shell find $(SRC_DIR) -name '*.c')
# Create object file list by replacing .c with .o and prefixing bin/
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRCS))

# Automatically include all subdirectories for -I
INCL := $(addprefix -I,$(shell find $(SRC_DIR) -type d))

# Default action
.PHONY: default
default: compile

.PHONY: compile
compile: $(EXEC)

.PHONY: run
run: compile
	@./$(EXEC)

.PHONY: valgrind
valgrind: compile
	@valgrind ./$(EXEC)

.PHONY: valgrind-full
valgrind-full: compile
	@valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes ./$(EXEC)

# Compile object files
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCL) -c $< -o $@ $(LIBS)

# Link final binary
$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(INCL) -o $@ $(LIBS)

# Clean targets
.PHONY: clean
clean:
	rm -rf $(BIN_DIR)/*.o $(EXEC) build/

.PHONY: clean-keep-build
clean-keep-build:
	rm -rf $(EXEC)

.PHONY: build
build: compile
