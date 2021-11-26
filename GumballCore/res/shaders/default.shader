#vert
#version 330 core

in vec4 pos;
in float value;
void main(){
    gl_Position = pos + value;
};


#frag
#version 330 core

uniform vec4 color = vec4(1, 1, 1, 1);
void main(){
    gl_FragColor = color;
    
};