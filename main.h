#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

#define SCREEN_SIZE 1000
#define IX(x, y) (x + y * N)

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

// fluid.c
FluidCube *FluidCubeCreate(int size, int diffusion, int viscosity, float dt);
void FluidCubeFree(FluidCube *cube);
void FluidCubeAddDensity(FluidCube *cube, int x, int y, float amount);
void FluidCubeAddVelocity(FluidCube *cube, int x, int y, float amountX, float amountY);

// fluid_step.c
void set_bnd(int b, float *x, int N);
void lin_solve(int b, float *x, float *x0, float a, float c, int iter, int N);
void diffuse (int b, float *x, float *x0, float diff, float dt, int iter, int N);
void project(float *velocX, float *velocY, float *p, float *div, int iter, int N);
void advect(int b, float *d, float *d0,  float *velocX, float *velocY, float dt, int N);
void FluidCubeStep(FluidCube *cube);
