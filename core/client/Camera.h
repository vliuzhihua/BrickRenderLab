#pragma once
#include "math/GraphicsMath.h"

namespace brick {

class Camera
{
public:
	enum Direction
	{
		RIGHT,
		UP,
		FORWARD
	};

	Camera()
	{
		m_transform = math::Matrix4x4f::Identity();
	}

	template<Direction dir>
	void Move(float distance = 0.5)
	{
		SetPosition(GetPosition() +  GetDirVector<dir>() * distance);
		//SetPosition(GetPosition() +  GetDirVector<dir>());
		//SetPosition(GetPosition() + 1.0f);
	}

	void SetEulerRadian(math::Vector3f EulerRadian) {
		mEulerRadian = EulerRadian;
		UpdateTransform();
	}

	math::Vector3f GetEulerRadian() {
		return mEulerRadian;
	}

	void UpdateTransform() {
		auto Position = GetPosition();
		m_transform = math::EulerToMatrix(mEulerRadian);
		SetPosition(Position);

	}

	template<Direction dir>
	math::Vector3f GetDirVector() {
		if constexpr (dir == FORWARD) {
			return GetForward();
		}else if constexpr(dir == RIGHT) {
			return GetRight();
			}
		else {
			return GetUp();
		}
	}

	math::Vector3f GetForward() const {
		return { m_transform(0, 2), m_transform(1, 2), m_transform(2, 2) };
	}

	math::Vector3f GetRight() const {
		return { m_transform(0, 0), m_transform(1, 0), m_transform(2, 0) };
	}

	math::Vector3f GetUp() const {
		return { m_transform(0, 1), m_transform(1, 1), m_transform(2, 1) };
	}

	math::Vector3f GetPosition() const {
		return {m_transform(0, 3), m_transform(1, 3), m_transform(2, 3)};
	}

	void SetPosition(const math::Vector3f& Position) {
		m_transform(0, 3) = Position.x;
		m_transform(1, 3) = Position.y;
		m_transform(2, 3) = Position.z;
	}

	math::Matrix4x4f GetRotation() const {
		math::Matrix4x4f RotationMat(m_transform.GetData());
		RotationMat(0, 3) = 0.0f;
		RotationMat(1, 3) = 0.0f;
		RotationMat(2, 3) = 0.0f;
		return RotationMat;
	}

private:
	math::Matrix4x4f m_transform;
	math::Vector3f mEulerRadian = {0.f};
};

}
