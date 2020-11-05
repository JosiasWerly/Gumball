#ifndef _drawCall
#define _drawCall
#include "glm/glm.hpp"
#include <vector>

class iDrawCall {
public:
    VertexBuffer vb; // the guy who contains the data
    VertexBufferLayout vl; //the layout of data
    IndexBuffer ib; // data replication
    VertexArray va; //the guys who wrap everything above
    virtual void draw(const class Renderer& renderer) = 0;
};
#endif // !_drawCall
