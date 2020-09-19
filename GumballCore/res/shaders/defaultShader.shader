#shader vertex
#version 330 core
layout(location = 0)in vec4 pos;
void main() {
    gl_Position = pos;
};


#shader fragment
#version 330 core
uniform vec4 uColor;
uniform vec2 uParam;
void main() {
    gl_FragColor = uColor;
};