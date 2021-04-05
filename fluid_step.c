#include "main.h"

int min(int a, int b)
{
	return a < b ? a : b;
}

int max(int a, int b)
{
	return a > b ? a : b;
}

int IX(int x, int y)
{
	x = min(max(0, x), CUBE_SIZE - 1);
	y = min(max(0, y), CUBE_SIZE - 1);
	return (x + y * CUBE_SIZE);
}

void set_bnd(int b, float *x)
{
    for(int k = 1; k < CUBE_SIZE - 1; k++) {
        for(int i = 1; i < CUBE_SIZE - 1; i++) {
            x[IX(i, 0)] = b == 2 ? -x[IX(i, 1)] : x[IX(i, 1)];
            x[IX(i, CUBE_SIZE-1)] = b == 2 ? -x[IX(i, CUBE_SIZE-2)] : x[IX(i, CUBE_SIZE-2)];
        }
    }
    for(int k = 1; k < CUBE_SIZE - 1; k++) {
        for(int j = 1; j < CUBE_SIZE - 1; j++) {
            x[IX(0  , j)] = b == 1 ? -x[IX(1  , j)] : x[IX(1  , j)];
            x[IX(CUBE_SIZE-1, j)] = b == 1 ? -x[IX(CUBE_SIZE-2, j)] : x[IX(CUBE_SIZE-2, j)];
        }
    }
    
    x[IX(0, 0)]       = 0.33f * (x[IX(1, 0)]
                                  + x[IX(0, 1)]
                                  + x[IX(0, 0)]);
    x[IX(0, CUBE_SIZE-1)]     = 0.33f * (x[IX(1, CUBE_SIZE-1)]
                                  + x[IX(0, CUBE_SIZE-2)]
                                  + x[IX(0, CUBE_SIZE-1)]);
    x[IX(CUBE_SIZE-1, 0)]     = 0.33f * (x[IX(CUBE_SIZE-2, 0)]
                                  + x[IX(CUBE_SIZE-1, 1)]
                                  + x[IX(CUBE_SIZE-1, 0)]);
    x[IX(CUBE_SIZE-1, CUBE_SIZE-1)]   = 0.33f * (x[IX(CUBE_SIZE-2, CUBE_SIZE-1)]
                                  + x[IX(CUBE_SIZE-1, CUBE_SIZE-2)]
                                  + x[IX(CUBE_SIZE-1, CUBE_SIZE-1)]);
}

void lin_solve(int b, float *x, float *x0, float a, float c, int iter)
{
    float cRecip = 1.0 / c;
    for (int k = 0; k < iter; k++) {
            for (int j = 1; j < CUBE_SIZE - 1; j++) {
                for (int i = 1; i < CUBE_SIZE - 1; i++) {
	                x[IX(i, j)] =
                        (x0[IX(i, j)]
                            + a*(    x[IX(i+1, j)]
                                    +x[IX(i-1, j)]
                                    +x[IX(i, j+1)]
                                    +x[IX(i, j-1)]
                           )) * cRecip;
                }
            }
        set_bnd(b, x);
    }
}

void diffuse (int b, float *x, float *x0, float diff, float dt, int iter)
{
    float a = dt * diff * (CUBE_SIZE - 2) * (CUBE_SIZE - 2);
    lin_solve(b, x, x0, a, 1 + 6 * a, iter);
}

void project(float *velocX, float *velocY, float *p, float *div, int iter)
{
        for (int j = 1; j < CUBE_SIZE - 1; j++) {
            for (int i = 1; i < CUBE_SIZE - 1; i++) {
                div[IX(i, j)] = -0.5f*(
                         velocX[IX(i+1, j)]
                        -velocX[IX(i-1, j)]
                        +velocY[IX(i  , j+1)]
                        -velocY[IX(i  , j-1)]
                    )/CUBE_SIZE;
                p[IX(i, j)] = 0;
            }
        }
    set_bnd(0, div);
    set_bnd(0, p);
    lin_solve(0, p, div, 1, 6, iter);

        for (int j = 1; j < CUBE_SIZE - 1; j++) {
            for (int i = 1; i < CUBE_SIZE - 1; i++) {
                velocX[IX(i, j)] -= 0.5f * (  p[IX(i+1, j)]
                                                -p[IX(i-1, j)]) * CUBE_SIZE;
                velocY[IX(i, j)] -= 0.5f * (  p[IX(i, j+1)]
                                                -p[IX(i, j-1)]) * CUBE_SIZE;
            }
        }
    set_bnd(1, velocX);
    set_bnd(2, velocY);
}

void advect(int b, float *d, float *d0,  float *velocX, float *velocY, float dt)
{
    float i0, i1, j0, j1;
    
    float dtx = dt * (CUBE_SIZE - 2);
    float dty = dt * (CUBE_SIZE - 2);
    
    float s0, s1, t0, t1;
    float tmp1, tmp2, x, y;
    
    float Nfloat = CUBE_SIZE;
    float ifloat, jfloat;
    int i, j;
    
        for(j = 1, jfloat = 1; j < CUBE_SIZE - 1; j++, jfloat++) { 
            for(i = 1, ifloat = 1; i < CUBE_SIZE - 1; i++, ifloat++) {
                tmp1 = dtx * velocX[IX(i, j)];
                tmp2 = dty * velocY[IX(i, j)];
                x    = ifloat - tmp1; 
                y    = jfloat - tmp2;
                
                if(x < 0.5f) x = 0.5f; 
                if(x > Nfloat + 0.5f) x = Nfloat + 0.5f; 
                i0 = floorf(x); 
                i1 = i0 + 1.0f;
                if(y < 0.5f) y = 0.5f; 
                if(y > Nfloat + 0.5f) y = Nfloat + 0.5f; 
                j0 = floorf(y);
                j1 = j0 + 1.0f; 
                
                s1 = x - i0; 
                s0 = 1.0f - s1; 
                t1 = y - j0; 
                t0 = 1.0f - t1;
                
                int i0i = i0;
                int i1i = i1;
                int j0i = j0;
                int j1i = j1;
                
                d[IX(i, j)] = 
                
                    s0 * ( t0 * d0[IX(i0i, j0i)]
                        +( t1 * d0[IX(i0i, j1i)]))
                   +s1 * ( t0 * d0[IX(i1i, j0i)]
                        +( t1 * d0[IX(i1i, j1i)]));
            }
        }
    set_bnd(b, d);
}

void FluidCubeStep(FluidCube *cube)
{
    float visc     = cube->visc;
    float diff     = cube->diff;
    float dt       = cube->dt;
    float *Vx      = cube->Vx;
    float *Vy      = cube->Vy;
    float *Vx0     = cube->Vx0;
    float *Vy0     = cube->Vy0;
    float *s       = cube->s;
    float *density = cube->density;

    diffuse(1, Vx0, Vx, visc, dt, 4);
    diffuse(2, Vy0, Vy, visc, dt, 4);

    project(Vx0, Vy0, Vx, Vy, 4);

    advect(1, Vx, Vx0, Vx0, Vy0, dt);
    advect(2, Vy, Vy0, Vx0, Vy0, dt);

    project(Vx, Vy, Vx0, Vy0, 4);

    diffuse(0, s, density, diff, dt, 4);
    advect(0, density, s, Vx, Vy, dt);
}
