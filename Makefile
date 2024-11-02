all:
	gcc -std=c17 main.c -I src/include -L src/lib -Wall -lmingw32 -lSDL2main -lSDL2 -lm -o main