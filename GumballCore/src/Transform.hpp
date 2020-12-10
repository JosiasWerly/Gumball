#ifndef _transform
#define _transform
#include "Math.hpp"

class FVector {
protected:
	glm::vec<3, float, glm::highp> _vec;
public:
	float
		&x = _vec.x,
		&y = _vec.y,
		&z = _vec.z;

	FVector() :
		_vec(0, 0, 0) {
	}
	FVector(glm::fvec3 vec) :
		_vec(vec){

	}
	FVector(float x, float y, float z) :
		_vec(x, y, z) {
	}

	glm::vec<3, float, glm::highp>& rawVec() {
		return this->_vec;
	}

	FVector& operator+=(FVector v) {
		_vec += v._vec;
		return *this;
	}
	FVector& operator-=(FVector v) {
		_vec -= v._vec;
		return *this;
	}
	FVector& operator*=(FVector v) {
		_vec *= v._vec;
		return *this;
	}
	FVector& operator/=(FVector v) {
		_vec /= v._vec;
		return *this;
	}

	FVector operator+(FVector v) {		
		return FVector(_vec + v._vec);
	}
	FVector operator-(FVector v) {
		return FVector(_vec - v._vec);
	}
	FVector operator*(FVector v) {
		return FVector(_vec * v._vec);
	}
	FVector operator/(FVector v) {
		return FVector(_vec / v._vec);
	}
	FVector normalize() {
		return glm::normalize(_vec);
	}


	static FVector direction(FVector a, FVector b) {
		return (b - a).normalize();
	}
	static FVector cross(FVector a, FVector b) {
		return FVector(glm::cross(a._vec, b._vec));
	}
	static float dot(FVector a, FVector b){
		return glm::dot(a._vec, b._vec);
	}
};
class Rotator {
protected:
	glm::mat4 _rot = glm::mat4(1);
public:
	Rotator() {
	}

	FVector forward() {
		return FVector(normalize(glm::vec3(glm::inverse(_rot)[2])));
	}
	FVector right() {
		return FVector(normalize(glm::vec3(glm::inverse(_rot)[0])));
	}
	FVector up() {
		return FVector(normalize(glm::vec3(glm::inverse(_rot)[1])));
	}
	
	Rotator& rotate(float pitch, float roll, float yaw) {
		_rot = glm::rotate(_rot, pitch, right().rawVec());
		_rot = glm::rotate(_rot, roll, forward().rawVec());
		_rot = glm::rotate(_rot, yaw, up().rawVec());
		return *this;
	}
	Rotator& rotateAround(float angle, FVector axis) {
		_rot = glm::rotate(_rot, angle, axis.rawVec());
		return *this;
	}

	FVector eulerAngles() {
		return glm::eulerAngles(glm::quat_cast(_rot));
	}
	glm::mat4& rawMat() {
		return _rot;
	}
};

class Transform{
	Transform* parent = nullptr;
	glm::mat4 ModelMatrix;
public:
	FVector
		position,
		scale = FVector(1, 1, 1);
	Rotator rotation;

	Transform(){
	}
	glm::mat4 getModel() {
		glm::mat4 out(1);
		out = glm::translate(out, position.rawVec());
		out = rotation.rawMat() * out;
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