//
// Created by skyehigh on 3/27/26.
//

#ifndef EUKARIOTSDL2_SHADER_H
#define EUKARIOTSDL2_SHADER_H

#include "LUtil.h"

class Shader
{
public:
    //program ID
    unsigned int ID;

    Shader( const char* vertexPath, const char* fragmentPath )
    {
        //1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        //ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions( std::ifstream::failbit | std::ifstream::badbit );
        fShaderFile.exceptions( std::ifstream::failbit | std::ifstream::badbit );
        try
        {
            //open files
            vShaderFile.open( vertexPath );
            fShaderFile.open( fragmentPath );
            std::stringstream vShaderStream, fShaderStream;
            //read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            printf("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n, %s", e.what());
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        //2. compile shaders
        int success = 0;
        char infoLog[512];

        //vertex shader
        GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        //print compile errors if any
        glGetShaderiv( vertex, GL_COMPILE_STATUS, &success );
        if (!success) {
            glGetShaderInfoLog( vertex, 512, NULL, infoLog );
            printf( "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n %s \n", infoLog );
        };
        //similar for fragment shader
        GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        glGetShaderiv( fragment, GL_COMPILE_STATUS, &success );
        if (!success) {
            glGetShaderInfoLog( fragment, 512, NULL, infoLog );
            printf( "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n %s \n", infoLog );
        };

        //shader program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        //check for linking errors
        glGetProgramiv( ID, GL_LINK_STATUS, &success );
        if (!success) {
            glGetProgramInfoLog( ID, 512, NULL, infoLog );
            printf( "ERROR::SHADER::PROGRAM::LINKING_FAILED\n %s \n", infoLog );
        };

        //delete shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void use() {
        glUseProgram(ID);
    }

    void setBool( const std::string &name, bool value ) const{
        glUniform1i( glGetUniformLocation( ID, name.c_str() ), (int)value );
    }
    void setInt( const std::string &name, int value ) const{
        glUniform1i( glGetUniformLocation( ID, name.c_str() ), value );
    }
    void setFloat( const std::string &name, float value ) const {
        glUniform1f( glGetUniformLocation( ID, name.c_str() ), value );
    }
};



#endif //EUKARIOTSDL2_SHADER_H