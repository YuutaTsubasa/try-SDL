/*This source code copyrighted by Lazy Foo' Productions (2004-2022)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
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

	return window;
}

SDL_Renderer* createRenderer(SDL_Window* window) {
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	
	if( renderer == NULL )
		return NULL;

	SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
	return renderer;
}

SDL_Texture* loadTexture(SDL_Renderer* renderer, string path, SDL_bool hasColorKey, Uint8 colorKeyR, Uint8 colorKeyG, Uint8 colorKeyB)
{
    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
		return NULL;
    }
    else
    {
		if (hasColorKey == SDL_TRUE)
			SDL_SetColorKey(loadedSurface, hasColorKey, SDL_MapRGB(loadedSurface->format, colorKeyR, colorKeyG, colorKeyB));


        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
			return NULL;
        }

		SDL_SetTextureBlendMode( newTexture, SDL_BLENDMODE_BLEND );
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return newTexture;
}

TTF_Font* loadFont(string path){
    TTF_Font* font = TTF_OpenFont(path.c_str(), 28 );
    if( font == NULL )
		return NULL;

    return font;
}

SDL_Texture* getTextTexture(SDL_Renderer* renderer, TTF_Font* font, string text, SDL_Color color){
    //Render text surface
    SDL_Surface* textSurface = TTF_RenderUTF8_Solid(font, text.c_str(), color);
    if( textSurface == NULL )
		return NULL;

	//Create texture from surface pixels
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface( renderer, textSurface );
	if( textTexture == NULL )
		return NULL;

	//Get rid of old surface
	SDL_FreeSurface( textSurface );
    
    //Return success
    return textTexture;
}

void renderTexture(SDL_Renderer* renderer, SDL_Texture* texture) {
	SDL_RenderCopyEx(renderer, texture, NULL, NULL, 0, NULL, SDL_FLIP_NONE);
}

void waitEvent(SDL_Renderer* renderer, SDL_Texture* textTexture, SDL_Texture* defaultImage, SDL_Texture* upImage){
	bool isQuit = false;

	while(!isQuit) {
		SDL_Event event;
		while(SDL_PollEvent(&event) != 0){
			if(event.type == SDL_QUIT){
				isQuit = true;
			}
			else if(event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_UP) {
					SDL_RenderClear(renderer);
					renderTexture(renderer, defaultImage);

					SDL_SetTextureAlphaMod( upImage, 0x33 );
					renderTexture(renderer, upImage);

					renderTexture(renderer, textTexture);
					SDL_RenderPresent(renderer);
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

	if( TTF_Init() == -1 )
	{
		printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
		return -1;
	}

	SDL_Window* window = createWindow();
	if (window == NULL){
		printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		return -1;
	}

	SDL_Renderer* renderer = createRenderer(window);
	if (renderer == NULL) {
		printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
		return -1;
	}

	TTF_Font* font = loadFont("SweiB2SugarCJKtc-Black.ttf");
	if( font == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        return -1;
    }
	
	SDL_Texture* textTexture = getTextTexture(renderer, font, "悠太翼", SDL_Color {0x00, 0x00, 0x00});
	if( textTexture == NULL )
    {
        printf( "Unable to get text! SDL Error: %s\n", SDL_GetError() );
        return -1;
    }

	SDL_Texture* defaultImage = loadTexture(renderer, DEFAULT_IMAGE_PATH, SDL_FALSE, 0, 0, 0);
	if( defaultImage == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", DEFAULT_IMAGE_PATH.c_str(), SDL_GetError() );
        return -1;
    }

	SDL_Texture* upImage = loadTexture(renderer, UP_IMAGE_PATH, SDL_TRUE, 0xFF, 0xFF, 0xFF);
	if( upImage == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", UP_IMAGE_PATH.c_str(), SDL_GetError() );
        return -1;
    }
	
	SDL_RenderClear(renderer);
	renderTexture(renderer, defaultImage);
	SDL_RenderPresent(renderer);

	waitEvent(renderer, textTexture, defaultImage, upImage);

	SDL_DestroyTexture(defaultImage);
	SDL_DestroyTexture(upImage);
	SDL_DestroyTexture(textTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();

	return 0;
}