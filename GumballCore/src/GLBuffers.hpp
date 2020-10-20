#ifndef _glbuffers
#define _glbuffers
#include "Gumball.hpp"

class VertexBuffer  {
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
    struct VBEProxy {
        VertexBufferElement value;
        unsigned int count;
        
        template<class t> VBEProxy(t, unsigned int) {
        }
        template<class t> VBEProxy(float, unsigned int) {
            value = { GL_FLOAT, count, false };
        }
        template<class t> VBEProxy(int, unsigned int) {
            value = { GL_INT, count, false };
        }
        template<class t> VBEProxy(char, unsigned int) {
            value = { GL_BYTE, count, false };
        }
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

    int stride = 0;
    vector<VertexBufferElement> elements;

    int getStride() const {
        return stride;
    }
    const vector<VertexBufferElement>& getElements() {
        return elements;
    }

    struct test {
        float a, b;
    };
    VertexBufferLayout& operator<<(float) {
        //elements.push_back(vbe.value);
        //stride += VertexBufferLayout::getSizeType(GL_FLOAT) * vbe.count;
        return *this;
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
    IndexBuffer() {
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

#endif // !_glbuffers
