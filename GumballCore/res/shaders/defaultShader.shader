#shader vertex
#version 330 core
in vec4 pos;
in vec2 texCoord;
out vec2 vTexture;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * pos;
    vTexture = texCoord;
};


#shader fragment
#version 330 core

in vec2 vTexture;
out vec4 color;

uniform vec4 uColor;
uniform sampler2D uTexture;

void main() {
    vec4 texColor = texture(uTexture, vTexture);
    color = texColor * uColor;
};