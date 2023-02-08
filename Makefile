CXX = g++
CXXFLAGS = -Wall -g -O1

GAME = snake
OBJCTS = main.o model.o view.o controller.o

$(GAME): $(OBJCTS)
	$(CXX) -o $(GAME) $(OBJCTS)

clean:
	$(RM) $(OBJCTS) $(GAME) .depends

depend:
	$(CXX) -MM $(OBJCTS:.o := .cc) > .depends

-include .depends
