all:
	@gcc *.c -lSDL2 -lSDL2_image -lm

debug:
	@gcc *.c -lSDL2 -lSDL2_image -g -lm
