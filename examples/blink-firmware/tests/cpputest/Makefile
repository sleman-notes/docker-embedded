CC=gcc
CXX=g++

TARGET=tests
BUILD_DIR= build

SRC_DIR= ../../Src

CFLAGS= -c -std=gnu99 -Wall -O0 -g
CXXFLAGS= -c -std=c++17 -Wall -O0 -g
LDFLAGS= -lCppUTest -lCppUTestExt

###########################################
#				 INCLUDES
###########################################

INCLUDES+= -I ../../Inc/


############################################
# 				SOURCE FILES
############################################

OBJS		+= $(BUILD_DIR)/main.o
OBJS		+= $(BUILD_DIR)/test_gpio_bits.o

SOURCES		+= $(BUILD_DIR)/gpio_bits.o


PREREQ = $(BUILD_DIR)/

all: $(PREREQ) $(BUILD_DIR)/$(TARGET)

$(PREREQ):
	mkdir $(PREREQ)

$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(BUILD_DIR)/$(*).o $(*).cpp

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(BUILD_DIR)/$(*).o $(SRC_DIR)/$(*).c

$(BUILD_DIR)/$(TARGET): $(OBJS) $(SOURCES)
	$(CXX) -o $@ $^ $(LDFLAGS)

run: all
	./$(BUILD_DIR)/$(TARGET) -c

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean