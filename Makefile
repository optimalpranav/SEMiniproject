# ============================================================================
# Railway Reservation System - Makefile
# Team 12 | PES University | Software Engineering Mini-Project
# ============================================================================

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

# Source files (Shared foundation + My modules)
SOURCES = $(SRC_DIR)/data_store.cpp \
          $(SRC_DIR)/validation.cpp \
          $(SRC_DIR)/reservation.cpp \
          $(SRC_DIR)/cancellation.cpp

# Test binary
TEST_TARGET = $(BUILD_DIR)/test_cancellation
TEST_SOURCES = $(SOURCES) $(TEST_DIR)/test_cancellation.cpp

.PHONY: all clean test

all: $(TEST_TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TEST_TARGET): $(TEST_SOURCES) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_SOURCES)

test: $(TEST_TARGET)
	cd $(SRC_DIR)/.. && $(BUILD_DIR)/test_cancellation

clean:
	rm -rf $(BUILD_DIR)
