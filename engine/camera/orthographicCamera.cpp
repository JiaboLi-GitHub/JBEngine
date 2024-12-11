#include "orthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace JB
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float near, float far)
	{
		m_type = Object3DType::OrthographicCamera;
		m_left = left;
		m_right = right;
		m_bottom = bottom;
		m_top = top;
		m_near = near;
		m_far = far;

		updateProjectionMatrix();
	}

	OrthographicCamera::Ptr OrthographicCamera::create(float left, float right, float bottom, float top, float near, float far)
	{
		return std::shared_ptr<OrthographicCamera>(new OrthographicCamera(left, right, bottom, top, near, far));
	}

	OrthographicCamera::~OrthographicCamera()
	{
	}

	void OrthographicCamera::updateProjectionMatrix()
	{
		m_projectionMatrix = glm::ortho(m_left, m_right, m_bottom, m_top, m_near, m_far);
	}

	void OrthographicCamera::setPerspective(float left, float right, float bottom, float top, float near, float far)
	{
		m_left = left;
		m_right = right;
		m_bottom = bottom;
		m_top = top;
		m_near = near;
		m_far = far;

		updateProjectionMatrix();
	}
}
