#vert
#version 330 core

in vec4 pos;
in vec2 texCoord;

out vec2 u_texCoord;


void main(){
    gl_Position = pos + value;
};


#frag
#version 330 core

in vec2 u_texCoord;

//uniform vec4 u_color = vec4(1, 1, 1, 1);
uniform sampler2D u_texture;

void main(){
    gl_FragColor = texture(u_texture, u_texCoord);
};