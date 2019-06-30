#set state shared
#version 330 core

struct VertexData
{
    vec3 position;
};

struct MaterialData
{
    vec4 color;
};

#set state vertex
in VertexData vertex;

void main()
{
    gl_Position = vec4(vertex.position, 0);
}

#set state fragment
uniform MaterialData mat;

out vec4 outColor;

void main()
{
    outColor = mat.color;
}