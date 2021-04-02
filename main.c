#include "main.h"

SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* gHelloWorld = NULL;

int init_SDL()
{
    //Initialization flag
    int success = 1;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = 0;
    }
    else
    {
        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_SIZE, SCREEN_SIZE, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = 0;
        }
        else
        {
            //Get window surface
            gScreenSurface = SDL_GetWindowSurface( gWindow );
        }
    }

    return success;
}

int load_picture()
{
    //Loading success flag
    int success = 1;

    //Load splash image
    gHelloWorld = IMG_Load( "11.jpg" );
    if( gHelloWorld == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", "02_getting_an_image_on_the_screen/hello_world.bmp", SDL_GetError() );
        success = 0;
    }

    return success;
}

void close_SDL()
{
    //Deallocate surface
    SDL_FreeSurface( gHelloWorld );
    gHelloWorld = NULL;

    //Destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

void try_SDL()
{
    //Start up SDL and create window
    if( !init_SDL() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        //Load media
        if( !load_picture() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            //Apply the image
            SDL_BlitSurface( gHelloWorld, NULL, gScreenSurface, NULL );
	    //Update the surface
            SDL_UpdateWindowSurface( gWindow );
	                //Wait two seconds
            SDL_Delay( 2000 );
        }
    }

    //Free resources and close SDL
    close_SDL();
}

int main( int argc, char* args[] )
{

    FluidCube *cube = FluidCubeCreate(100, 0, 0, 0.1);
    return 0;
}
