
CC=g++
CFLAGS=-lglfw
SRC=src
BINDIR=bin
BIN=$(BINDIR)/spazzo

SRCS=$(wildcard $(SRC)/*.cpp)
SRCS+=$(wildcard $(SRC)/*.c)

all: $(BIN)

$(BIN): $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	$(RM) $(BINDIR)/*
