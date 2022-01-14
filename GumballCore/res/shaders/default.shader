#vert
#version 440 core

in vec4 pos;
in vec2 texCoord;

out vec2 fTexCoord;

uniform mat4 uProj, uView, uModel;
void main(){
    fTexCoord = texCoord;
    gl_Position = uProj * uView * uModel * pos;
};


#frag
#version 440 core

in vec2 fTexCoord;

uniform vec4 uColor;
uniform sampler2D uTexture;

out vec4 gl_FragColor;
void main(){
    gl_FragColor = texture(uTexture, fTexCoord) * uColor;
};