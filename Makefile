CC=gcc
CFLAGS=-g -Wall
SRC_DIR=src
INC_DIR=h
OBJ_DIR=obj
TARGET=a.out

# List of .c files without the extension
SRC_FILES = $(notdir $(basename $(wildcard $(SRC_DIR)/*.c)))
OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(SRC_FILES)))

$(TARGET): $(OBJ_FILES)
	$(CC) -o $@ $(OBJ_FILES) -L/wiredtiger/build/ -lwiredtiger

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INC_DIR)

.PHONY: clean
clean:
	rm -rf $(OBJ_FILES) $(TARGET)