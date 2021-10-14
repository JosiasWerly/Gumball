#vert
#version 330 core
in vec4 pos;
void main(){
    //_color = col;
    gl_Position = pos;
};


#frag
#version 330 core

uniform vec4 color = vec4(1, 1, 1, 1);
void main(){
    gl_FragColor = color;
};