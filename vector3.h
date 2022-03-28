#ifndef VECTOR_3_H
#define VECTOR_3_H
#include <d3d9.h>
#include <math.h>
#define UCONST_Pi 3.1415926535
#define RadianToURotation 180.0f / UCONST_Pi

class Vector3 {

public:

    // -------------------- Attributes -------------------- //

    // Components of the vector
    float x, y, z;

    // -------------------- Methods -------------------- //

    // Constructor
    Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

    // Constructor
    Vector3(const Vector3& vector) : x(vector.x), y(vector.y), z(vector.z) {}

    // Constructor
    ~Vector3() {}

    // = operator
    Vector3& operator=(const Vector3& vector) {
        if (&vector != this) {
            x = vector.x;
            y = vector.y;
            z = vector.z;
        }
        return *this;
    }

    // + operator
    Vector3 operator+(const Vector3& v) const {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }

    // += operator
    Vector3& operator+=(const Vector3& v) {
        x += v.x; y += v.y; z += v.z;
        return *this;
    }

    // - operator
    Vector3 operator-(const Vector3& v) const {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }

    // -= operator
    Vector3& operator-=(const Vector3& v) {
        x -= v.x; y -= v.y; z -= v.z;
        return *this;
    }

    // == operator
    bool operator==(const Vector3& v) const {
        return x == v.x && y == v.y && z == v.z;
    }

    // != operator
    bool operator!=(const Vector3& v) const {
        return !(*this == v);
    }

    // * operator
    Vector3 operator*(float f) const {
        return Vector3(f * x, f * y, f * z);
    }

    // *= operator
    Vector3& operator*=(float f) {
        x *= f; y *= f; z *= f;
        return *this;
    }

    // / operator
    Vector3 operator/(float f) const {
        assert(f > std::numeric_limits<float>::epsilon());
        float inv = 1.f / f;
        return Vector3(x * inv, y * inv, z * inv);
    }

    // /= operator
    Vector3& operator/=(float f) {
        assert(f > std::numeric_limits<float>::epsilon());
        float inv = 1.f / f;
        x *= inv; y *= inv; z *= inv;
        return *this;
    }

    // - operator
    Vector3 operator-() const {
        return Vector3(-x, -y, -z);
    }

