CPP = g++ -std=c++11
CPPFLAGS = -Wall -O3
HERDERS = NODE.h
SOURCES = main.cpp NODE.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CPP) $(CPPFLAGS) $(OBJECTS) -o $@

%.o: %.cpp %.h
	$(CPP) $(CPPFLAGS) -c $<

clean: 
	rm -rf *.o $(EXECUTABLE)