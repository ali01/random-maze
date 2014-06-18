TARGET := maze_gen
CXX_FLAGS += -std=c++11 -Wall


.PHONY: all
all: $(TARGET)


.PHONY: clean
clean:
	rm -f $(TARGET)
	rm -f *.o


maze.o: maze.cpp maze.h
	$(CXX) $(CXX_FLAGS) -c $< -o $@


main.o: main.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@


$(TARGET): main.o maze.o
	$(CXX) $^ -o $(TARGET)
