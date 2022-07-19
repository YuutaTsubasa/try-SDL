/*This source code copyrighted by Lazy Foo' Productions (2004-2022)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const string DEFAULT_IMAGE_PATH = "tutorial.png";
const string UP_IMAGE_PATH = "tutorial2.bmp";

SDL_Window* createWindow()
{
	SDL_Window* window = NULL;
	window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

	if (window == NULL)
		return NULL;

	SDL_Surface* screenSurface = SDL_GetWindowSurface( window );
	SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x00, 0x00, 0x00 ) );
	SDL_UpdateWindowSurface( window );
	return window;
}

SDL_Surface* loadImage(SDL_Window* window, string path){
	SDL_Surface* rawImage = IMG_Load(path.c_str());
	if (rawImage == NULL)
		return NULL;

	SDL_Surface* screenSurface = SDL_GetWindowSurface( window );
	SDL_Surface* optimizedImage = SDL_ConvertSurface( rawImage, screenSurface->format, 0 );
	SDL_FreeSurface(rawImage);

	return optimizedImage;
}

void renderImage(SDL_Window* window, SDL_Surface* image) {
	SDL_Surface* screenSurface = SDL_GetWindowSurface( window );
	SDL_Rect stretchRect;
	stretchRect.x = 0;
	stretchRect.y = 0;
	stretchRect.w = SCREEN_WIDTH;
	stretchRect.h = SCREEN_HEIGHT;
	SDL_BlitScaled( image, NULL, screenSurface, &stretchRect );
	SDL_UpdateWindowSurface( window );
}

void waitEvent(SDL_Window* window, SDL_Surface* defaultImage, SDL_Surface* upImage){
	bool isQuit = false;

	while(!isQuit) {
		SDL_Event event;
		while(SDL_PollEvent(&event) != 0){
			if(event.type == SDL_QUIT){
				isQuit = true;
			}
			else if(event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_UP) {
					renderImage(window, upImage);
				} 
				else {
					renderImage(window, defaultImage);
				}
			}
		}
	}
}

int main( int argc, char* args[] )
{
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return -1;
	}

	if( !( IMG_Init( IMG_INIT_PNG ) & IMG_INIT_PNG ) )
	{
		printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
		return -1;
	}

	SDL_Window* window = createWindow();
	if (window == NULL){
		printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		return -1;
	}

	SDL_Surface* defaultImage = loadImage(window, DEFAULT_IMAGE_PATH);
	if( defaultImage == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", DEFAULT_IMAGE_PATH.c_str(), SDL_GetError() );
        return -1;
    }

	SDL_Surface* upImage = loadImage(window, UP_IMAGE_PATH);
	if( upImage == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", UP_IMAGE_PATH.c_str(), SDL_GetError() );
        return -1;
    }
	
	renderImage(window, defaultImage);

	waitEvent(window, defaultImage, upImage);

	SDL_FreeSurface(defaultImage);
	SDL_FreeSurface(upImage);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}