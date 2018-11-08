CPP = g++ 
CPPFLAGS = -Wall -std=c++11 -O3
HERDERS = NODE.h LUT.h
SOURCES = DAO.cpp NODE.cpp LUT.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = DAO

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CPP) $(CPPFLAGS) $(OBJECTS) -o $@

%.o: %.cpp %.h
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean: 
	rm -rf *.o $(EXECUTABLE) *.out