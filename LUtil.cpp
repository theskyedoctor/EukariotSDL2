//
// Created by skyehigh on 3/6/26.
//

#include "headers/LUtil.h"

int gViewportMode = VIEWPORT_MODE_FULL;
GLfloat gCameraX = 0.f, gCameraY = 0.f;

//Graphics program
GLuint gProgramID = 0;
GLint gVertexPos2DLocation = -1;
GLuint gVBO = 0;
GLuint gIBO = 0;

bool initGL()
{
    bool success = true;

    //generate program
    gProgramID = glCreateProgram();

    //create vertex shader
    GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );

    //get vertex source
    const GLchar* vertexShaderSource[] =
    {
        "#version 140\nin vec2 LVertexPos2D; void main() { gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 );}"
    };

    //set vertyex source
    glShaderSource( vertexShader, 1, vertexShaderSource, NULL );

    //compile vertex source
    glCompileShader( vertexShader );

    //check vertex shader for errors
    GLint vShaderCompiled = GL_FALSE;
    glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &vShaderCompiled );
    if ( vShaderCompiled != GL_TRUE )
    {
        printf( "Unable to compile vertex shader %d!\n", vertexShader );
        printShaderLog( vertexShader );
        success = false;
    }
    else
    {
        //attach vertex shader
        glAttachShader( gProgramID, vertexShader );

        //create fragment shader
        GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

        //get fragment source
        const GLchar* fragmentShaderSource[] =
        {
            "#version 140\nout vec4 LFragment; void main() { LFragment = vec4( 1.0, 1.0, 1.0, 1.0 ); }"
        };

        //set fragment source
        glShaderSource( fragmentShader, 1, fragmentShaderSource, NULL );

        //compile fragment source
        glCompileShader( fragmentShader );

        //check fragment shader for errors
        GLint fShaderCompiled = GL_FALSE;
        glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled );
        if ( fShaderCompiled != GL_TRUE )
        {
            printf( "Unable to compile fragment shader %d!\n", fragmentShader );
            printShaderLog( fragmentShader );
            success = false;
        }
        else
        {
            //attach fragment shader to program
            glAttachShader( gProgramID, fragmentShader );

            //link program
            glLinkProgram( gProgramID );

            //check for errors
            GLint programSuccess = GL_TRUE;
            glGetProgramiv( gProgramID, GL_LINK_STATUS, &programSuccess );
            if ( programSuccess != GL_TRUE )
            {
                printf( "Error linking program %d!\n", gProgramID );
                printProgramLog( gProgramID );
                success = false;
            }
            else
            {
                //get vertex attribute location
                gVertexPos2DLocation = glGetAttribLocation( gProgramID, "LVertexPos2D" );
                if ( gVertexPos2DLocation == -1 )
                {
                    printf(  "LVertexPos2D is not a valid glsl program variable!\n" );
                    success = false;
                }
                else
                {
                    //Initialize clear color
                    glClearColor( 0.f, 0.f, 0.f, 1.f );

                    //VBO data
                    GLfloat vertexData[] =
                    {
                        -0.5f, -0.5f,
                        0.5f, -0.5f,
                        0.5f, 0.5f,
                        -0.5f, 0.5f,
                    };

                    //IBO data
                    GLuint indexData[] = { 0, 1, 2, 3 };

                    //create VBO
                    glGenBuffers( 1, &gVBO );
                    glBindBuffer( GL_ARRAY_BUFFER, gVBO );
                    glBufferData( GL_ARRAY_BUFFER, 2 * 4 * sizeof( GLfloat ), vertexData, GL_STATIC_DRAW );

                    //create IBO
                    glGenBuffers( 1, &gIBO );
                    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIBO );
                    glBufferData( GL_ELEMENT_ARRAY_BUFFER, 4* sizeof( GLuint ), indexData, GL_STATIC_DRAW );
                }
            }
        }
    }
    return success;
}

bool loadMedia()
{
    //checkerboard pixels
}

void update()
{
    //space intentionally left blank
}

void render()
{
    //bind program
    glUseProgram( gProgramID );

    //Enable vertex position
    glEnableVertexAttribArray( gVertexPos2DLocation );

    //set vertex data
    glBindBuffer( GL_ARRAY_BUFFER, gVBO );
    glVertexAttribPointer( gVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof( GLfloat ), NULL );

    //set index data and render
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIBO );
    glDrawElements( GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL );

    //disable vertex position
    glDisableVertexAttribArray( gVertexPos2DLocation );

    //unbind program
    glUseProgram( NULL );
}

void handleKeys( unsigned char key, int x, int y )
{
    //toggle quad color
    if (key == 'q' )
    {
        //cycle through viewport modes
        gViewportMode++;
        if ( gViewportMode > VIEWPORT_MODE_RADAR )
        {
            gViewportMode = VIEWPORT_MODE_FULL;
        }
    }
    if ( key == 'w' )
    {
        gCameraY -= 16.f;
    }
    if ( key == 's' )
    {
        gCameraY += 16.f;
    }
    if ( key == 'a' )
    {
        gCameraX -= 16.f;
    }
    if ( key == 'd' )
    {
        gCameraX += 16.f;
    }

    //take saved matrix off the stack and reset it
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();
    glLoadIdentity();

    //move camera to position
    glTranslatef( -gCameraX, -gCameraY, 0.f );

    //save default matrix again this time with camera translation
    glPushMatrix();
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