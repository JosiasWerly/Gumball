#ifndef _drawCall
#define _drawCall
#include "glm/glm.hpp"
#include <vector>
//struct sMeshVertex{
//	glm::vec3 pos, normal;
//	glm::vec2 uv;
//};
//class DrawCallBufferData {
//public:
//	vector<unsigned int> index;
//	vector<sMeshVertex> bufferData;
//};



class DrawCallBuffers {
public:
    IndexBuffer iBuffer;
    VertexBuffer vBuffer;
    VertexBufferLayout vLayout;
    VertexArray vArray;
    Shader shader;
};
class iDrawCall {
public:
    virtual void draw(const class Renderer& renderer) = 0;
};
class MeshDrawCall {

};


#endif // !_drawCall