    // [] operator
    float& operator[](int i) {
        assert(i >= 0 && i <= 2);
        switch (i) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        }
        return z;
    }

    // [] operator
    const float& operator[](int i) const {
        assert(i >= 0 && i <= 2);
        switch (i) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        }
        return z;
    }

    // Cross product operator
    Vector3 cross(const Vector3& v) const {
        return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }

    // Dot product operator
    float dot(const Vector3& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    // Normalize the vector and return it
    Vector3 normalize() {
        float l = length();
        if (l < std::numeric_limits<float>::epsilon()) {
            assert(false);
        }
        x /= l;
        y /= l;
        z /= l;
        return *this;
    }

    bool isNull() const {
        return(x == 0. && y == 0. && z == 0.);
    }

    // Clamp the values between 0 and 1
    Vector3 clamp01() {
        if (x > 1.f) x = 1.f;
        else if (x < 0.f) x = 0.f;
        if (y > 1.f) y = 1.f;
        else if (y < 0.f) y = 0.f;
        if (z > 1.f) z = 1.f;
        else if (z < 0.f) z = 0.f;
        return *this;
    }

    inline float Distance(Vector3 v) {
        return float(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
    }

    inline float Dot(Vector3 v) {
        return x * v.x + y * v.y + z * v.z;
    }


    // Return the squared length of the vector
    float lengthSquared() const { return x * x + y * y + z * z; }

    // Return the length of the vector
    float length() const { return sqrt(lengthSquared()); }
};


struct Vector2 {
public:
	float x;
	float y;

	inline Vector2() : x(0), y(0) {}
	inline Vector2(float x, float y) : x(x), y(y) {}

	inline float Distance(Vector2 v) {
		return sqrtf(((v.x - x) * (v.x - x) + (v.y - y) * (v.y - y)));
	}

	inline Vector2 operator+(const Vector2& v) const {
		return Vector2(x + v.x, y + v.y);
	}

	inline Vector2 operator-(const Vector2& v) const {
		return Vector2(x - v.x, y - v.y);
	}
};

struct FQuat {
	float x;
	float y;
	float z;
	float w;
};

struct FTransform {
	FQuat rot;
	Vector3 translation;
	char pad[4];
	Vector3 scale;
	char pad1[4];
	D3DMATRIX ToMatrixWithScale() {
		D3DMATRIX m;
		m._41 = translation.x;
		m._42 = translation.y;
		m._43 = translation.z;

		float x2 = rot.x + rot.x;
		float y2 = rot.y + rot.y;
		float z2 = rot.z + rot.z;

		float xx2 = rot.x * x2;
		float yy2 = rot.y * y2;
		float zz2 = rot.z * z2;
		m._11 = (1.0f - (yy2 + zz2)) * scale.x;
		m._22 = (1.0f - (xx2 + zz2)) * scale.y;
		m._33 = (1.0f - (xx2 + yy2)) * scale.z;

		float yz2 = rot.y * z2;
		float wx2 = rot.w * x2;
		m._32 = (yz2 - wx2) * scale.z;
		m._23 = (yz2 + wx2) * scale.y;

		float xy2 = rot.x * y2;
		float wz2 = rot.w * z2;
		m._21 = (xy2 - wz2) * scale.y;
		m._12 = (xy2 + wz2) * scale.x;

		float xz2 = rot.x * z2;
		float wy2 = rot.w * y2;
		m._31 = (xz2 + wy2) * scale.z;
		m._13 = (xz2 - wy2) * scale.x;

		m._14 = 0.0f;
		m._24 = 0.0f;
		m._34 = 0.0f;
		m._44 = 1.0f;

		return m;
	}
};

struct FMinimalViewInfo
{
	Vector3 Location; //+ 0x1260
	Vector3 Rotation; //+ 0x126C
	float FOV;     //+ 0x1278
};

class FRotator
{
public:
	float Pitch = 0.f;
	float Yaw = 0.f;
	float Roll = 0.f;

	D3DMATRIX GetAxes() {
		auto tempMatrix = Matrix();
		return tempMatrix;
	}

	D3DMATRIX Matrix(Vector3 origin = Vector3(0, 0, 0)) {
		float radPitch = (Pitch * float(UCONST_Pi) / 180.f);
		float radYaw = (Yaw * float(UCONST_Pi) / 180.f);
		float radRoll = (Roll * float(UCONST_Pi) / 180.f);
		float SP = sinf(radPitch);
		float CP = cosf(radPitch);
		float SY = sinf(radYaw);
		float CY = cosf(radYaw);
		float SR = sinf(radRoll);
		float CR = cosf(radRoll);

		D3DMATRIX matrix;
		matrix.m[0][0] = CP * CY;
		matrix.m[0][1] = CP * SY;
		matrix.m[0][2] = SP;
		matrix.m[0][3] = 0.f;

		matrix.m[1][0] = SR * SP * CY - CR * SY;
		matrix.m[1][1] = SR * SP * SY + CR * CY;
		matrix.m[1][2] = -SR * CP;
		matrix.m[1][3] = 0.f;

		matrix.m[2][0] = -(CR * SP * CY + SR * SY);
		matrix.m[2][1] = CY * SR - CR * SP * SY;
		matrix.m[2][2] = CR * CP;
		matrix.m[2][3] = 0.f;

		matrix.m[3][0] = origin.x;
		matrix.m[3][1] = origin.y;
		matrix.m[3][2] = origin.z;
		matrix.m[3][3] = 1.f;

		return matrix;
	}
};
#endif