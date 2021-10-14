#vert
#version 330 core
in vec4 pos;

in float test;
void main(){
    gl_Position = pos + vec4(0, test, 0, 0);
};


#frag
#version 330 core

uniform vec4 color = vec4(1, 1, 1, 1);
void main(){
    gl_FragColor = color;
    
};