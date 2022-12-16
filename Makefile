
CC=g++
CFLAGS=-g -lglfw -ldl -lpthread -lm -Wall
SRC=src
OBJ=obj
BINDIR=bin
BIN=$(BINDIR)/spazzo

SRCS_CPP=$(wildcard $(SRC)/*.cpp)
OBJS_CPP = $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRCS_CPP))

SRCS_C=$(wildcard $(SRC)/*.c)
OBJS_C=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS_C))

SRCS = $(SRCS_C) $(SRCS_CPP)
OBJS = $(OBJS_C) $(OBJS_CPP)


$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# NOTE: Whit this, the clean is executed even if there's a file called `clean`
.PHONY: clean
clean:
	$(RM) $(OBJ)/* $(BINDIR)/*

