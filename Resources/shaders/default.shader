#vert
#version 440 core

in vec3 pos;
in vec3 normal;
in vec2 uv;

out vec2 f_uv;
out vec3 f_normal;

uniform mat4 uProj, uView, uModel;

void main(){
	f_normal = normal;
    f_uv = uv;
    gl_Position = uProj * uView * uModel * vec4(pos, 1);
};


#frag
#version 440 core

in vec2 f_uv;
in vec3 f_normal;

out vec4 fColor;

uniform vec4 uColor;
uniform sampler2D uTexture;

void main(){
    fColor = texture(uTexture, f_uv) * uColor;
};