#pragma once
#include "Math3d.h"
//#include <math>
#include <cmath>

namespace brick {

namespace math {
	/*
	World Matrix:	Left Hand Coordinate
	View Matirx:	Left Hand Coordinate

	*/
	const float PI = 3.1415926;

	inline float DegreeToRadian(float Degree) {
		return Degree / 180.f * PI;
	}

	inline float RadianToDefree(float Radian) {
		return Radian / PI * 180.f;
	}

	inline Matrix4x4f LookAt(const Vector3f& Eye, const Vector3f& Forward, const Vector3f& Up) {
		//cross product need right hand, so reverse the crose value
		auto Right = Up.Cross(Forward);
		auto OrthoUp = Forward.Cross(Right);
		Matrix4x4f Rotation = {
			Right.x,	Right.y,	Right.z,	0.f,
			OrthoUp.x,	OrthoUp.y,	OrthoUp.z,	0.f,
			Forward.x,	Forward.y,	Forward.z,	0.f,
			0.0f,		0.0f,		0.0f,		1.0f
		};
		Matrix4x4f Transform = {
			1.0f,	0.0f,	0.0f,	-Eye.x,
			0.0f,	1.0f,	0.0f,	-Eye.y,
			0.0f,	0.0f,	1.0f,	-Eye.z,
			0.0f,	0.0f,	0.0f,	1.0f
			};
		return Rotation * Transform;
	}

	inline Matrix4x4f Scale(const Vector3f& S) {
		float data[] = {
			S.x,	0.0f,	0.0f,	0.0f,
			0.0f,	S.y,	0.0f,	0.0f,
			0.0f,	0.0f,	S.z,	0.0f,
			0.0f,	0.0f,	0.0f,	1.0f
		};
		return data;
	}
	
	inline Matrix4x4f RotateAroundX(const float Radian) {
		float data[] = {
			1.0f,			0.0f,			0.0f,			0.0f,
			0.0f,			cos(Radian),	sin(Radian),	0.0f,
			0.0f,			-sin(Radian),	cos(Radian),	0.0f,
			0.0f,			0.0f,			0.0f,			1.0f
		};
		return data;
	}

	inline Matrix4x4f RotateAroundY(const float Radian) {
		float data[] = {
			cos(Radian),	0.0f,		-sin(Radian),	0.0f,
			0.0f,			1.0f,		0.0f,			0.0f,
			sin(Radian),	0.0f,		cos(Radian),	0.0f,
			0.0f,			0.0f,		0.0f,			1.0f
		};
		return data;
	}

	inline Matrix4x4f RotateAroundZ(const float Radian) {
		float data[] = {
			cos(Radian),	sin(Radian),	0.0f,	0.0f,
			-sin(Radian),	cos(Radian),	0.0f,	0.0f,
			0.0f,			0.0f,			1.0f,	0.0f,
			0.0f,			0.0f,			0.0f,	1.0f
		};
		return data;
	}
	
	inline Matrix4x4f Perspective(float Fov, float Ratio, float Near, float Far) {
		const float NdcNear = -1.0f;
		const float NdcFar = 1.0f;
		float Diff = Far - Near;
		float HalfNearWidth = Near * std::tan(Fov * 0.5);
		float HalfNearHeight = HalfNearWidth / Ratio;
		float A = (Far * NdcFar - Near * NdcNear) / Diff;
		float B = (Near * Far * NdcNear - Near * Far * NdcFar) / Diff;
		float data[] = {
			Near/HalfNearWidth,	0.0f,					0.0f,	0.0f,
			0.0f,				Near/HalfNearHeight,	0.0f,	0.0f,
			0.0f,				0.0f,					A,		B,
			0.0f,				0.0f,					1.0f,	0.0f
		};
		return data;
	}
	
	template<typename T>
	inline Matrix4x4<T> EulerToMatrix(Vector3f EulerRadian) {
		float x_angle = EulerRadian.x;
		float y_angle = EulerRadian.y;
		float z_angle = EulerRadian.z;
		T xc = (T)cos(x_angle), xs = (T)sin(x_angle);
		T yc = (T)cos(y_angle), ys = (T)sin(y_angle);
		T zc = (T)cos(z_angle), zs = (T)sin(z_angle);
		Matrix4x4<T> Mat = Matrix4x4<T>::Identity();
		//case EULER_ORDER_XYZ:
		{
			Mat(0, 0) = yc * zc;
			Mat(1, 0) = -xc * zs + zc * xs * ys;
			Mat(2, 0) = xs * zs + xc * zc * ys;

			Mat(0, 1) = yc * zs;
			Mat(1, 1) = xc * zc + xs * ys * zs;
			Mat(2, 1) = -xs * zc + xc * ys * zs;

			Mat(0, 2) = -ys;
			Mat(1, 2) = xs * yc;
			Mat(2, 2) = xc * yc;
		}
		return Mat;
		return math::RotateAroundX(EulerRadian.x) *math::RotateAroundY(EulerRadian.y);
	}
}

}
