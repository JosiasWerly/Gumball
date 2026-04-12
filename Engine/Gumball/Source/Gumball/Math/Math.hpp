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
	typedef TVector<T> Vector;
	typedef T Type;

	Type rawVector;

	TVector() : rawVector(Type(0)) {}
	TVector(Type rawVector) : rawVector(rawVector) {}
	TVector(const Vector &) = default;
	TVector(const Vector &&) = delete;
	operator Type &() { return rawVector; }

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
	using TVector<glm::ivec2>::Type;

	int &x, &y;

	Vector2i(Type init = Type(0)) :
		TVector<glm::ivec2>(init),
		x(rawVector.x), y(rawVector.y) {
	}
	Vector2i(int x, int y) :
		TVector<glm::ivec2>(Type(x, y)),
		x(rawVector.x), y(rawVector.y) {
	}
	Vector2i(const Vector &cpy) :
		TVector<glm::ivec2>(cpy),
		x(rawVector.x), y(rawVector.y) {
	}
	Vector2i(const Vector2i &cpy) :
		TVector<glm::ivec2>(cpy.rawVector),
		x(rawVector.x), y(rawVector.y) {
	}
	Vector2i &operator=(const Vector2i &other) {
		x = other.x;
		y = other.y;
		return *this;
	};

	operator Vector3i();
	operator Vector4i();
	operator Vector2();
	operator Vector3();
	operator Vector4();
};
class Vector3i : public TVector<glm::ivec3> {
public:	
	using TVector<glm::ivec3>::Type;

	int &x, &y, &z;

	Vector3i(Type init = Type(0)) :
		TVector<glm::ivec3>(init),
		x(rawVector.x), y(rawVector.y), z(rawVector.z) {
	}
	Vector3i(int x, int y, int z) :
		TVector<glm::ivec3>(Type(x, y, z)),
		x(rawVector.x), y(rawVector.y), z(rawVector.z) {
	}
	Vector3i(const Vector &cpy) :
		TVector<glm::ivec3>(cpy),
		x(rawVector.x), y(rawVector.y), z(rawVector.z) {
	}
	Vector3i(const Vector3i &cpy) :
		TVector<glm::ivec3>(cpy.rawVector),
		x(rawVector.x), y(rawVector.y), z(rawVector.z) {
	}
	Vector3i &operator=(const Vector3i &other) {
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	};

	operator Vector2i();
	operator Vector4i();
	operator Vector2();
	operator Vector3();
	operator Vector4();
};
class Vector4i : public TVector<glm::ivec4> {
public:
	using TVector<glm::ivec4>::Type;

	int &x, &y, &z, &w;

	Vector4i(Type init = Type(0)) :
		TVector<glm::ivec4>(init),
		x(rawVector.x), y(rawVector.y), z(rawVector.z), w(rawVector.w) {
	}
	Vector4i(int x, int y, int z, int w) :
		TVector<glm::ivec4>(Type(x, y, z, w)),
		x(rawVector.x), y(rawVector.y), z(rawVector.z), w(rawVector.w) {
	}
	Vector4i(const Vector &cpy) :
		TVector<glm::ivec4>(cpy),
		x(rawVector.x), y(rawVector.y), z(rawVector.z), w(rawVector.w) {
	}
	Vector4i(const Vector4i &cpy) :
		TVector<glm::ivec4>(cpy.rawVector),
		x(rawVector.x), y(rawVector.y), z(rawVector.z), w(rawVector.w) {
	}
	Vector4i &operator=(const Vector4i &other) {
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
		return *this;
	};

	operator Vector2i();
	operator Vector3i();
	operator Vector2();
	operator Vector3();
	operator Vector4();
};
class Vector2 : public TVector<glm::vec2> {
public:
	using TVector<glm::vec2>::Type;

	float &x, &y;

	Vector2(Type init = Type(0)) :
		TVector<glm::vec2>(init),
		x(rawVector.x), y(rawVector.y) {
	}
	Vector2(int x, int y) :
		TVector<glm::vec2>(Type(x, y)),
		x(rawVector.x), y(rawVector.y) {
	}
	Vector2(const Vector &cpy) :
		Vector(cpy),
		x(rawVector.x), y(rawVector.y) {
	}
	Vector2(const Vector2 &cpy) :
		TVector<glm::vec2>(cpy.rawVector),
		x(rawVector.x), y(rawVector.y) {
	}
	Vector2 &operator=(const Vector2 &other) {
		x = other.x;
		y = other.y;
		return *this;
	};

	operator Vector2i();
	operator Vector3i();
	operator Vector4i();
	operator Vector3();
	operator Vector4();
};
class Vector3 : public TVector<glm::vec3> {
public:
	using TVector<glm::vec3>::Type;

	float &x, &y, &z;

	Vector3(Type init = Type(0)) :
		TVector<glm::vec3>(init),
		x(rawVector.x), y(rawVector.y), z(rawVector.z) {
	}
	Vector3(int x, int y, int z) :
		TVector<glm::vec3>(Type(x, y, z)),
		x(rawVector.x), y(rawVector.y), z(rawVector.z) {
	}
	Vector3(const Vector &cpy) :
		TVector<glm::vec3>(cpy),
		x(rawVector.x), y(rawVector.y), z(rawVector.z) {
	}
	Vector3(const Vector3 &cpy) :
		TVector<glm::vec3>(cpy.rawVector),
		x(rawVector.x), y(rawVector.y), z(rawVector.z) {
	}
	Vector3 &operator=(const Vector3 &other) {
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	};

	operator Vector2i();
	operator Vector3i();
	operator Vector4i();
	operator Vector2();
	operator Vector4();
};
class Vector4 : public TVector<glm::vec4> {
public:
	using TVector<glm::vec4>::Type;

	float &x, &y, &z, &w;

	Vector4(Type init = Type(0)) :
		TVector<glm::vec4>(init),
		x(rawVector.x), y(rawVector.y), z(rawVector.z), w(rawVector.w) {
	}
	Vector4(int x, int y, int z, int w) :
		TVector<glm::vec4>(Type(x, y, z, w)),
		x(rawVector.x), y(rawVector.y), z(rawVector.z), w(rawVector.w) {
	}
	Vector4(const Vector &cpy) :
		TVector<glm::vec4>(cpy),
		x(rawVector.x), y(rawVector.y), z(rawVector.z), w(rawVector.w) {
	}
	Vector4(const Vector4 &cpy) :
		TVector<glm::vec4>(cpy.rawVector),
		x(rawVector.x), y(rawVector.y), z(rawVector.z), w(rawVector.w) {
	}
	Vector4 &operator=(const Vector4 &other) {
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
		return *this;
	};

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