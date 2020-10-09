
#include "Renderer.hpp"
#include "Patterns.hpp"


namespace ep {
    class VertexBuffer {
    public:
        unsigned int bufferID;
        VertexBuffer() {
            glDCall(glGenBuffers(1, &bufferID));
        }
        ~VertexBuffer() {
            glDCall(glDeleteBuffers(1, &bufferID));
        }
        void setData(const void* data, unsigned int size) {
            glDCall(glBindBuffer(GL_ARRAY_BUFFER, bufferID));
            glDCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
        }
        void bind() const {
            glDCall(glBindBuffer(GL_ARRAY_BUFFER, bufferID));
        }
        void unbind() const {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    };
    class DrawObject : public iDrawCall{
    public:
        void(*fnx)(void);
        void draw() {
            fnx(); 
        }
    };
    void test(Renderer &r) {
        Shader sA("defaultShader");
        sA.getParam("uColor")->value<Uniform<glm::fvec4>>().data = { 1, 1, 1, 1 };
        sA.getParam("mvp")->value<Uniform<glm::mat4>>().data = r.projection;

        DrawObject* d = r.newDrawCall<DrawObject>();

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
    

    ep::test(r);

	//auto DrawCallA = r.newDrawCall<debugDraw>();
	//Texture tA("grid");
	//tA.bind();
	//Shader sA("defaultShader");
	//sA.getParam("uColor")->value<Uniform<glm::fvec4>>().data = { 1, 1, 1, 1 };
	//sA.getParam("mvp")->value<Uniform<glm::mat4>>().data = r.projection;
	//auto bfLayoutA = new VertexBufferLayout;
	//bfLayoutA->push<float>(2);
	//bfLayoutA->push<float>(2);
	//DrawCallA->setup(
	//	&sA,
	//	*bfLayoutA,
	//	{
	//		0, 0,        0, 0,
	//		100, 0,      1, 0,
	//		100, 100,    1, 1,
	//		0, 100,      0, 1
	//	},
	//	{
	//		0, 1, 2,
	//		2, 3, 0
	//	}
	//	);


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