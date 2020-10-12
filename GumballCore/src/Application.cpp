
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
    class VertexBufferLayout {
    public:
        struct VertexBufferElement {
        unsigned int type, count;
        unsigned char normalized;
    };
        
        static unsigned int getSizeType(const unsigned int type) {
            switch (type) {
            case GL_FLOAT:  return 4;
            case GL_INT:    return 4;
            case GL_BYTE:   return 1;
            }
            __debugbreak();
            return -1;
        }

        int stride;
        vector<VertexBufferElement> elements;

        int getStride() const {
            return stride;
        }
        const vector<VertexBufferElement>& getElements() {
            return elements;
        }
        template<typename T>void push(unsigned int) {}
        template<>void push<float>(unsigned int count) {
            elements.push_back({ GL_FLOAT, count, false });
            stride += VertexBufferLayout::getSizeType(GL_FLOAT) * count;
        }
        template<>void push<int>(unsigned int count) {
            elements.push_back({ GL_INT, count, false });
            stride += VertexBufferLayout::getSizeType(GL_INT) * count;
        }
        template<>void push<char>(unsigned int count) {
            elements.push_back({ GL_BYTE, count, false });
            stride += VertexBufferLayout::getSizeType(GL_BYTE) * count;
        }
    };
    class VertexArray {
    public:
        unsigned int bufferId;
        VertexArray() {
            glGenVertexArrays(1, &bufferId);
        }
        ~VertexArray() {
            glDeleteVertexArrays(1, &bufferId);
        }
        void bind() {
            glBindVertexArray(bufferId);
        }
        void unbind() {
            glBindVertexArray(0);
        }
        void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
            bind();
            vb.bind();
            const auto& elements = layout.elements;
            unsigned int offset = 0;
            for (unsigned int i = 0; i < elements.size(); i++) {
                const auto& element = elements[i];
                glDCall(glEnableVertexAttribArray(i));
                glDCall(glVertexAttribPointer(i, element.count, element.type,
                    element.normalized, layout.getStride(), (const void*)offset));
                offset += element.count * VertexBufferLayout::getSizeType(element.type);
            }
        }
    };
    class IndexBuffer {
    public:
        unsigned int bufferID, count;
        IndexBuffer(){
            glDCall(glGenBuffers(1, &bufferID));
        }
        ~IndexBuffer() {
            glDCall(glDeleteBuffers(1, &bufferID));
        }
        void setData(unsigned int* indices, unsigned int count) {
            this->count = count;
            glDCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID));
            glDCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW));
        }
        void bind() const {
            glDCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID));
        }
        void unbind() const {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        unsigned int getCount() const {
            return count;
        }
    };


    class DrawObject : public iDrawCall{
    public:
        VertexBuffer vb;
        VertexBufferLayout vl;
        VertexArray va;
        IndexBuffer ib;
        Shader sa;
        Texture tx;

        vector<float> vertex;
        vector<unsigned int> index;
        
        DrawObject() {
            tx.changeTexture("grid");
            sa.changeShader("defaultShader");
            vertex = {
                0, 0,        0, 0,
                100, 0,      1, 0,
                100, 100,    1, 1,
                0, 100,      0, 1
            };
            index = {
                0, 1, 2,
                2, 3, 0
            };

            tx.bind();
            vl.push<float>(2);
            vl.push<float>(2);

            va.bind();
            vb.setData(vertex.data(), vertex.size() * sizeof(float));
            ib.setData(index.data(), index.size());
            va.addBuffer(vb, vl);
            va.unbind();
        }
        void draw() {
            tx.bind();
            sa.bind();
            va.bind();    
            glDCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
        }
    };
    void test(Renderer &r) {
        DrawObject* d = r.newDrawCall<DrawObject>();
        d->sa.getParam("uColor")->value<Uniform<glm::fvec4>>().data = { 1, 1, 1, 1 };
        d->sa.getParam("mvp")->value<Uniform<glm::mat4>>().data = r.projection;
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
    //);


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