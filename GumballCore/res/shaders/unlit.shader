#shader vertex
#version 330 core
in vec4 pos;
in vec4 normal;
in vec2 uv;


out vec3 fNormal;
uniform mat4 uProj, uView, uModel;
void main() {
    fNormal = normal.xyz;
    gl_Position = uProj * uView * uModel * pos;
};


#shader fragment
#version 330 core
out vec4 color;
in vec3 fNormal;
uniform vec4 uColor;

void main() {
    float diffuse = dot(fNormal, vec3(0, 0, 1));
    color = diffuse * uColor;
    //color = diffuse * vec4(1, 1, 1, 1);
};