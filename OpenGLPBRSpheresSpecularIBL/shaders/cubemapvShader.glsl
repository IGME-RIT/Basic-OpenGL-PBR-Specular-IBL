

#version 430 core

layout(location = 0) in vec3 in_position;

uniform mat4 view;
uniform mat4 proj;

out vec3 localPos;


void main()
{
    localPos=in_position;

    gl_Position =  proj * view * vec4(localPos, 1.0);

}  