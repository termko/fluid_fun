#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define SCREEN_SIZE 1024
#define SQUARE_SIZE 8
#define CUBE_SIZE SCREEN_SIZE/SQUARE_SIZE

typedef struct	s_minmax {
	float min;
	float max;
}		t_minmax;

typedef struct	s_p {
	int x;
	int y;
}		t_p;

typedef struct	FluidCube {
	int size;
	float dt;	
	float diff;
	float visc;

	float *s;
	float *density;

	float *Vx;
	float *Vy;

	float *Vx0;
	float *Vy0;
}		FluidCube;

typedef struct	s_sdl {
	FluidCube	*cube;
	SDL_Renderer	*renderer;
	SDL_Window	*window;
	SDL_Texture	*texture;
	Uint32		*pix;
	t_p		mouse;
	t_p		pmouse;
	int		mouse_pressed;
}		t_sdl;

// fluid.c
FluidCube *FluidCubeCreate(int diffusion, int viscosity, float dt);
void FluidCubeFree(FluidCube *cube);
void FluidCubeAddDensity(FluidCube *cube, int x, int y, float amount);
void FluidCubeAddVelocity(FluidCube *cube, int x, int y, float amountX, float amountY);

// fluid_step.c
void set_bnd(int b, float *x);
void lin_solve(int b, float *x, float *x0, float a, float c, int iter);
void diffuse (int b, float *x, float *x0, float diff, float dt, int iter);
void project(float *velocX, float *velocY, float *p, float *div, int iter);
void advect(int b, float *d, float *d0,  float *velocX, float *velocY, float dt);
void FluidCubeStep(FluidCube *cube);
