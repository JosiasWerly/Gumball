
#include "Renderer.h"
#include "Patterns.hpp"




class iAssetInstance {
public:

};

template<typename tType> class iAssetSystem{
private:
    typedef map<string, iAssetInstance*> TAssets;
    typedef pair<string, iAssetInstance*> aPair;
    typedef TAssets::iterator aIt;
    TAssets assets;
public:
    bool contains(string name) {
        return assets.find(name) != assets.end();
    }
    bool contains(string name, aIt &it) {
        it = assets.find(name);
        return  it != assets.end();
    }

    bool load(string name, tType&& other) {
        if (!contains(name)) {
            assets.insert(aPair(name, new tType(other)));
            return true;
        }
        return false;
    }
    bool unload(string name, tType&&other) {
        aIt it;
        if (contains(name, it)) {
            delete (*it).second;
            assets.erase(it);
            return true;
        }
        return false;
    }
};

class BufferAsset : 
    public iAssetInstance{
public:
    char* data;
    BufferAsset() {
    }
};



class BufferInstance : 
    public BufferAsset {
    BufferInstance(){
    }
};
int main() {




















    Renderer r;
    r.setup("gumball", 800, 600);
    auto a = new debugDraw, b = new debugDraw;
    r.drawcalls.insert(a);
    r.drawcalls.insert(b);
    
    auto &sy = ShaderSystem::getInstance();
    sy.newShaderFromFile("res/shaders/defaultShader.shader");
    sy.newShaderFromFile("res/shaders/red.shader");
    sy.newShaderFromFile("res/shaders/blue.shader");
    
    Shader sA("defaultShader"), sB("defaultShader");
    sA.setParam<UniformParam<float, 4>>("uColor", { 0.f, 1.f, 0.f, 0.f });
    sB.setParam<UniformParam<float, 4>>("uColor", { 1.f, 0.f, 0.f, 0.f });
    auto bfLayout = new VertexBufferLayout;
    bfLayout->push<float>(2);
    a->setup(
        &sA,
        *bfLayout,
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
        &sB,
        *bfLayout,
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

    while (!glfwWindowShouldClose(r.window)){
        r.clear();       
        r.draw();
        r.swap();
    }
    sy.clearAllShaders();
    r.drawcalls.clear();
    delete a, b;

    glfwTerminate();
    return 0;
}