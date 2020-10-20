#ifndef _drawCall
#define _drawCall
#include "glm/glm.hpp"
#include <vector>

class iDrawCall {
public:
    virtual void draw(const class Renderer& renderer) = 0;
};
#endif // !_drawCall
