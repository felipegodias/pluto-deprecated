#set state shared
#version 330 core

struct MaterialData
{
    vec4 color;
};

#set state vertex
layout (location = 0) in vec3 vertexPosition;
uniform mat4 u_mvp;

void main()
{
    gl_Position = u_mvp * vec4(vertexPosition, 1);
}

#set state fragment
uniform MaterialData u_mat;

out vec4 outColor;

void main()
{
    outColor = u_mat.color;
}