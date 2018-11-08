CPP = g++ 
CPPFLAGS = -Wall -std=c++11 -O3
HERDERS = NODE.h
SOURCES = main.cpp NODE.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CPP) $(CPPFLAGS) $(OBJECTS) -o $@

%.o: %.cpp %.h
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean: 
	rm -rf *.o $(EXECUTABLE)