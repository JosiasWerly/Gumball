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



//class DrawCallBuffer{
//    IndexBuffer iBuffer;
//    VertexBuffer vBuffer;
//    VertexBufferLayout vLayout;
//    VertexArray vArray;
//    Shader shader;
//public:
//
//};
class iDrawCall {
public:
    virtual void draw(const class Renderer& renderer) = 0;
};






#endif // !_drawCall
