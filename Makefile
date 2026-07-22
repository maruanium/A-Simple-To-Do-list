CC = gcc

CFLAGS = -Iinclude \
         -ILoading-Screen-C-Lib/include

SRC = src/main.c \
      src/tasks.c \
      src/loading_screen.c

TARGET = todo

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
