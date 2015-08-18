
SOURCES= analitza.cpp  analitza_main.cpp
OBJECTS=analitza.o analitza_main.o
INCLUDES=analitza.h

CC=g++  -c
LINKER=g++ -o

TARGET= analitza

all: $(TARGET)

$(TARGET): OBJECTS
	$(LINKER) $(TARGET) $(LFLAGS) $(OBJECTS)
	@echo "Linking complete!"	

OBJECTS: $(SOURCES) $(INCLUDES)
	$(CC) $(CFLAGS) $(SOURCES)

clean:
	rm $(TARGET) $(OBJECTS)
	@echo "Cleanup complete!"
