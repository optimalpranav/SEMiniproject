# ============================================================================
# Railway Reservation System - Makefile
# Team 12 | PES University | Software Engineering Mini-Project
# ============================================================================

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

# Source files
SOURCES = $(SRC_DIR)/data_store.cpp \
          $(SRC_DIR)/validation.cpp \
          $(SRC_DIR)/train_search.cpp \
          $(SRC_DIR)/booking.cpp \
          $(SRC_DIR)/reservation.cpp \
          $(SRC_DIR)/cancellation.cpp

# Main application
APP_TARGET = $(BUILD_DIR)/railway_system
APP_SOURCES = $(SOURCES) $(SRC_DIR)/main.cpp

# Test binary
TEST_TARGET = $(BUILD_DIR)/test_cancellation
TEST_SOURCES = $(SOURCES) $(TEST_DIR)/test_cancellation.cpp

.PHONY: all clean test run

all: $(APP_TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(APP_TARGET): $(APP_SOURCES) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(APP_SOURCES)

$(TEST_TARGET): $(TEST_SOURCES) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_SOURCES)

test: $(TEST_TARGET)
	cd $(SRC_DIR)/.. && $(BUILD_DIR)/test_cancellation

run: $(APP_TARGET)
	cd $(SRC_DIR)/.. && $(APP_TARGET)

clean:
	rm -rf $(BUILD_DIR)
