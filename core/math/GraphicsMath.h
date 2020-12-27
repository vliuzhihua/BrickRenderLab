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
		auto Right = Forward.Cross(Up);
		auto OrthoUp = Right.Cross(Forward);
		//Matrix4x4f Rotation = {
		//	Right.x, OrthoUp.x, Forward.x, 0.f,
		//	Right.y, OrthoUp.y, Forward.y, 0.f,
		//	Right.z, OrthoUp.z, Forward.z, 0.f,
		//	0.0f, 0.0f, 0.0f, 1.0f
		//};
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
			Near/HalfNearWidth,	0.0f,	0.0f,	0.0f,
			0.0f,	Near/HalfNearHeight,	0.0f,	0.0f,
			0.0f,	0.0f,	A,		B,
			0.0f,	0.0f,	1.0f,	0.0f
		};
		return data;
	}
}

}
