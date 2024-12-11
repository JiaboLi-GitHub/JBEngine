#include "camera.h"

namespace JB
{
	Camera::Camera()
	{
		m_type = Object3DType::Camera;
		m_modelMatrixInverse = glm::mat4(1.0f);
		m_projectionMatrix = glm::mat4(1.0f);
	}

	Camera::~Camera()
	{
	}

	void Camera::updateModelMatrix(bool updateParent, bool updateChildren)
	{
		Object3D::updateModelMatrix(updateParent, updateChildren);

		m_modelMatrixInverse = glm::inverse(m_modelMatrix);
	}

	glm::mat4x4 Camera::getModelMatrixInverse()
	{
		return m_modelMatrixInverse;
	}

	glm::mat4x4 Camera::getProjectionMatrix()
	{
		return m_projectionMatrix;
	}
}
