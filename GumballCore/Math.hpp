#ifndef __math
#define __math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

const float PI = glm::pi<float>();
const float PIRad = PI / 180.f;
const float RadPI = 180.f / PI;

Inline void fRadToDegree(float &value);
Inline void fDegreeToRad(float &value);


struct Color {
	union {
		unsigned char R, G, B, A;
		int hexColor;	
	};
};

template<class T> 
class TVector {
public:
	typedef TVector<T> TParent;
	typedef T TVec;

	TVec rawVector;

	TVector(){}
	TVector(TVec &&rawVector) :
		rawVector(rawVector){
	}

	TVector &operator+=(TVector v) {
		rawVector += v.rawVector;
		return *this;
	}
	TVector &operator-=(TVector v) {
		rawVector -= v.rawVector;
		return *this;
	}
	TVector &operator*=(TVector v) {
		rawVector *= v.rawVector;
		return *this;
	}
	TVector &operator/=(TVector v) {
		rawVector /= v.rawVector;
		return *this;
	}

	TVector operator+(TVector v) {
		return TVector(rawVector + v.rawVector);
	}
	TVector operator-(TVector v) {
		return TVector(rawVector - v.rawVector);
	}
	TVector operator*(TVector v) {
		return TVector(rawVector * v.rawVector);
	}
	TVector operator/(TVector v) {
		return TVector(rawVector / v.rawVector);
	}

	Inline TVector normalize() const {
		return glm::normalize(rawVector);
	}

	static TVector direction(TVector a, TVector b) {
		return (b - a).normalize();
	}
	static TVector cross(TVector a, TVector b) {
		return TVector(glm::cross(a.rawVector, b.rawVector));
	}
	static float dot(TVector a, TVector b) {
		return glm::dot(a.rawVector, b.rawVector);
	}
};

class Vector3;
class Vector2;
class Vector3i;
class Vector2i;

class Vector3 : 
	public TVector<glm::vec3> {
public:
	float	&x = rawVector.x,
			&y = rawVector.y,
			&z = rawVector.z;

	Vector3() = default;
	Vector3(float x, float y, float z) :
		TParent(TVec(x, y, z)) {
	}
	Vector3(TParent &&init) :
		TParent(init) {
	}
	Vector3 &operator=(const Vector3 &other) {
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	};

	operator Vector3i();
	operator Vector2();
	operator Vector2i();
};
class Vector2 : 
	public TVector<glm::vec2> {
public:
	using TParent::TParent;
	float	&x = rawVector.x,
			&y = rawVector.y;

	Vector2() = default;
	Vector2(float x, float y) :
		TParent(TVec(x, y)) {
	}
	Vector2(TParent &&init) :
		TParent(init) {
	}
	Vector2 &operator=(const Vector2 &other) {
		x = other.x;
		y = other.y;
		return *this;
	};

	operator Vector3();
	operator Vector3i();
	operator Vector2i();
};
class Vector3i :
	public TVector<glm::ivec3> {
public:
	using TParent::TParent;
	int	&x = rawVector.x,
		&y = rawVector.y,
		&z = rawVector.z;

	Vector3i() = default;
	Vector3i(int x, int y, int z) :
		TParent(TVec(x, y, z)) {
	}
	Vector3i(TParent && init) :
		TParent(init) {
	}
	Vector3i &operator=(const Vector3i &other) {
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	};

	operator Vector3();
	operator Vector2();
	operator Vector2i();
};
class Vector2i :
	public TVector<glm::ivec2> {
public:
	using TParent::TParent;
	int	&x = rawVector.x,
		&y = rawVector.y;
	
	Vector2i() = default;
	Vector2i(int x, int y) :
		TParent(TVec(x, y)) {
	}
	Vector2i(TParent && init) :
		TParent(init) {
	}
	Vector2i &operator=(const Vector2i & other) {
		x = other.x;
		y = other.y;
		return *this;
	};

	operator Vector3();
	operator Vector3i();
	operator Vector2();
};




class Rotator {
protected:
	glm::mat4 _rot = glm::mat4(1);
public:
	Rotator() {
	}

	Vector3 forward() {
		return Vector3(normalize(glm::vec3(glm::inverse(_rot)[2])));
	}
	Vector3 right() {
		return Vector3(normalize(glm::vec3(glm::inverse(_rot)[0])));
	}
	Vector3 up() {
		return Vector3(normalize(glm::vec3(glm::inverse(_rot)[1])));
	}

	Rotator &rotate(float pitch, float roll, float yaw) {
		fDegreeToRad(pitch);
		fDegreeToRad(roll);
		fDegreeToRad(yaw);
		_rot = glm::rotate(_rot, pitch, right().rawVector);
		_rot = glm::rotate(_rot, roll, forward().rawVector);
		_rot = glm::rotate(_rot, yaw, up().rawVector);
		return *this;
	}
	Rotator &rotateAround(float angle, Vector3 axis) {
		_rot = glm::rotate(_rot, angle, axis.rawVector);
		return *this;
	}

	Vector3 eulerAngles() {
		return Vector3(glm::eulerAngles(glm::quat_cast(_rot)));
	}
	glm::mat4 &rawMat() {
		return _rot;
	}
};

#endif // !__math
