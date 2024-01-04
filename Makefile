all: compile run

compile:
	gcc main.c -o main.exe -O1 -Wall -std=c99 -Wno-missing-braces -I ./library/include/ -L ./library/lib/ -lraylib -lopengl32 -lgdi32 -lwinmm

run:
	.\main.exe