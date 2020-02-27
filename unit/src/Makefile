CC = gcc

INCLUDE_DIR = ./include
BUILD_DIR = ./build
BIN_DIR = $(BUILD_DIR)/bin
OBJ_DIR = $(BUILD_DIR)/obj
INCLUDE = -I$(INCLUDE_DIR)

CFLAGS += $(INCLUDE)

OBJ = $(OBJ_DIR)/main.o $(OBJ_DIR)/lib.o $(OBJ_DIR)/increase.o
BIN = $(BIN_DIR)/increaser

all: $(BIN)

$(BIN): $(OBJ) $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ)

$(BIN_DIR): $(BUILD_DIR)
	mkdir -p $(BIN_DIR)

$(OBJ_DIR): $(BUILD_DIR)
	mkdir -p $(OBJ_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(OBJ_DIR)/%.o: %.c $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -r $(BUILD_DIR)

.PHONY: all clean
