CC=g++
STD=-std=c++11
CF=$(STD)
BUILD_DIR=build

all: $(BUILD_DIR) main-process.out worker.out

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/main.o: main.cpp
	$(CC) $(CF) -c main.cpp -o $(BUILD_DIR)/main.o

main-process.out: $(BUILD_DIR)/main.o
	$(CC) $(CF) -o main-process.out $(BUILD_DIR)/main.o

$(BUILD_DIR)/worker.o: worker.cpp
	$(CC) $(CF) -c worker.cpp -o $(BUILD_DIR)/worker.o


worker.out: $(BUILD_DIR)/worker.o
	$(CC) $(CF) -o worker.out $(BUILD_DIR)/worker.o


.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) *.o *.out

