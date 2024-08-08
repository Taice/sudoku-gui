CC := clang
CFLAGS := -Wall -g

SRCFILES := $(wildcard ./src/*.c)
OBJFILES := $(SRCFILES:.c=.o)

LINK := -lraylib -lm

OUTPUT := sudoku-gui

all : $(OUTPUT)

$(OUTPUT) : $(OBJFILES)
	$(CC) $(CFLAGS) $^ -o ./bin/$@ $(LINK)

%.o : %.c 
	$(CC) $(CFLAGS) -c $^ $(LINK)

run : 
	make
	./bin/$(OUTPUT)

clean : 
	rm $(OBJFILES) $(OUTPUT)

debug :
	make
	gdb ./$(OUTPUT)
