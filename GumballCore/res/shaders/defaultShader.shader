#shader vertex
#version 330 core
layout(location = 0)in vec4 pos;
void main() {
    gl_Position = pos;
};

//this is a comment
#shader fragment
#version 330 core
//layout(location = 0)out vec4 color;
uniform vec4 uColor;
void main() {
    //gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    gl_FragColor = uColor;
};