CXX = g++

ifeq ($(MODE), debug)
	CXXFLAGS = -Wall -v -g
else 
  CXXFLAGS = -Wall
endif

CXX_STANDARD = --std=c++20
LDFFLAGS = -lGL -lGLEW -lglfw

LIBSOURCES = src/utils/window.cpp

SRC ?= src/sphere.cpp

TARGET ?= app

all:
	$(CXX) $(CXXFLAGS) $(SRC) $(LIBSOURCES) -o build/$(TARGET) $(LDFFLAGS) $(CXX_STANDARD)

clean:
	rm -f build/*
