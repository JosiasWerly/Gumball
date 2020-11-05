#ifndef _math
#define _math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Vector2 {
	glm::fvec2 _vec;
public:
	float 
		&x = _vec.x, 
		&y = _vec.y;
};


#endif // !_math