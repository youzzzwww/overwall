CC = g++
PROJECT_PATH := ..
TMP_DIR := $(PROJECT_PATH)/tmp
SRC_PATH := $(PROJECT_PATH)/src

TARGET_CXXFLAGS = -g -Wall --std=c++11
TARGET_LDFLAGS = -pthread -L/usr/local/lib -ldl
TARGET_INCLUDE = -I$(SRC_PATH)
TARGET_NAME = $(PROJECT_PATH)/bin/overwall

SRC := $(wildcard $(SRC_PATH)/*.cc)
OBJ := $(notdir $(SRC:.cc=.o))
OBJS := $(patsubst %,$(TMP_DIR)/%, $(OBJ))

overwall : $(OBJS)
	$(CC) -o $(TARGET_NAME) $^ $(TARGET_LDFLAGS)

$(TMP_DIR)/%.o : $(SRC_PATH)/%.cc
	$(CC) $(TARGET_CXXFLAGS) -c -o $@ $^ $(TARGET_INCLUDE)

.PHONY : clean

clean :
	rm -rf ${TMP_DIR}/*
