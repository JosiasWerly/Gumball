#pragma once
#ifndef _math
#define _math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
const float PI = glm::pi<float>();
const float PIRad = PI/180.f;
const float RadPI = 180.f/ PI;

void fRadToDegree(float& value);
void fDegreeToRad(float& value);


#endif // !_math