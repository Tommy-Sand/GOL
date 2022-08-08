OS := $(shell uname)

main: main.c
ifeq ($(OS), Linux)
	gcc main.c -lSDL2main -lSDL2 -g -o main.exe
else
	gcc main.c -lmingw32 -lSDL2main -lSDL2 -g -o main.exe
endif
