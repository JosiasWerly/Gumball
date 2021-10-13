#vert
#version 330 core
layout (location = 0) in vec4 pos;
out vec4 vColor;
void main(){
    vColor = vec4(sin(pos.x), cos(pos.y), 0.0, 1.0);
    gl_Position = pos;
};


#frag
#version 330 core
uniform float time;
in vec4 vColor;		
void main(){
    gl_FragColor = vec4(sin(gl_FragCoord.x), cos(gl_FragCoord.y), 0, 1.0);
};