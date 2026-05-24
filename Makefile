run: kilo
	./kilo

kilo: kilo.c
	$(CC) kilo.c -o kilo -Wall -Wextra -std=c99