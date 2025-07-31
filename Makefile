CXX = g++

ifeq ($(MODE), debug)
	CXXFLAGS = -Wall -g
else 
  CXXFLAGS = -Wall
endif

LDFFLAGS = -lGL -lGLEW -lglfw

LIBSOURCES = src/utils/window.cpp

SRC ?= src/sphere.cpp

TARGET ?= app

all:
	$(CXX) $(CXXFLAGS) $(SRC) $(LIBSOURCES) -o build/$(TARGET) $(LDFFLAGS)

clean:
	rm -f build/$(TARGET)
