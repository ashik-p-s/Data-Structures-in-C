SRC = main.c inverted_search.c
OBJ = $(SRC:.c=.o)

inverted_search: $(OBJ)
	gcc -o $@ $^ -Wall -g

%.o: %.c main.h
	gcc -c $< -o $@ -Wall -g

clean:
	rm -f *.o inverted_search a.out backup.txt *\:Zone.Identifier
	@echo "Cleaned up build files."