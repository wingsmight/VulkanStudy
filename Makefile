CXX := g++
CXX_FLAGS := -g -std=c++17 -Wextra -Wall

SRC	:= src
INCLUDE	:= ./include
LIB	:= ./lib

LIBRARIES := -lglfw3 -lvulkan -ldl -lpthread -lboost_filesystem -lboost_system
EXECUTABLE := main
FRAMEWORKS := -framework Cocoa -framework OpenGL -framework IOKit
SUBFOLDERSRCS := ./lib/glad.c src/*/*.cpp
APPNAME := main

all:./$(EXECUTABLE)

run: all
	./$(EXECUTABLE)

$(EXECUTABLE):$(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I $(INCLUDE) -L $(LIB) $^ $(SUBFOLDERSRCS) -o $(APPNAME).app $(LIBRARIES) $(FRAMEWORKS) ;
	open $(APPNAME).app
