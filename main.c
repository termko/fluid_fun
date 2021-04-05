#include "main.h"

int ISX(int x, int y)
{
	x = min(max(0, x), SCREEN_SIZE - 1);
	y = min(max(0, y), SCREEN_SIZE - 1);
	return (x + y * SCREEN_SIZE);
}

int init_SDL(t_sdl *sdl)
{
    //Initialization flag
    int success = 1;
	sdl->window = SDL_CreateWindow("Fluid Sim", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, SCREEN_SIZE, SCREEN_SIZE, 0);
	sdl->renderer = SDL_CreateRenderer(sdl->window, -1, 0);
	sdl->texture = SDL_CreateTexture(sdl->renderer,
			SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC,
			SCREEN_SIZE, SCREEN_SIZE);
	
	sdl->pix = (Uint32*)malloc(sizeof(Uint32) *
				(SCREEN_SIZE * SCREEN_SIZE));
	if (!sdl->window || !sdl->renderer)
	{
		printf("Something went wrong\n");
		success = 0;
	}
	memset(sdl->pix, 0, SCREEN_SIZE * SCREEN_SIZE * sizeof(Uint32));
//Probably on a loop
	SDL_RenderDrawPoint(sdl->renderer, 400, 300); //Renders on middle of screen.
	SDL_RenderPresent(sdl->renderer);
	sdl->mouse.x = 0;
	sdl->mouse.y = 0;
	sdl->mouse_pressed = 0;
     	sdl->cube = FluidCubeCreate(0.9, 0.1, 0.01);
	return success;
}

void render_screen(t_sdl *sdl)
{
	SDL_UpdateTexture(sdl->texture, NULL, sdl->pix,
		SCREEN_SIZE * sizeof(Uint32));
	SDL_RenderClear(sdl->renderer);
	SDL_RenderCopy(sdl->renderer, sdl->texture, NULL, NULL);
	SDL_RenderPresent(sdl->renderer);
}

void mouse_handler(t_sdl *sdl, SDL_Event evt)
{
	if (evt.type == SDL_MOUSEBUTTONDOWN)
		sdl->mouse_pressed = 1;
	else if (evt.type == SDL_MOUSEBUTTONUP)
		sdl->mouse_pressed = 0;
}

void event_listener(t_sdl *sdl)
{
	SDL_Event evt;

	while (SDL_PollEvent(&evt))
	{
		if ((SDL_QUIT == evt.type) || (SDL_KEYDOWN == evt.type &&
				SDL_SCANCODE_ESCAPE == evt.key.keysym.scancode))
		exit(0);
		if( evt.type == SDL_MOUSEMOTION || evt.type == SDL_MOUSEBUTTONDOWN || evt.type == SDL_MOUSEBUTTONUP )
			mouse_handler(sdl, evt);
	}
}

void noise_pixels(t_sdl *sdl)
{
	srand(time(NULL));   // Initialization, should only be called once.
	for (int i = 0; i < SCREEN_SIZE; i++)
	{
		for (int j = 0; j < SCREEN_SIZE; j++)
		{
			int r = rand();
			sdl->pix[ISX(i, j)] = r;
		}
	}
}

t_minmax find_minmax_float(float *arr)
{
	t_minmax ret = {arr[0], arr[0]};
	for (int i = 0; i < CUBE_SIZE - 1; i++)
	{
		for (int j = 0; j < CUBE_SIZE - 1; j++)
		{
			if (arr[IX(i, j)] > ret.max)
				ret.max = arr[IX(i, j)];
			if (arr[IX(i, j)] < ret.min)
				ret.min = arr[IX(i, j)];
		}
	}
	return ret;
}

int normalize_density(float density, t_minmax minmax)
{
	int ret = (int)(((density - minmax.min)/(minmax.max - minmax.min)) * 255);
	return (int)(((density - minmax.min)/(minmax.max - minmax.min)) * 255);
}

void change_pixels(t_sdl *sdl)
{
	if (sdl->mouse_pressed)
	{
		static int time = 0;
		FluidCubeAddDensity(sdl->cube, sdl->mouse.x / SQUARE_SIZE, sdl->mouse.y / SQUARE_SIZE, 100);
	if (time > 10)
	{
		time = 0;
		sdl->pmouse = sdl->mouse;
	}
		time++;
	SDL_GetMouseState(&(sdl->mouse.x), &(sdl->mouse.y));
		float xvel = (sdl->mouse.x - sdl->pmouse.x) * 1000;
		float yvel = (sdl->mouse.y - sdl->pmouse.y) * 1000;
		FluidCubeAddVelocity(sdl->cube, sdl->mouse.x / SQUARE_SIZE, sdl->mouse.y / SQUARE_SIZE, xvel, yvel);
	}
	FluidCubeStep(sdl->cube);
	t_minmax min_max = find_minmax_float(sdl->cube->density);
	printf("%f %f\n", min_max.min, min_max.max);
	for (int i = 0; i < SCREEN_SIZE; i++)
	{
		for (int j = 0; j < SCREEN_SIZE; j++)
		{
			float dens_float = sdl->cube->density[IX(i / SQUARE_SIZE, j / SQUARE_SIZE)];
			int density = normalize_density(dens_float, min_max);
			sdl->pix[ISX(i, j)] = ((density) << 16) | ((density) << 8) | ((density));
			//sdl->pix[ISX(i, j)] = (int)density;
		}
	}
}

void try_SDL()
{
	SDL_Event evt;
	t_sdl *sdl = (t_sdl*)malloc(sizeof(t_sdl));
    //Start up SDL and create window
    if( !init_SDL(sdl))
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
	while (1)
	{
		// noise_pixels(sdl);
		change_pixels(sdl);

		render_screen(sdl);

		event_listener(sdl);
	}
    }
    //Free resources and close SDL
    SDL_Quit();
}

int main( int argc, char* args[] )
{
	try_SDL();
	return 0;
}
