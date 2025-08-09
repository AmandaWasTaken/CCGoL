flags = -Wall -Wextra -Wpedantic

main: main.c
	gcc -o main main.c color.c $(flags)
