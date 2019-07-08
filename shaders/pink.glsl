#set state shared
#version 330 core

struct VertexData
{
    vec3 position;
};

#set state vertex
in VertexData vertex;

void main()
{
    gl_Position = vec4(vertex.position, 0);
}

#set state fragment

out vec4 outColor;

void main()
{
    outColor = vec4(1, 0, 1, 1);
}