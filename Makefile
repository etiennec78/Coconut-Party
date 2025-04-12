SRC_FILES=$(wildcard *.c)
OBJ_FILES=$(SRC_FILES:.c=.o)
BUILD_DIR=build
BUILD_OBJ_FILES=$(addprefix $(BUILD_DIR)/, $(OBJ_FILES))

# MARK: - Default rule
all: clean $(BUILD_DIR)/game

# MARK: - Compilation of .c to .o files
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	gcc -c $< -o $@

# MARK: - Generate the final file
$(BUILD_DIR)/game: $(BUILD_OBJ_FILES)
	gcc $^ -o $@

# MARK: - Creating the build folder
$(BUILD_DIR):
	mkdir -p $@/

# MARK: - Cleaning build folder
clean:
	@if [ -d $(BUILD_DIR) ]; then \
		rm -rf $(BUILD_DIR); \
	fi