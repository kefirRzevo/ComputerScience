CXX = g++
CXXFLAGS = -Wall -g -O1 -std=c++11

TARGET = snake
OBJDIR = obj/
SRCS = main.cpp src/view.cpp

all: $(TARGET) obj

$(TARGET): $(OBJDIR)main.o $(OBJDIR)view.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) $^

$(OBJDIR)main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)view.o: src/view.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

obj:
	mkdir $(OBJDIR)

clean:
	$(RM) $(OBJDIR)* $(TARGET) depends

-include .depends

depend:
	$(CXX) -MM main.cpp src/view.cpp > depends
