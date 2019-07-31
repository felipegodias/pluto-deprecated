#version 330 core

#ifdef PLUTO_VERTEX_SHADER

struct VertexData
{
    vec3 pos;
};

in VertexData vertex;
uniform mat4 u_mvp;

void main()
{
    gl_Position = u_mvp * vec4(vertex.pos, 1);
}

#endif

#ifdef PLUTO_FRAGMENT_SHADER

out vec4 outColor;

void main()
{
    outColor = vec4(1, 0, 1, 1);
}

#endif