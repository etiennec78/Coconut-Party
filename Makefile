SRC_DIR = src
SRC_FILES = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/**/*.c)

OBJ_DIR = build
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

INC_DIR = include

TARGET = game
CFLAGS = -I $(INC_DIR) -lm


# MARK: - Default rule
# NOTE: This rule is executed when no target is specified
all: $(TARGET)

# MARK: - Compilation of .c to .o files
# NOTE: This rule compiles all the .c files into .o files in the build directory
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	gcc -c $< -o $@ $(CFLAGS)

# MARK: - Generate the final file
# NOTE: This rule links all the .o files into the final executable
$(TARGET): $(OBJ_FILES)
	gcc $^ -o $@ $(CFLAGS)

# MARK: - Creating the build folder
# NOTE: This rule creates the build directory if it does not exist
$(OBJ_DIR):
	mkdir -p $@/

# MARK: - Cleaning build folder adn target
# NOTE: This rule removes the build directory and the target executable
clean:
	@rm -rf $(OBJ_DIR) $(TARGET)

# MARK: - Phony targets
# NOTE: This rule marks targets that do not represent files
.PHONY: all clean