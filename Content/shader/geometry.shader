#vert
#version 440 core

in vec3 pos;
in vec3 normal;
in vec2 uv;

out vec3 fPos;
out vec3 fNormal;
out vec2 fUV;

uniform mat4 uProj, uView, uModel;

void main(){
	fNormal = normal;
    fUV = uv;
    gl_Position = uProj * uView * uModel * vec4(pos, 1);
    fPos = gl_Position.xyz;
};


#frag
#version 440 core

in vec3 fPos;
in vec3 fNormal;
in vec2 fUV;

out vec4 gAlbedo;
out vec3 gPosition;
out vec3 gNormal;

uniform vec4 uColor;
uniform sampler2D uTexture;

void main(){
    gPosition = fPos;
    gNormal = normalize(fNormal);
    gAlbedo = texture(uTexture, fUV) * uColor;
};