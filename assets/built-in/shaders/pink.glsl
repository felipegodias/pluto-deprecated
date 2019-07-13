#set state shared
#version 330 core

#set state vertex
layout (location = 0) in vec3 vertexPosition;
uniform mat4 u_mvp;

void main()
{
    gl_Position = u_mvp * vec4(vertexPosition, 1);
}

#set state fragment
out vec4 outColor;

void main()
{
    outColor = vec4(1, 0, 1, 1);
}