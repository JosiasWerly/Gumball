#vert
#version 440 core

in vec3 pos;
in vec3 normal;
in vec2 uv;

out vec2 f_uv;
out vec3 f_normal;

void main() {
    f_normal = normal;
    f_uv = uv;
    gl_Position = vec4(pos, 1);
}


#frag
#version 440 core

in vec2 f_uv;
in vec3 f_normal;

out vec4 fColor;

uniform sampler2D uTexture;

void main() {
    fColor  =  texture(uTexture, f_uv);
    //fColor = vec4(f_uv.x, f_uv.y, f_uv.x + f_uv.y, 1);    
    //fColor  =  texture(uTexture, f_uv) + vec4(f_uv.x, f_uv.y, f_uv.x + f_uv.y, 1);
}