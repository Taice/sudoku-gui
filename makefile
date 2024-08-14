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

check : 
	make
	$(OUTPUT) realtime

clean : 
	rm $(OBJFILES) $(OUTPUT)

debug :
	make
	gdb ./$(OUTPUT)
