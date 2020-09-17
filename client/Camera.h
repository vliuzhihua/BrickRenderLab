#pragma once
#include "Eigen/StdVector"
//#include "Eigen/Eigen"
#include "Eigen/Dense"
#include "Math.h"
#include "glm/gtc/matrix_transform.hpp"

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
		m_position = { 0, 0, 0};
		m_axis[RIGHT] = { 1, 0, 0};
		m_axis[UP] = { 0, 1, 0 };
		m_axis[FORWARD] = { 0, 0, -1};
	}

	template<Direction dir>
	void Move(float distance = 0.1)
	{
		m_position += m_axis[dir] * distance;
	}

	void Rotate(Eigen::Vector3f euler_degree)
	{
		auto rotate_func = [&](float degree, Eigen::Vector3f axis)
		{
			Eigen::AngleAxisf rotate(degree, axis);

			m_axis[RIGHT] = rotate.toRotationMatrix() * m_axis[RIGHT];
			m_axis[UP] = rotate.toRotationMatrix() *  m_axis[UP];
			m_axis[FORWARD] = rotate.toRotationMatrix() *  m_axis[FORWARD];
		};

		glm::mat4 Trans = glm::mat4(1.0f);
		glm::rotate(Trans, euler_degree[0], glm::vec3(1.0, 0.0, 0.0));
		glm::rotate(Trans, euler_degree[1], glm::vec3(0.0, 1.0, 0.0));
		glm::rotate(Trans, euler_degree[2], glm::vec3(0.0, 0.0, 1.0));

		glm::mat4 OriginMat = { 
			m_axis[RIGHT][0], m_axis[RIGHT][1], m_axis[RIGHT][2], 0.0,
			m_axis[UP][0], m_axis[UP][1], m_axis[UP][2], 0.0,
			m_axis[FORWARD][0], m_axis[FORWARD][1], m_axis[FORWARD][2], 0.0,
			0.0, 0.0, 0.0, 1.0
		};

		glm::mat4 NewMat = Trans * OriginMat;// *Trans;

		//m_axis[RIGHT] = { NewMat[0][0], NewMat[0][1], NewMat[0][2] };
		//m_axis[UP] = { NewMat[1][0], NewMat[1][1], NewMat[1][2] };
		//m_axis[FORWARD] = { NewMat[2][0], NewMat[2][1], NewMat[2][2] };

		rotate_func(euler_degree[0], m_axis[RIGHT]);
		rotate_func(euler_degree[1], m_axis[UP]);
		rotate_func(euler_degree[2], m_axis[FORWARD]);
		
		//m_axis[UP] = Eigen::Cross(m_axis[RIGHT], m_axis[FORWARD]);
		//m_axis[UP] = m_axis[RIGHT].cross(m_axis[FORWARD]);

	}

//private:
	Eigen::Matrix4f m_transform;
	Eigen::Vector3f m_position;
	//0: Right; 1: Up; 2: Forward
	Eigen::Vector3f m_axis[3];
};
