CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = malloc_test
OBJS = main.o malloc.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp malloc.h
	$(CXX) $(CXXFLAGS) -c main.cpp

malloc.o: malloc.cpp malloc.h
	$(CXX) $(CXXFLAGS) -c malloc.cpp

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
