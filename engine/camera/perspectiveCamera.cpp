#include "perspectiveCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace JB
{
	PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float near, float far)
	{
		m_type = Object3DType::PerspectiveCamera;
		m_fov = fov;
		m_aspect = aspect;
		m_near = near;
		m_far = far;

		updateProjectionMatrix();
	}

	PerspectiveCamera::Ptr PerspectiveCamera::create(float fov, float aspect, float near, float far)
	{
		return std::shared_ptr<PerspectiveCamera>(new PerspectiveCamera(fov, aspect, near, far));
	}

	PerspectiveCamera::~PerspectiveCamera()
	{
	}

	void PerspectiveCamera::updateProjectionMatrix()
	{
		m_projectionMatrix = glm::perspective(m_fov, m_aspect, m_near, m_far);
	}

	void PerspectiveCamera::setPerspective(float fov, float aspect, float near, float far)
	{
		m_fov = fov;
		m_aspect = aspect;
		m_near = near;
		m_far = far;
		
		updateProjectionMatrix();
	}
}
