SOURCES=utestanalitza_main.cpp ../analitza/analitza.cpp
OBJECTS=utestanalitza_main.o analitza.o
TARGET=utestanalitza_main
COMPILER=g++ -c  
LINKER=g++ -o
CFLAGS=-I ../gtest -I ../analitza/
LINKFLAGS=-L ../gtest/libs -lgtest -lgtest_main -lpthread 

all: $(TARGET)

$(TARGET): OBJECTS
	$(LINKER) $(TARGET) $(LINKFLAGS) $(OBJECTS)

OBJECTS: $(SOURCES)
	$(COMPILER) $(CFLAGS) $(SOURCES)

run:
	DYLD_LIBRARY_PATH=../gtest/libs/ ./utestanalitza_main	
clean:
	rm $(OBJECTS)
	rm $(TARGET)
