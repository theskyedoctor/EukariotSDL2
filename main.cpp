#include "headers/LUtil.h"

//starts up SDL and creates window
bool init();

//frees media and shuts down SDL
void close();

void runMainLoop( int val );
/*
Pre Condition:
 -Initialized SDL2
Post Condition:
 -Calls the main loop functions and sets itself to be called back in 1000 / SCREEN_FPS milliseconds
Side Effects:
 -Sets sdl timer
*/

//the window we'll be rendering to
SDL_Window* gWindow = NULL;

//openGL context
SDL_GLContext gContext;

//debug flag


bool init()
{
    //initialization flag
    bool success = true;

    //initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf(  "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //set texture filtering to linear
        if ( !SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf(  "Warning: Linear texture filtering is not enabled!" );
        }

        //openGL 2.1
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );


        //create window
        gWindow = SDL_CreateWindow( "Eukariot", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create context
            gContext = SDL_GL_CreateContext( gWindow );
            if ( gContext == NULL )
            {
                printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //initialize GLEW
                glewExperimental = GL_TRUE;
                GLenum glewError = glewInit();
                if ( glewError != GLEW_OK )
                {
                    printf( "Error initializing GLEW! %s\n", glewGetErrorString( glewError ) );
                }
                //use Vsync
                if ( SDL_GL_SetSwapInterval( 1 ) < 0 )
                {
                    printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
                }

                //Initialize OpenGL
                if ( !initGL() )
                {
                    printf( "Unable to initialize OpenGL!\n" );
                    success = false;
                }
            }
        }
    }
    return success;
}

void close()
{

    //destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    //quit SDL subsystems
    SDL_Quit();
}

void runMainLoop(int val)
{
    //frame logic

    uint32_t timeElapsed = SDL_GetTicks();

    float timeValue = timeElapsed / 1000.0f;
    update();
    render(timeValue);
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
        //main loop flag
        bool quit = false;

        //event handler
        SDL_Event e;

        //enable text input
        SDL_StartTextInput();

        //while application is running
        while ( !quit )
        {
            //Handle events on queue
            while ( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                if ( e.type == SDL_QUIT )
                {
                    quit = true;
                }
                //handle keypress with current mouse position
                else if ( e.type == SDL_TEXTINPUT )
                {
                    int x = 0, y = 0;
                    SDL_GetMouseState(&x, &y);
                    handleKeys( e.text.text[ 0 ], x, y );
                }
            }

            //run main loop
            runMainLoop(0);

            //update screen
            SDL_GL_SwapWindow( gWindow );
        }
        //disable text input
        SDL_StopTextInput();
    }
    //free resources and close SDL
    close();

    return 0;
}