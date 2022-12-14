
CC=g++
CFLAGS=-lglfw
SRC=src
BINDIR=bin
BIN=$(BINDIR)/spazzo

SRCS=$(wildcard $(SRC)/*.cpp)
SRCS+=$(wildcard $(SRC)/*.c)

all: $(BIN)

$(BIN): $(SRCS)
	$(CC) $(CFLAGS) $^ -g -o $@ -ldl -lpthread -lm
clean:
	$(RM) $(BINDIR)/*





#$(CC) $(CFLAGS) $^ -g -o $@ -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/harfbuzz -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/sysprof-4 -pthread -L/usr/local/lib -lfreetype

