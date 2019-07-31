#version 330 core

#ifdef PLUTO_VERTEX_SHADER

struct VertexData
{
    vec3 pos;
};

in VertexData vertex;

struct AppData {
    mat4 mvp;
};

uniform AppData app;

void main()
{
    gl_Position = app.mvp * vec4(vertex.pos, 1);
}

#endif

#ifdef PLUTO_FRAGMENT_SHADER

struct MaterialData 
{
    vec4 color;
};

uniform MaterialData material;

out vec4 outColor;

void main()
{
    outColor = material.color;
}

#endif