
SOURCES=GraphCompareMain.cpp
OBJECTS=GraphCompareMain.cpp.o
INCLUDES=GraphCompareMain.h

CC=g++  -c
LINKER=g++ -o

GSL_MATRIX_INCLUDE_DIR=../../../gsl-1.16/include
BOOST_INCLUDE_DIR=../../../boost/include
CFLAGS=-I$(GSL_MATRIX_INCLUDE_DIR)
CFLAGS+=-I$(BOOST_INCLUDE_DIR)

TARGET= graphcomapre

all: $(TARGET)

OBJECTS: $(SOURCES) $(INCLUDES)
	$(CC) $(CFLAGS) $(SOURCES)


$(TARGET): OBJECTS
	$(LINKER) $(TARGET) $(LFLAGS) $(OBJECTS)
	@echo "Linking complete!"	

clean:
	rm $(TARGET) $(OBJECTS)
	@echo "Cleanup complete!"
