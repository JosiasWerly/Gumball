#ifndef _math
#define _math
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtx/quaternion.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtx/matrix_decompose.hpp>

const float PI = glm::pi<float>();
const float PIRad = PI / 180.f;
const float RadPI = 180.f / PI;

void fRadToDegree(float &value);
void fDegreeToRad(float &value);

struct GENGINE Color {
	unsigned char r, g, b, a;

	Color();
	Color(unsigned int hex);
	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 0);

	Color(glm::vec4 glmVec4);
	operator glm::vec4();
};

class Vector2i;
class Vector3i;
class Vector4i;
class Vector2;
class Vector3;
class Vector4;

template<class T>
class TVector {
public:
	typedef TVector<T> TParent;
	typedef T TVec;

	TVec rawVector;

	TVector() :
		rawVector(TVec(0)) {
	}
	TVector(TVec rawVector) :
		rawVector(rawVector) {
	}
	operator TVec &() { return rawVector; }

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


	TVector &operator+=(float v) {
		rawVector += v;
		return *this;
	}
	TVector &operator-=(float v) {
		rawVector -= v;
		return *this;
	}
	TVector &operator*=(float v) {
		rawVector *= v;
		return *this;
	}
	TVector &operator/=(float v) {
		rawVector /= v;
		return *this;
	}

	TVector operator+(float v) {
		return TVector(rawVector + v);
	}
	TVector operator-(float v) {
		return TVector(rawVector - v);
	}
	TVector operator*(float v) {
		return TVector(rawVector * v);
	}
	TVector operator/(float v) {
		return TVector(rawVector / v);
	}

	Inline TVector normalize() const { return glm::normalize(rawVector); }
	Inline float length() const { return glm::length(rawVector); }

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

class Vector2i : public TVector<glm::ivec2> {
public:
	using TParent::TParent;
	Vector2i() = default;
	Vector2i(int x, int y) :
		TParent(TVec(x, y)) {
	}
	Vector2i(TParent &&init) :
		TParent(init) {
	}
	Vector2i &operator=(const Vector2i &other) {
		x() = other.x();
		y() = other.y();
		return *this;
	};
	
	Inline int &x() { return rawVector.x; }
	Inline int &y() { return rawVector.y; }
	Inline const int &x() const { return rawVector.x; }
	Inline const int &y() const { return rawVector.y; }

	operator Vector3i();
	operator Vector4i();
	operator Vector2();
	operator Vector3();
	operator Vector4();
};
class Vector3i : public TVector<glm::ivec3> {
public:
	using TParent::TParent;
	Vector3i() = default;
	Vector3i(int x, int y, int z) :
		TParent(TVec(x, y, z)) {
	}
	Vector3i(TParent &&init) :
		TParent(init) {
	}
	Vector3i &operator=(const Vector3i &other) {
		x() = other.x();
		y() = other.y();
		z() = other.z();
		return *this;
	};

	Inline int &x() { return rawVector.x; }
	Inline int &y() { return rawVector.y; }
	Inline int &z() { return rawVector.z; }
	Inline const int &x() const { return rawVector.x; }
	Inline const int &y() const { return rawVector.y; }
	Inline const int &z() const { return rawVector.z; }

	operator Vector2i();
	operator Vector4i();
	operator Vector2();
	operator Vector3();
	operator Vector4();
};
class Vector4i : public TVector<glm::ivec4> {
public:
	using TParent::TParent;
	Vector4i() = default;
	Vector4i(int x, int y, int z, int w) :
		TParent(TVec(x, y, z, w)) {
	}
	Vector4i(TParent &&init) :
		TParent(init) {
	}
	Vector4i &operator=(const Vector4i &other) {
		x() = other.x();
		y() = other.y();
		z() = other.z();
		w() = other.w();
		return *this;
	};

	Inline int &x() { return rawVector.x; }
	Inline int &y() { return rawVector.y; }
	Inline int &z() { return rawVector.z; }
	Inline int &w() { return rawVector.w; }
	Inline const int &x() const { return rawVector.x; }
	Inline const int &y() const { return rawVector.y; }
	Inline const int &z() const { return rawVector.z; }
	Inline const int &w() const { return rawVector.w; }

