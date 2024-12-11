#pragma once
#include <memory>
#include "camera.h"


namespace JB
{
	class PerspectiveCamera : public Camera
	{
	public:
		using Ptr = std::shared_ptr<PerspectiveCamera>;

		static PerspectiveCamera::Ptr create(float fov, float aspect, float near, float far);

		~PerspectiveCamera();

		virtual void updateProjectionMatrix() override;

		void setPerspective(float fov, float aspect, float near, float far);

	protected:
		PerspectiveCamera(float fov, float aspect, float near, float far);

	private:
		float m_fov;
		float m_aspect;
		float m_near;
		float m_far;
	};
}
