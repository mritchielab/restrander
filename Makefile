CC = gcc
CFLAGS = -Wall -std=c++17
CXXFLAGS = -Wall -std=c++17
LDFLAGS = -lstdc++ -lz -lm

SOURCES = src/main.cpp src/classify.cpp src/ukkonenMatch.cpp src/utilities.cpp src/Record.cpp src/Reader.cpp src/Writer.cpp src/config.cpp src/artefact.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = restrander

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	@rm -f $(TARGET) $(OBJECTS)