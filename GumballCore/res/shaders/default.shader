#shader vertex
#version 330 core
in vec4 pos;
uniform mat4 uProj, uView, uModel;
void main() {
    gl_Position = uProj * uView * uModel * pos;
};


#shader fragment
#version 330 core

out vec4 color;
uniform vec4 uColor;
void main() {
    color =  uColor;
};