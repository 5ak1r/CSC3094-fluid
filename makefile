INCLUDE_DIR := include
BUILD_DIR   := build
EXEC        := fluid
SRC_DIRS    := src
SRC         := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c*))
TARGET      := $(BUILD_DIR)/$(EXEC)
CXX         := g++
CXXFLAGS    := -o $(TARGET) -I$(INCLUDE_DIR) -lglfw -lGL -lGLU -lassimp

build: $(SRC)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(SRC) $(CXXFLAGS)

run: build
	$(TARGET)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)