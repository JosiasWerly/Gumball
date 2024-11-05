#include "Math.hpp"

void fRadToDegree(float &value) {
	value *= RadPI;
}
void fDegreeToRad(float &value) {
	value *= PIRad;
}

#pragma warning( disable : 4101 4244)




//	constexpr const Color Black(0x000000ff);
//	constexpr const Color White(0xffffffff);
//	constexpr Color Red = Color::Color(0xff0000ff);
//	constexpr const Color Green(0x00ff00ff);
//	constexpr const Color Blue(0x0000ffff);
//	constexpr const Color Yellow(0xffff00ff);
//	constexpr const Color Cyan(0x00ffffff);
//	constexpr const Color Magenta(0xff00ffff);
//	constexpr const Color Gray(0x808080ff);
//	constexpr const Color Brown(0xa52a2aff);
//	constexpr const Color Orange(0xffa500ff);
//	constexpr const Color Pink(0xffc0cbff);
//	constexpr const Color Purple(0x800080ff);

Color::Color() {
	r = g = b = 255;
	a = 0;
}
Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}
Color::Color(unsigned int hex) {
	r = (hex & 0xff000000) >> 24;
	g = (hex & 0x00ff0000) >> 16;
	b = (hex & 0x0000ff00) >> 8;
	a = hex & 0x000000ff;
}
Color::Color(glm::vec4 glmVec4) {
	glmVec4 *= 255.f;
	r = static_cast<char>(glmVec4.r);
	g = static_cast<char>(glmVec4.r);
	b = static_cast<char>(glmVec4.r);
	a = static_cast<char>(glmVec4.r);
}
Color::operator glm::vec4() { return glm::vec4(r/255.f, g/255.f, b/255.f, a/255.f); }


Vector2i::operator Vector3i() { return Vector3i(x, y, 0); }
Vector2i::operator Vector4i() { return Vector4i(x, y, 0, 0); }
Vector2i::operator Vector2() { return Vector2(x, y); }
Vector2i::operator Vector3() { return Vector3(x, y, 0); }
Vector2i::operator Vector4() { return Vector4(x, y, 0, 0); }

Vector3i::operator Vector2i() { return Vector2i(x, y); }
Vector3i::operator Vector4i() { return Vector4i(x, y, z, 0); }
Vector3i::operator Vector2() { return Vector2(x, y); }
Vector3i::operator Vector3() { return Vector3(x, y, z); }
Vector3i::operator Vector4() { return Vector4(x, y, z, 0); }

Vector4i::operator Vector2i() { return Vector2i(x, y); }
Vector4i::operator Vector3i() { return Vector3i(x, y, z); }
Vector4i::operator Vector2() { return Vector2(x, y); }
Vector4i::operator Vector3() { return Vector3(x, y, z); }
Vector4i::operator Vector4() { return Vector4(x, y, z, w); }

Vector2::operator Vector2i() { return Vector2i(x, y); }
Vector2::operator Vector3i() { return Vector3i(x, y, 0); }
Vector2::operator Vector4i() { return Vector4i(x, y, 0, 0); }
Vector2::operator Vector3() { return Vector3(x, y, 0); }
Vector2::operator Vector4() { return Vector4(x, y, 0, 0); }

Vector3::operator Vector2i() { return Vector2i(x, y); }
Vector3::operator Vector3i() { return Vector3i(x, y, z); }
Vector3::operator Vector4i() { return Vector4i(x, y, z, 0); }
Vector3::operator Vector2() { return Vector2(x, y); }
Vector3::operator Vector4() { return Vector4(x, y, z, 0); }

Vector4::operator Vector2i() { return Vector2i(x, y); }
Vector4::operator Vector3i() { return Vector3i(x, y, z); }
Vector4::operator Vector4i() { return Vector4i(x, y, z, w); }
Vector4::operator Vector2() { return Vector2(x, y); }
Vector4::operator Vector3() { return Vector3(x, y, z); }

Vector3 Quat::forward() const {
	return Vector3(glm::normalize(glm::rotate(rawQuat, glm::vec3(0.0f, 0.0f, -1.0f))));
}
Vector3 Quat::right() const {
	return Vector3(glm::normalize(glm::rotate(rawQuat, glm::vec3(1.0f, 0.0f, 0.0f))));
}
Vector3 Quat::up() const {
	return Vector3(glm::normalize(glm::rotate(rawQuat, glm::vec3(0.0f, 1.0f, 0.0f))));
}
Vector3 Quat::eulerAngles() const {
	return Vector3(glm::degrees(glm::eulerAngles(rawQuat)));
}
void Quat::eulerAngles(Vector3 direction) {
	rawQuat = glm::quat(glm::radians(direction.rawVector));
}
glm::mat4 Quat::getMatrix() const {
	return glm::toMat4(rawQuat);
}
void Quat::rotate(float pitch, float roll, float yaw) {
	rawQuat = rawQuat * glm::quat(glm::vec3(pitch, roll, yaw));
}
void Quat::rotateAround(Vector3 axis, float angle) {
	rawQuat = rawQuat * glm::angleAxis(glm::radians(angle), axis.rawVector);
}

Transform::Transform() {
	scale = Vector3(1, 1, 1);
}
Transform::Transform(glm::mat4 mat) {
	setMatrix(mat);
}
void Transform::setMatrix(const glm::mat4 &mat) {
	position = Vector3(glm::vec3(mat[3]));
	
	glm::mat3 rotationmat = glm::mat3(mat);
	scale.x = glm::length(glm::vec3(rotationmat[0]));
	scale.y = glm::length(glm::vec3(rotationmat[1]));
	scale.z = glm::length(glm::vec3(rotationmat[2]));
	rotationmat[0] /= scale.x;
	rotationmat[1] /= scale.y;
	rotationmat[2] /= scale.z;
	rotation.rawQuat = glm::quat_cast(rotationmat);
}
glm::mat4 Transform::getMatrix() const 	{
	glm::mat4 mat(1);
	mat = glm::translate(mat, position.rawVector);
	mat *= glm::mat4_cast(rotation.rawQuat);
	mat = glm::scale(mat, scale.rawVector);
	return mat;
}
glm::mat4 Transform::getRelativeMatrix(glm::mat4 mat) const {
	for (Transform *t = parent; t; t = t->parent)
		mat = parent->getMatrix() * mat;
	return mat;
}

#pragma warning( default : 4101 4244)


//class Rotator {
//protected:
//	glm::mat4 rot = glm::mat4(1); //EVALUATE perhaps I should transform this into a quat..
//public:
//	Rotator() {
//	}
//
//	Vector3 forward() {
//		return Vector3(normalize(glm::vec3(rot[2])));
//	}
//	Vector3 right() {
//		return Vector3(normalize(glm::vec3(rot[0])));
//	}
//	Vector3 up() {
//		return Vector3(normalize(glm::vec3(rot[1])));
//	}
//
//	Rotator &rotate(float pitch, float roll, float yaw) {
//		rot = glm::rotate(rot, glm::radians(pitch), glm::vec3(1, 0, 0));
//		rot = glm::rotate(rot, glm::radians(roll), glm::vec3(0, 1, 0));
//		rot = glm::rotate(rot, glm::radians(yaw), glm::vec3(0, 0, 1));
//		return *this;
//	}
//	Rotator &rotateAround(float angle, Vector3 axis) {
//		rot = glm::rotate(rot, glm::radians(angle), axis.rawVector);
//		return *this;
//	}
//	Vector3 eulerAngles() {
//		return Vector3(glm::eulerAngles(glm::quat_cast(rot)));
//	}
//	glm::mat4& getMat() {
//		return rot;
//	}
//};