	operator Vector2i();
	operator Vector3i();
	operator Vector2();
	operator Vector3();
	operator Vector4();
};
class Vector2 : public TVector<glm::vec2> {
public:
	using TParent::TParent;
	Vector2() = default;
	Vector2(float x, float y) :
		TParent(TVec(x, y)) {
	}
	Vector2(TParent &&init) :
		TParent(init) {
	}
	Vector2 &operator=(const Vector2 &other) {
		x() = other.x();
		y() = other.y();
		return *this;
	};

	Inline float &x() { return rawVector.x; }
	Inline float &y() { return rawVector.y; }
	Inline const float &x() const { return rawVector.x; }
	Inline const float &y() const { return rawVector.y; }

	operator Vector2i();
	operator Vector3i();
	operator Vector4i();
	operator Vector3();
	operator Vector4();
};
class Vector3 :	public TVector<glm::vec3> {
public:
	using TParent::TParent;
	Vector3() = default;
	Vector3(float x, float y, float z) :
		TParent(TVec(x, y, z)) {
	}
	Vector3(TParent &&init) :
		TParent(init) {
	}
	Vector3 &operator=(const Vector3 &other) {
		x() = other.x();
		y() = other.y();
		z() = other.z();
		return *this;
	};

	Inline float &x() { return rawVector.x; }
	Inline float &y() { return rawVector.y; }
	Inline float &z() { return rawVector.z; }
	Inline const float &x() const { return rawVector.x; }
	Inline const float &y() const { return rawVector.y; }
	Inline const float &z() const { return rawVector.z; }

	operator Vector2i();
	operator Vector3i();
	operator Vector4i();
	operator Vector2();
	operator Vector4();
};
class Vector4 :	public TVector<glm::vec4> {
public:
	using TParent::TParent;
	Vector4() = default;
	Vector4(float x, float y, float z, float w) :
		TParent(TVec(x, y, z, w)) {
	}
	Vector4(TParent &&init) :
		TParent(init) {
	}
	Vector4 &operator=(const Vector4 &other) {
		x() = other.x();
		y() = other.y();
		z() = other.z();
		w() = other.w();
		return *this;
	};

	Inline float &x() { return rawVector.x; }
	Inline float &y() { return rawVector.y; }
	Inline float &z() { return rawVector.z; }
	Inline float &w() { return rawVector.w; }

	Inline const float &x() const { return rawVector.x; }
	Inline const float &y() const { return rawVector.y; }
	Inline const float &z() const { return rawVector.z; }
	Inline const float &w() const { return rawVector.w; }

	operator Vector2i();
	operator Vector3i();
	operator Vector4i();
	operator Vector2();
	operator Vector3();
};

class GENGINE Quat {
public:
	glm::quat rawQuat;

	Quat() :
		rawQuat(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)) {
	}
	Quat(float w, float x, float y, float z) :
		rawQuat(glm::quat(w, x, y, z)) {
	}

	Vector3 forward() const;
	Vector3 right() const;
	Vector3 up() const;
	Vector3 eulerAngles() const;
	void eulerAngles(Vector3 direction);

	glm::mat4 getMatrix() const;
	void rotate(float pitch, float roll, float yaw);
	void rotateAround(Vector3 axis, float angle);
};

class GENGINE Transform {
private:
	Transform *parent = nullptr;

	glm::mat4 getRelativeMatrix(glm::mat4 mat) const;

public:
	Vector3 position;
	Vector3 scale;
	Quat rotation;

	Transform();
	Transform(glm::mat4 mat);

	void setParent(Transform *newParent) { parent = newParent; }
	Transform *getParent() { return parent; }

	void setMatrix(const glm::mat4 &mat);
	glm::mat4 getMatrix() const;
	glm::mat4 getRelativeMatrix() const { return getRelativeMatrix(getMatrix()); }

};
#endif // !_math