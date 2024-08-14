CC := clang
CFLAGS := -Wall -g

SRCFILES := $(wildcard ./src/*.c)
OBJFILES := $(SRCFILES:.c=.o)

LINK := -lraylib -lm

OUTPUT := bin/sudoku-gui

all : $(OUTPUT)

$(OUTPUT) : $(OBJFILES)
	$(CC) $(CFLAGS) $^ -o $@ $(LINK)

%.o : %.c 
	$(CC) $(CFLAGS) -c $^ -o $@

run : 
	make
	$(OUTPUT)

clean : 
	rm $(OBJFILES) $(OUTPUT)

debug :
	make
	gdb ./$(OUTPUT)
