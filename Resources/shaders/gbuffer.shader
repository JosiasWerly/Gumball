#vert
#version 440 core

in vec3 pos;
in vec3 normal;
in vec2 uv;

out vec2 fUV;
out vec3 fNormal;

void main() {    
    fUV = uv;
    fNormal = normal;
    gl_Position = vec4(pos, 1);
}


#frag
#version 440 core

in vec2 fUV;
in vec3 fNormal;

out vec4 fColor;

uniform sampler2D gAlbedo;
uniform sampler2D gPosition;
uniform sampler2D gNormal;

void main() {
    fColor  =  texture(gAlbedo, fUV);
}