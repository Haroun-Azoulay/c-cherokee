CC = g++
CFLAGS = -g -Wall -pthread -Iinclude
LDFLAGS = -lcriterion

SRC_DIR = src
OBJ_DIR = build/obj
TARGET = build/server
TEST_TARGET = build/test_runner

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

TEST_SRCS = src/test.cpp src/utils.cpp src/http_handler.cpp src/request_router.cpp
TEST_OBJS = $(TEST_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)


all: $(TARGET)

$(TARGET): $(filter-out $(OBJ_DIR)/test.o, $(OBJS))
	$(CC) $(CFLAGS) -o $@ $^

$(TEST_TARGET): $(TEST_SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(TEST_TARGET)

.PHONY: all clean run test