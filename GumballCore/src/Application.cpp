
#include "Renderer.hpp"
#include "Patterns.hpp"


//sa.getParam("uColor")->value<Uniform<glm::fvec4>>().data = c;
//sa.getParam("uModel")->value<Uniform<glm::mat4>>().data = fMat;
//sa.getParam("uView")->value<Uniform<glm::mat4>>().data = renderer.viewMode.mView;
//sa.getParam("uProj")->value<Uniform<glm::mat4>>().data = renderer.viewMode.mProjection;




// d->sa.getParam("uProj")->value<Uniform<glm::mat4>>().data = camera->viewMode.mProjection;
class baseUniform {
public:
    void* const ptr;
    baseUniform(void* ptr = nullptr) : ptr(ptr){}
    virtual void pushParam() = 0;
};

template<class T>class UniformMK2 : public baseUniform {
public:
    UniformMK2(){}
    virtual void pushParam() = 0;
};
template<>class UniformMK2<float> : public baseUniform {
public:
    float value;
    UniformMK2() : baseUniform(&value){}
    void pushParam() { cout << "float " << value << endl; }
};
template<>class UniformMK2<int> : public baseUniform {
public:
    int value;
    UniformMK2() : baseUniform(&value) {}
    void pushParam() { cout << "int " << value << endl; }
};
template<>class UniformMK2<char> : public baseUniform {
public:
    char value;
    UniformMK2() : baseUniform(&value) {}
    void pushParam() { cout << "char " << value << endl; }
};






class ShaderMk2{
public:    
    map<string, baseUniform*> uniforms;
    ShaderMk2() {
        uniforms["a"] = new UniformMK2<float>;
        uniforms["b"] = new UniformMK2<int>;
        uniforms["c"] = new UniformMK2<char>;
    }
    baseUniform* getParam(string key){
        return uniforms[key];
    }
    template<class T> T& get(string key) {
        static char returnFail[64];
		auto it = uniforms.find(key);
		if (it != uniforms.end())
			return *reinterpret_cast<T*>((it)->second->ptr);
		return *(T*)&returnFail;
    }
};




void processInput(GLFWwindow* window);
int main() {
    ShaderMk2 sa;


    sa.getParam("a")->pushParam();
    sa.getParam("b")->pushParam();
    sa.getParam("c")->pushParam();

    sa.get<float>("a") = 2.5;
    sa.get<int>("b") = -1;
    sa.get<char>("c") = 'c';

    sa.get<char>("ww") = 'c';

    sa.getParam("a")->pushParam();
    sa.getParam("b")->pushParam();
    sa.getParam("c")->pushParam();
    



    Gumball::Window w;
    w.create("Gumball", 800, 600);
       
    auto& sy = ShaderSystem::instance();
    auto& st = TextureSystem::instance();
    auto& sm = MeshSystem::instance();

    sy.loadFromFile("res/shaders/default.shader");
    sy.loadFromFile("res/shaders/unlit.shader");
    sy.loadFromFile("res/shaders/texture.shader");
    
    st.loadFromFile("res/textures/test.png");
    st.loadFromFile("res/textures/gumball.png");
    st.loadFromFile("res/textures/gumbalA.png");
    st.loadFromFile("res/textures/grid.png");

    sm.loadFromFile("res/models/suzane.obj");
    sm.loadFromFile("res/models/torus.obj");
        
    Renderer r(&w);
    Camera* c = r.camera = new Camera();
    c->transform.position += glm::vec3(0, 0, -200);
    
    constexpr int recSize = 100;
    vector<Meshdata*> drawObjects;

    for (size_t i = 0; i < recSize; i++) {
        Meshdata* newMesh = new Meshdata;

        //if(drawObjects.size() != 0)
        //    newMesh->transform.attach(&drawObjects.back()->transform);
        
        if (i % 2)
            newMesh->loadMesh("suzane" );
        else 
            newMesh->loadMesh("torus");
        newMesh->transform.position += glm::vec3(rand() % 100 -50, rand() % 100-50, rand() % 100-50);
        drawObjects.push_back(newMesh);
        r << newMesh;
    }


    w.clearGLStack();
    while (!glfwWindowShouldClose(r.window)){
        if (glfwGetKey(r.window, GLFW_KEY_W) == GLFW_PRESS)
            c->transform.rotation += glm::vec3(0, 0, 0.1);
        else if (glfwGetKey(r.window, GLFW_KEY_S) == GLFW_PRESS)
            c->transform.rotation += glm::vec3(0, 0, -0.1);

        if (glfwGetKey(r.window, GLFW_KEY_A) == GLFW_PRESS)
            c->transform.rotation += glm::vec3(0, 0.1, 0);
		else if (glfwGetKey(r.window, GLFW_KEY_D) == GLFW_PRESS)
            c->transform.rotation += glm::vec3(0, -0.1, 0);
		       
        w.clearBuffer();
        r.drawRender();
        w.swapBuffers();
        cout << w.getMS() << endl;
    }
    sy.unloadAll();
    r.drawcalls.clear();
    glfwTerminate();
    return 0;
}

/*
##### Gumball 0.1 #####
-create asset system
-resolve drawcall, camera and uniforms system

-collision detection
-traces, lines
*/