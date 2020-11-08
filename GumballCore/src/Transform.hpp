#ifndef _transform
#define _transform
#include "Math.hpp"

class Transform{
	Transform* parent = nullptr;


	glm::mat4 ModelMatrix;
public:
	glm::vec3 
		position = glm::vec3(0, 0, 0),
		rotation = glm::vec3(0, 0, 0),
		scale = glm::vec3(1, 1, 1);
	Transform(){
	}
	
	glm::mat4 getModel() {
		glm::mat4 out(1);
		out = glm::translate(out, position);
		out = glm::rotate(out, rotation.x, glm::vec3(1, 0, 0));
		out = glm::rotate(out, rotation.y, glm::vec3(0, 1, 0));
		out = glm::rotate(out, rotation.z, glm::vec3(1, 0, 0));
		out = glm::scale(out, scale);
		return out;
	}
	glm::mat4 getResultModel(glm::mat4 childModel = glm::mat4(1)) {
		if (parent)
			return parent->getResultModel(getModel());
		else
			return getModel() * childModel;			
	}
	void attach(Transform* parent) {
		this->parent = parent;
	}
};
 

#endif // !_math