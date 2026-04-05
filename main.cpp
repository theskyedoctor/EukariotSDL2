#include "headers/LUtil.h"
#define STB_IMAGE_IMPLEMENTATION
#include <ranges>

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

                //Initialize OpenGL
                initGL();
                SDL_SetRelativeMouseMode(SDL_TRUE);
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
        bool mouseFirst = true;

        //event handler
        SDL_Event e;

        //enable text input
        SDL_StartTextInput();

        //create shader
        Shader ourShader("../src/shaders/shader.vs", "../src/shaders/shader.fs");

        glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
        };

        //camera
        glm::vec3 cameraPos = glm::vec3(0.f, 0.f, 3.f);
        glm::vec3 cameraFront = glm::vec3(0.f, 0.f, -1.f);
        glm::vec3 cameraUp = glm::vec3(0.f, 1.f, 0.f);

        float lastFrame, deltaTime = 0.f;

        //while application is running
        while ( !quit )
        {
            float currentFrame = SDL_GetTicks() / 1000.f;
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            float cameraSpeed = 2.5f * deltaTime;

            //Handle events on queue
            while ( SDL_PollEvent( &e ) != 0 )
            {
                if(e.type == SDL_QUIT){quit = true;}
            }

            const Uint8* keystates = SDL_GetKeyboardState(NULL);
            if (keystates[SDL_SCANCODE_ESCAPE])
            {
                quit = true;
            }
            if(keystates[SDL_SCANCODE_W]){
                cameraPos += cameraFront * cameraSpeed;
            }
            if(keystates[SDL_SCANCODE_S]){
                cameraPos -= cameraFront * cameraSpeed;
            }
            if(keystates[SDL_SCANCODE_D]){
                cameraPos += glm::normalize(glm::cross(cameraFront ,cameraUp )) * cameraSpeed;
            }
            if(keystates[SDL_SCANCODE_A]){
                cameraPos -= glm::normalize(glm::cross(cameraFront ,cameraUp)) * cameraSpeed;
            }

            update();

            glm::mat4 view = glm::mat4(1.f);
            view = glm::lookAt(cameraPos, cameraPos+cameraFront, cameraUp);

            glm::mat4 projection = glm::mat4(1.f);
            projection = glm::perspective(glm::radians(45.f), (float)(SCREEN_WIDTH) / (float)(SCREEN_HEIGHT), 0.1f, 100.f);

            //Initialize clear color
            glClearColor( 0.2f, 0.3f, 0.3f, 1.f );
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



            //glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);

            ourShader.use();



            ourShader.setMat4("projection", projection);
            ourShader.setMat4("view", view);

            //draw first tri
            glBindVertexArray(gVAO);

            for (unsigned int i = 0; i <10; i++)
            {
                glm::mat4 model = glm::mat4(1.f);
                model = glm::translate(model, cubePositions[i]);
                float angle = (10.f * i) + 10.f;
                model = glm::rotate(model, glm::radians(angle)*(SDL_GetTicks()/1000.f), glm::vec3(1.f, 0.5f, 0.1f));
                ourShader.setMat4("model", model);

                glDrawArrays( GL_TRIANGLES, 0, 36 );
            }

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
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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

    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), NULL );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), (void*) (3* sizeof(float)));
    glEnableVertexAttribArray( 1 );


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
    unsigned char *data = stbi_load("../src/textures/Guidedcatgames.png", &width, &height, &nrChannels,0);
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
    glEnable(GL_DEPTH_TEST);
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