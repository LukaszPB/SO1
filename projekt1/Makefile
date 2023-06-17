OBJ = main.o wczytaj.o sort.o zwolnij_pamiec.o signal.o
all: projekt1
projekt1: $(OBJ)
	gcc $(OBJ) -o projekt1
$(OBJ): function.h
.PHONY: clean
clean:
	rm -f *.o projekt1