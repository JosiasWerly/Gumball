#shader vertex
#version 330 core

layout(location = 0)in vec4 pos;
layout(location = 1)in vec2 texCoord;

out vec2 vTexture;
void main() {
    gl_Position = pos;
    vTexture = texCoord;
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec2 vTexture;
uniform vec4 uColor;
uniform sampler2D uTexture;
void main() {
    vec4 texColor = texture(uTexture, vTexture);
    color = texColor;
};