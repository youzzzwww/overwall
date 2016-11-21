CC = g++
PROJECT_PATH := ..
TMP_DIR := $(PROJECT_PATH)/tmp
SRC_PATH := $(PROJECT_PATH)/src

TARGET_CXXFLAGS = -g -Wall --std=c++11
TARGET_LDFLAGS = -pthread -L/usr/local/lib -ldl
TARGET_INCLUDE = -I$(SRC_PATH)
TARGET_NAME = $(PROJECT_PATH)/bin/overwall
TEST_TARGET = $(PROJECT_PATH)/bin/testoverwall

SRC := $(wildcard $(SRC_PATH)/*.cc)
OBJ := $(notdir $(SRC:.cc=.o))
TEST_OBJ := $(filter-out main.o, $(OBJ))
MAIN_OBJ := $(filter-out test%, $(OBJ))
TEST_OBJS := $(patsubst %,$(TMP_DIR)/%, $(TEST_OBJ))
MAIN_OBJS := $(patsubst %,$(TMP_DIR)/%, $(MAIN_OBJ))

.PHONY : all
all : overwall testoverwall

overwall : $(MAIN_OBJS)
	$(CC) -o $(TARGET_NAME) $^ $(TARGET_LDFLAGS)

testoverwall : $(TEST_OBJS)
	$(CC) -o $(TEST_TARGET) $^ $(TARGET_LDFLAGS)

$(TMP_DIR)/%.o : $(SRC_PATH)/%.cc
	$(CC) $(TARGET_CXXFLAGS) -c -o $@ $^ $(TARGET_INCLUDE)

clean :
	rm -rf ${TMP_DIR}/*
