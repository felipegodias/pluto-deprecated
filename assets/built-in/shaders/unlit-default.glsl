#version 330 core

#define PLUTO_BLEND SRC_ALPHA ONE_MINUS_SRC_ALPHA
#define PLUTO_BLEND_EQUATION ADD

struct MaterialData
{
    sampler2D mainTex;
    vec4 mainColor;
};

uniform MaterialData u_mat;

struct V2F
{
    vec2 texCoord;
    vec4 color;
};

#ifdef PLUTO_VERTEX_SHADER

struct VertexData
{
    vec3 pos;
    vec2 uv;
};

in VertexData vertex;

uniform mat4 u_mvp;

out V2F v2f;

void main()
{
    gl_Position = u_mvp * vec4(vertex.pos, 1);
    v2f.texCoord = vertex.uv;
    v2f.color = u_mat.mainColor;
}

#endif

#ifdef PLUTO_FRAGMENT_SHADER
in V2F v2f;

out vec4 outColor;

void main()
{
    outColor = texture(u_mat.mainTex, v2f.texCoord) * v2f.color;
}
#endif