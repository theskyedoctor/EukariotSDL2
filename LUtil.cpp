//
// Created by skyehigh on 3/6/26.
//

#include "headers/LUtil.h"
#include "headers/shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "headers/stb_image.h"

//Graphics program
GLint gVertexPos2DLocation = -1;
GLuint gVBOs[2];
GLuint gIBO = 0;
GLuint gVAOs[2];
GLuint texture;

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
    GLfloat quad[] =
    {
        //position     //color        //texture coords
        0.5f, 0.5, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f, //top right
        0.5f, -0.5f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, //bottom right
        -0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, // bottom left
        -0.5f, 0.5f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f // top left

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

    //first quad
    glBindVertexArray( gVAOs[0] );
    glBindBuffer( GL_ARRAY_BUFFER, gVBOs[0] );
    glBufferData( GL_ARRAY_BUFFER, sizeof( quad ), quad, GL_STATIC_DRAW );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), NULL );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), (void*) (3* sizeof(GLfloat)));
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), (void*) (6 * sizeof(GLfloat)));

    //texture setup
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    //set texture wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load("../src/textures/eukariot.png", &width, &height, &nrChannels,0);
    if ( data )
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load texture \n");
    }


    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
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
    Shader ourShader("../src/shaders/shader.vs", "../src/shaders/shader.fs");

    float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    //int vertexColorLocation = glGetUniformLocation( shaderProgram, "ourColor" );
    //Initialize clear color
    glClearColor( 0.2f, 0.3f, 0.3f, 1.f );
    glClear(GL_COLOR_BUFFER_BIT);

    ourShader.use();

    //draw first tri
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(gVAOs[0]);
    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );

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