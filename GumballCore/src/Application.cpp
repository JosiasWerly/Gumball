
#include "Renderer.h"




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
static int loadShaderFromFile(string filePath) {
    auto shaderCode = loadShaderCode(filePath);
    return createShader(shaderCode.vertex, shaderCode.fragment);
}



int main() {
    Renderer r;
    r.setup("gumball", 800, 600);
    auto a = new debugDraw, b = new debugDraw;
    r.drawcalls.insert(a);
    r.drawcalls.insert(b);

    a->setup(
        loadShaderFromFile("res/shaders/blue.shader"), 
        {
            -.1,  -.1,
            .1,  -.1,
            .1, .1,
            -.1,  .1
        },
        { 
            0, 1, 2,
            2, 3, 0 
        }
    );
    b->setup(
        loadShaderFromFile("res/shaders/red.shader"),
        {
            -.4,  -.2,
            -.4,  -.0,
            -.2, .0,
            -.2,  -.2
        },
        {
            0, 1, 2,
            2, 3, 0
        }
        );


    //clear all the bind/selectionStack
    glUseProgram(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    //
    while (true){
        r.clear();       
        r.draw();
        r.swap();
    }

    
    return 0;
}


//int main() {
//    Window w;
//    w.setup("gumball", 800, 600);
//    auto d = new debugDraw;
//    w.drawcalls.insert(d);
//
//    float vertices[] = {
//        -.5,  -.5,
//        .5,  -.5,
//        .5, .5,
//        -.5,  .5
//    };
//    unsigned int vIndex[] = {
//        0, 1, 2,
//        2, 3, 0
//    };
//
//    auto shaderCode = loadShaderCode("res/shaders/defaultShader.shader");
//    unsigned int shader = createShader(shaderCode.vertex, shaderCode.fragment);
//
//
//    //bus of data
//    unsigned int va = 0;
//    glGenVertexArrays(1, &va);
//    glBindVertexArray(va);
//
//
//    // bus of data to send
//    unsigned int vb = 0;
//    glGenBuffers(1, &vb);
//    glBindBuffer(GL_ARRAY_BUFFER, vb);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//size of the data to send
//
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);//bind this vbo to vao
//
//    unsigned int ve = 0;
//    glGenBuffers(1, &ve);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ve);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vIndex), vIndex, GL_STATIC_DRAW); //??
//
//
//    //clear all the bind/selectionStack
//    glUseProgram(0);
//    glBindVertexArray(0);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//    //
//    while (true) {
//        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//
//
//        glUseProgram(shader);
//        glUniform4f(glGetUniformLocation(shader, "uColor"), .0, 0.1, 0.2, 1);
//        glBindVertexArray(va);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ve);
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
//        //endDrawCall
//
//        w.clear();
//
//    }
//
//    glDeleteVertexArrays(1, &va);
//    glDeleteBuffers(1, &ve);
//    glDeleteBuffers(1, &vb);
//    glDeleteProgram(shader);
//    return 0;
//}