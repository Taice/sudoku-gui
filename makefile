CC := clang
CFLAGS := -Wall -g

SRCFILES := $(wildcard ./src/*.c)
OBJFILES := $(SRCFILES:.cpp=.o)

LINK := -lraylib

OUTPUT := sudoku-gui

all : $(OUTPUT)

$(OUTPUT) : $(OBJFILES)
	$(CC) $(CFLAGS) $^ -o ./bin/$@ $(LINK)

%.o : %.cpp $.hpp
	$(CC) $(CFLAGS) -c $^ $(LINK)

run : 
	make
	./bin/$(OUTPUT)

clean : 
	rm $(OBJFILES) $(OUTPUT)

debug :
	make
	gdb ./$(OUTPUT)
