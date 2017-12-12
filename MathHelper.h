#ifndef _MATHHELPER_H
#define _MATHHELPER_H

#include <math.h>

#define PI 3.14159265
#define DEG_TO_RAD PI / 180.0f

struct Vector2 {

	float x;
	float y;

	Vector2(float _x = 0.0f, float _y = 0.0f)
		:x(_x), y(_y) {}

	float MagnitudeSqr() {

		return x*x + y*y;
	}

	float Magnitude() {

		return (float)sqrt(x*x + y*y);
	}

	Vector2 Normalized() {

		float mag = Magnitude();

		return Vector2(x / mag, y / mag);

	}

	Vector2& operator +=(const Vector2& rhs) {

		x += rhs.x;
		y += rhs.y;

		return *this;
	}

	Vector2& operator -=(const Vector2& rhs) {

		x -= rhs.x;
		y -= rhs.y;

		return *this;
	}

	Vector2 operator -() const {

		return Vector2(-x, -y);
	}
};

//addition
inline Vector2 operator +(const Vector2& lhs, const Vector2& rhs) {

	return Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
}

//subtraction
inline Vector2 operator -(const Vector2& lhs, const Vector2& rhs) {

	return Vector2(lhs.x - rhs.x, lhs.y - rhs.y);
}

//mult
inline Vector2 operator *(const Vector2& lhs, const float& rhs) {

	return Vector2(lhs.x * rhs, lhs.y * rhs);
}

//mult
inline Vector2 operator *(const float& lhs, const Vector2& rhs) {

	return Vector2(lhs * rhs.x, lhs * rhs.y);
}

//rotating vectors
inline Vector2 RotateVector(Vector2& vec, float angle) {

	float radAngle = (float)(angle * DEG_TO_RAD);

	return Vector2((float)(vec.x * cos(radAngle) - vec.y * sin(radAngle)), (float)(vec.x * sin(radAngle) + vec.y * cos(radAngle)));
}

//lerp
inline Vector2 Lerp(Vector2& start, Vector2& end, float time) {

	if (time <= 0.0f) {
		return start;
	}

	if (time >= 1.0f) {
		return end;
	}

	Vector2 dir = (end - start).Normalized();
	float mag = (end - start).Magnitude();

	return start + dir * mag * time;
}

const Vector2 VEC2_ZERO = { 0.0f, 0.0f };
const Vector2 VEC2_ONE = { 1.0f, 1.0f };
const Vector2 VEC2_UP = { 0.0f, 1.0f };
const Vector2 VEC2_RIGHT = { 1.0f, 0.0f };

struct BezierCurve {

	Vector2 p0;
	Vector2 p1;
	Vector2 p2;
	Vector2 p3;

	Vector2 CalculateCurvePoint(float t) {

		float tt = t * t;
		float ttt = tt * t;
		float u = 1.0f - t;
		float uu = u * u;
		float uuu = uu * u;

		Vector2 point = (uuu * p0) + (3 * uu * t * p1) + (3 * u * tt * p2) + (ttt * p3);
		point.x = round(point.x);
		point.y = round(point.y);
		return point;
	}
};

#endif