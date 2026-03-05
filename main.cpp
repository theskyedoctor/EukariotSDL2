#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include <string>

//screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

//starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//frees media and shuts down SDL
void close();

//loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//the window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gTexture = NULL;

bool init()
{
    //initialization flag
    bool success = true;

    //initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf(  "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }
    else
    {
        //set texture filtering to linear
        if ( !SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf(  "Warning: Linear texture filtering is not enabled!" );
        }

        //create window
        gWindow = SDL_CreateWindow( "Eukariot", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //create renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
            if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL_Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //initialize render color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

                //initialize PBG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
            }
        }
    }
    return success;
}

bool loadMedia()
{
    //loading success flag
    bool success = true;

    //load png texture
    gTexture = loadTexture( "../src/eukariot.png" );
    if ( gTexture == NULL )
    {
        printf( "failted to load texture image!\n" );
        success = false;
    }
    return success;
}

void close()
{
    //free loaded image
    SDL_DestroyTexture( gTexture );
    gTexture = NULL;

    //destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture* loadTexture( std::string path )
{
    //the final texture
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf(  "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if ( newTexture == NULL )
        {
            printf("Unable to create texture form %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        //get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    return newTexture;
}

int main( int argc, char* args[] )
{
    //start up SDL and create window
    if ( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        if ( !loadMedia() )
        {
            printf( "Failed to load media!\n");
        }
        else
        {
            //main loop flag
            bool quit = false;

            //event handler
            SDL_Event e;

            while ( !quit )
            {
                //handle events on queue
                while ( SDL_PollEvent( &e) != 0 )
                {
                    if ( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                }

                //clear screen
                SDL_RenderClear( gRenderer );

                //render texture to screen
                SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );

                //update screen
                SDL_RenderPresent( gRenderer );

            }
        }
    }


    close();

    return 0;
}