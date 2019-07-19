#set state shared
#version 330 core

struct MaterialData
{
    sampler2D mainTex;
};

struct V2F
{
    vec2 texCoord;
};

#set state vertex
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexUv;

uniform mat4 u_mvp;

out V2F v2f;

void main()
{
    gl_Position = u_mvp * vec4(vertexPosition, 1);
    v2f.texCoord = vertexUv;
}

#set state fragment
uniform MaterialData u_mat;

in V2F v2f;

out vec4 outColor;

void main()
{
    outColor = texture(u_mat.mainTex, v2f.texCoord);
}