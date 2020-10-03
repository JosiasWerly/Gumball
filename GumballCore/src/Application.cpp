
#include "Renderer.hpp"
#include "Patterns.hpp"


namespace exp {
    #include <unordered_map>


    struct VertexBufferElement{
        unsigned int type, count;
        unsigned char normalized;
    };
    class vLayout {
    public:
        vector<VertexBufferElement> elements;
        vLayout& operator<<(VertexBufferElement data) {
            //elements.push_back(data);
            return *this;
        }
    };
    class DrawObject : public iDrawCall{
    public:
        vLayout layout;
        void draw() {

        }
    };
    void test(Renderer &r) {
        DrawObject* d = r.newDrawCall<DrawObject>();
        d->layout << VertexBufferElement{0, 0, 0};

    }
};


int main() {
    Renderer r;
    r.setup("gumball", 800, 600);

    auto& sy = ShaderSystem::instance();
    auto& st = TextureSystem::instance();

    sy.loadFromFile("res/shaders/defaultShader.shader");
    sy.loadFromFile("res/shaders/red.shader");
    sy.loadFromFile("res/shaders/blue.shader");
    st.loadFromFile("res/textures/test.png");
    st.loadFromFile("res/textures/gumball.png");
    st.loadFromFile("res/textures/gumbalA.png");
    st.loadFromFile("res/textures/grid.png");
    

    exp::test(r);

    auto DrawCallA = r.newDrawCall<debugDraw>();
	Texture tA("grid");
    tA.bind();
	Shader sA("defaultShader");
    sA.getParam("uColor")->value<Uniform<glm::fvec4>>().data = { 1, 1, 1, 1 };
    sA.getParam("mvp")->value<Uniform<glm::mat4>>().data = r.projection;
    auto bfLayoutA = new VertexBufferLayout;
	bfLayoutA->push<float>(2);
    bfLayoutA->push<float>(2);
	DrawCallA->setup(
		&sA,
		*bfLayoutA,
		{
			0, 0,        0, 0,
			100, 0,      1, 0,
            100, 100,    1, 1,
			0, 100,      0, 1
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
    sy.unloadAll();
    r.drawcalls.clear();
    glfwTerminate();
    return 0;
}