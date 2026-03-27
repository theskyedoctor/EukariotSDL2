//
// Created by skyehigh on 3/6/26.
//

#include "headers/LUtil.h"

//Graphics program
GLuint shaderProgram = 0;
GLint gVertexPos2DLocation = -1;
GLuint gVBOs[2];
GLuint gIBO = 0;
GLuint gVAOs[2];


bool initGL()
{
    bool success = true;
    //check available attributes
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    if (DEBUG)
    {
        printf("Maximum nr of vertex attributes supported: %d", nrAttributes);
    }

    shaderProgram = glCreateProgram();


    //create vertex shader
    GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );

    //get vertex source

    const GLchar* vertexShaderSource[] =
    {
        "#version 330 core\n layout (location = 0) in vec3 LVertexPos2D; layout (location = 1) in vec3 aColor; out vec3 ourColor; void main() { gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, LVertexPos2D.z, 1 ); ourColor = aColor;}"
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
        //create fragment shader
        GLuint fragmentShaderWhite = glCreateShader( GL_FRAGMENT_SHADER );

        //get fragment source
        const GLchar* fragmentShaderWhiteSource[] =
        {
            "#version 330 core\nout vec4 LFragment; in vec3 ourColor; void main() { LFragment = vec4(ourColor, 1.f); }"
        };

        //set fragment source and compile
        glShaderSource( fragmentShaderWhite, 1, fragmentShaderWhiteSource, NULL );
        glCompileShader( fragmentShaderWhite );


        //check fragment shader for errors
        GLint fShader1Compiled = GL_FALSE;
        glGetShaderiv( fragmentShaderWhite, GL_COMPILE_STATUS, &fShader1Compiled );
        if ( (fShader1Compiled) != GL_TRUE )
        {
            printf( "Unable to compile fragment shader %d!\n", fragmentShaderWhite );
            printShaderLog( fragmentShaderWhite );
            success = false;
        }
        else
        {


            glAttachShader(shaderProgram, vertexShader);
            glAttachShader(shaderProgram, fragmentShaderWhite);
            glLinkProgram( shaderProgram );


            //check for errors
            GLint program1Success = GL_TRUE;
            glGetProgramiv( shaderProgram, GL_LINK_STATUS, &program1Success );
            if ( (program1Success ) != GL_TRUE )
            {
                printf( "Error linking program!\n");
                printProgramLog( shaderProgram );
                success = false;
            }
            else
            {
                //VBO data
                GLfloat firstTriangle[] =
                {
                    -0.9f, -0.5f, 0.f, 1.f, 0.f, 0.f, //left
                    0.f, -0.5f, 0.f, 0.f, 1.f, 0.f, //right
                    -0.45f, 0.5f, 0.f, 0.f, 0.f, 1.f //top
                };

                GLfloat secondTriangle[] =
                {
                    0.f, -0.5f, 0.f, 0.f, 0.f, 1.f, //left
                    0.9f, -0.5f, 0.f, 1.f, 0.f, 0.f, //right
                    0.45f, 0.5f, 0.f, 0.f, 1.f, 0.f //top
                };

                //IBO data
                //GLuint indexData[] = { 0, 1, 2 };

                //create VAO
                glGenVertexArrays(2, gVAOs);

                //create VBO
                glGenBuffers( 2, gVBOs );

                /*create IBO
                glGenBuffers( 1, &gIBO );
                glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIBO );
                glBufferData( GL_ELEMENT_ARRAY_BUFFER, 4* sizeof( GLuint ), indexData, GL_STATIC_DRAW );
                */

                //first tri
                glBindVertexArray( gVAOs[0] );
                glBindBuffer( GL_ARRAY_BUFFER, gVBOs[0] );
                glBufferData( GL_ARRAY_BUFFER, sizeof( firstTriangle ), firstTriangle, GL_STATIC_DRAW );
                glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( GLfloat ), NULL );
                glEnableVertexAttribArray( 0 );
                glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( GLfloat ), (void*) (3* sizeof(GLfloat)));
                glEnableVertexAttribArray( 1 );

                //second tri
                glBindVertexArray( gVAOs[1] );
                glBindBuffer( GL_ARRAY_BUFFER, gVBOs[1] );
                glBufferData( GL_ARRAY_BUFFER, sizeof( secondTriangle ), secondTriangle, GL_STATIC_DRAW );
                glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), NULL );
                glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( GLfloat ), NULL );
                glEnableVertexAttribArray( 0 );
                glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( GLfloat ), (void*) (3* sizeof(GLfloat)));
                glEnableVertexAttribArray( 1 );

                //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
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

void render( float timeValue )
{
    float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    //int vertexColorLocation = glGetUniformLocation( shaderProgram, "ourColor" );
    //Initialize clear color
    glClearColor( 0.2f, 0.3f, 0.3f, 1.f );
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram( shaderProgram );

    //draw first tri
    glBindVertexArray(gVAOs[0]);
    glDrawArrays( GL_TRIANGLES, 0, 3 );

    //draw second tri
    glBindVertexArray(gVAOs[1]);
    glDrawArrays( GL_TRIANGLES, 0, 3 );


    //unbind program
    glUseProgram( NULL );
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