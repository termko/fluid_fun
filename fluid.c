#include "main.h"

FluidCube *FluidCubeCreate(int size, int diffusion, int viscosity, float dt)
{
    FluidCube *cube = malloc(sizeof(*cube));
   int N = SCREEN_SIZE;

    cube->size = size;
    cube->dt = dt;
    cube->diff = diffusion;
    cube->visc = viscosity;
    
    cube->s = calloc(N * N, sizeof(float));
    cube->density = calloc(N * N, sizeof(float));
    
    cube->Vx = calloc(N * N, sizeof(float));
    cube->Vy = calloc(N * N, sizeof(float));
    
    cube->Vx0 = calloc(N * N, sizeof(float));
    cube->Vy0 = calloc(N * N, sizeof(float));
    
    return cube;
}

void FluidCubeFree(FluidCube *cube)
{
    free(cube->s);
    free(cube->density);
    
    free(cube->Vx);
    free(cube->Vy);
    
    free(cube->Vx0);
    free(cube->Vy0);
    
    free(cube);
}

void FluidCubeAddDensity(FluidCube *cube, int x, int y, float amount)
{
    int N = cube->size;
    cube->density[IX(x, y)] += amount;
}

void FluidCubeAddVelocity(FluidCube *cube, int x, int y, float amountX, float amountY)
{
    int N = cube->size;
    int index = IX(x, y);

    cube->Vx[index] += amountX;
    cube->Vy[index] += amountY;
}
