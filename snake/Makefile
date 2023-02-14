CXX = g++
CXXFLAGS = -Wall -g -O1

TARGET = snake
OBJDIR = obj/

all: $(TARGET)

$(TARGET): $(OBJDIR)main.o $(OBJDIR)view.o
	$(CXX) -o $(TARGET) $^

$(OBJDIR)main.o: main.cpp
	$(CXX) -c -o $@ $<

$(OBJDIR)view.o: view/view.cpp
	$(CXX) -c -o $@ $<

obj:
	mkdir $(OBJDIR)

clean:
	$(RM) $(OBJDIR)* $(TARGET) depends

-include .depends

depend:
	$(CXX) -MM main.cpp view/view.cpp > depends
