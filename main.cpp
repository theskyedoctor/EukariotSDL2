#include "headers/LUtil.h"
#define STB_IMAGE_IMPLEMENTATION
#include "headers/stb_image.h"

#include "headers/shader.h"

//Graphics program
int gVertexPos2DLocation = -1;
unsigned int gVBO;
unsigned int gEBO = 0;
unsigned int gVAO;
unsigned int texture;


//starts up SDL and creates window
bool init();

//frees media and shuts down SDL
void close();

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

        Shader ourShader("../src/shaders/shader.vs", "../src/shaders/shader.fs");

        //Initialize OpenGL
        initGL();

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

            update();

            //Initialize clear color
            glClearColor( 0.2f, 0.3f, 0.3f, 1.f );
            glClear(GL_COLOR_BUFFER_BIT);



            //glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);

            ourShader.use();

            //draw first tri
            glBindVertexArray(gVAO);
            glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );

            //unbind program
            glUseProgram( NULL );

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

void initGL()
{

    //check available attributes
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    if (DEBUG)
    {
        printf("Maximum nr of vertex attributes supported: %d", nrAttributes);
    }
    //VBO data
    float vertices[] =
    {
        //position     //color        //texture coords
        0.5f, 0.5, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f, //top right
        0.5f, -0.5f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, //bottom right
        -0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, // bottom left
        -0.5f, 0.5f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f // top left

    };


    //eBO data
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    //create VAO
    glGenVertexArrays(1, &gVAO);

    //create VBO
    glGenBuffers( 1, &gVBO );

    //create EBO
    glGenBuffers( 1, &gEBO );

    //first quad
    glBindVertexArray( gVAO );
    glBindBuffer( GL_ARRAY_BUFFER, gVBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), NULL );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), (void*) (3* sizeof(float)));
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), (void*) (6 * sizeof(float)));
    glEnableVertexAttribArray( 2 );


    //texture setup
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    //set texture wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //load and generate the texture
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("../src/textures/eukariot.png", &width, &height, &nrChannels,0);
    if ( data )
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

    }
    else
    {
        printf("Failed to load texture \n");
    }

    stbi_image_free(data);
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
}

void update()
{
    //space intentionally left blank
}

void handleKeys( unsigned char key, int x, int y )
{

}

void printProgramLog( GLuint program )
{
    //make sure name is shader
    if ( glIsProgram( program ) )
    {
        //Program log length
        int infoLogLength = 0;
        int maxLength = infoLogLength;

        //get info string length
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength );

        //allocate string
        char* infoLog = new char[ maxLength ];

        //get info log
        glGetProgramInfoLog( program, maxLength, &infoLogLength, infoLog );
        if ( infoLogLength > 0 )
        {
            //print Log
            printf( "%s\n", infoLog );
        }

        //deallocate string
        delete[] infoLog;
    }
    else
    {
        printf( "Name %d is not a program\n", program );
    }
}

void printShaderLog( GLuint shader )
{
    //make sure name is shader
    if ( glIsShader( shader ) )
    {
        //Shader log length
        int infoLogLength = 0;
        int maxLength = infoLogLength;

        //Get info string length
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );

        //Allocate string
        char* infoLog = new char[ maxLength ];

        //Get info log
        glGetShaderInfoLog( shader, maxLength, &infoLogLength, infoLog );
        if( infoLogLength > 0 )
        {
            //Print Log
            printf( "%s\n", infoLog );
        }

        //Deallocate string
        delete[] infoLog;
    }
    else
    {
        printf( "Name %d is not a shader\n", shader );
    }
}