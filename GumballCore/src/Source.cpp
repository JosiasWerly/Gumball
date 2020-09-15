
#include "Gumball.h"

static int compileShader(unsigned int ShaderType, const string& code) {
    unsigned int id = glCreateShader(ShaderType);
    const char* src = code.c_str();
    glShaderSource(id, 1, &src, 0);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* msg = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, msg);

        cout << (ShaderType == GL_VERTEX_SHADER ? "vert" : "frag") << " shader failed to compile =(" << endl;
        return 0;
    }
    return id;
}
static int createShader(const string& vertexShader, const string& fragmentShader) {
    unsigned int 
        p = glCreateProgram(),
        vs = compileShader(GL_VERTEX_SHADER, vertexShader),
        fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(p, vs);
    glAttachShader(p, fs);
    glLinkProgram(p);
    glValidateProgram(p); //???

    int valid;
    glGetProgramiv(p, GL_LINK_STATUS, &valid);
    if (!valid)
        cout << "program shader error" << endl;
    glDeleteShader(vs);
    glDeleteShader(fs);
    return p;
}



#include <fstream>
#include <sstream>
struct ShaderSource{
    string vertex, fragment;
};
static ShaderSource loadShaderCode(string filePath) {
    ifstream fileStream(filePath);
    string line;
    bool a = false;
    ShaderSource out;
    string outString[2];
    enum eShaderType {
        none = -1, vertex, fragment
    } eShType;
    while (getline(fileStream, line)) {
        if (line.find("#shader vertex") != string::npos)
            eShType = eShaderType::vertex;
        else if (line.find("#shader fragment") != string::npos)
            eShType = eShaderType::fragment;
        else if(line.find("//") == string::npos)
            outString[eShType] += line + "\n";
    }
    return { outString[eShaderType::vertex], outString[eShaderType::fragment] };
}


int main() {
    Window w;
    w.setup("gumball", 800, 600);
    
    

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };
    
    auto shaderCode = loadShaderCode("defaultShader.shader");
    unsigned int shader = createShader(shaderCode.vertex, shaderCode.fragment);
    glUseProgram(shader);



    unsigned int vb = 0, va = 0;
    glGenVertexArrays(1, &va);
    glBindVertexArray(va);

    
    glGenBuffers(1, &vb);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0); //this is weird
    glBindVertexArray(0); // clear the stack ?
    

    glUseProgram(shader);
    while (true){
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glBindVertexArray(va);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        w.clear();

    }

    glDeleteVertexArrays(1, &va);
    glDeleteBuffers(1, &vb);
    glDeleteProgram(shader);
    return 0;
}




/*
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>


static const struct
{
    float x, y;
    float r, g, b;
} vertices[3] =
{
    { -0.6f, -0.4f, 1.f, 0.f, 0.f },
    {  0.6f, -0.4f, 0.f, 1.f, 0.f },
    {   0.f,  0.6f, 0.f, 0.f, 1.f }
};

static const char* vertex_shader_text =
"#version 110\n"
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec2 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"#version 110\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(void)
{
    GLFWwindow* window;
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vcol_location;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(1);

    // NOTE: OpenGL error checks have been omitted for brevity

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
        sizeof(vertices[0]), (void*)0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
        sizeof(vertices[0]), (void*)(sizeof(float) * 2));

    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        glm::mat4x4 m, p, mvp;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        m = glm::mat4x4(1);
        m = glm::rotate(m, glm::radians((float)glfwGetTime()*10.f), glm::vec3(0.0f, 0.0f, 1.0f));
        p = glm::ortho(-ratio, ratio, -1.0f, 1.0f, 1.0f, -1.0f);
        mvp = p * m;

        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
*/