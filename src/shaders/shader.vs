#version 330 core
layout (location = 0) in vec3 aPos; //this position has an attribute position 0
layout (location = 1) in vec3 aColor; //color has atribute position 1

out vec3 ourColor;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    ourColor = aColor; //set ourColor to input color from the vertex data
}