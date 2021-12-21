#vert
#version 440 core

in vec4 pos;
in vec2 texCoord;

out vec2 vTexCoord;

void main(){
    vTexCoord = texCoord;
    gl_Position = pos;
};


#frag
#version 440 core

in vec2 vTexCoord;

uniform vec4 uColor;
uniform sampler2D uTexture;

out vec4 gl_FragColor;
void main(){
    //gl_FragColor = vec4(sin(vTexCoord.x), cos(vTexCoord.y), atan(vTexCoord.x/vTexCoord.y), 0);
    gl_FragColor = texture(uTexture, vTexCoord) * uColor;
};