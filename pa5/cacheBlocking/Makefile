OUTPUT=cacheBlocking
CFLAGS=-Wall -Werror -m64
# -fsanitize=address
SOURCES = cacheBlocking.0

TARGET = cacheBlocking
CC = gcc


all: $(SOURCES) $(TARGET)
    $(CC) $(SOURCES) -O0 -o $(TARGET) 

clean:
	rm -rf $(TARGET) 
