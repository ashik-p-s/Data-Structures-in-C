SRC = main.c apc.c add.c sub.c mul.c div.c
OBJ = $(SRC:.c=.o)

apc: $(OBJ)
	gcc -o $@ $^ -Wall -g

%.o: %.c apc.h
	gcc -c $< -o $@ -Wall -g

clean:
	rm -f *.o apc a.out *\:Zone.Identifier