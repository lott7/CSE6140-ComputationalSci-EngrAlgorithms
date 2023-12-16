CC = g++
CFLAGS = -std=c++17 -Wall

# Add additional source files here
SRCS = main.cpp TravelingSalesmanSolver.cpp Approximation.cpp BruteForce.cpp LocalSearch.cpp

# Derive object file names from source file names
OBJS = $(SRCS:.cpp=.o)

# Target executable
TARGET = my_program

# Build rule for the target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Build rule for object files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove object files and the executable
clean:
	rm -f $(OBJS) $(TARGET)
