#pragma once
#include "camera.h"

namespace JB
{
	//正交投影相机
	class OrthographicCamera : public Camera
	{
	public:
		using Ptr = std::shared_ptr<OrthographicCamera>;

		static OrthographicCamera::Ptr create(float left, float right, float bottom, float top, float near, float far);

		~OrthographicCamera();

		virtual void updateProjectionMatrix() override;

		void setPerspective(float left, float right, float bottom, float top, float near, float far);

	protected:
		OrthographicCamera(float left, float right, float bottom, float top, float near, float far);

	private:
		float m_left;
		float m_right;
		float m_bottom;
		float m_top;
		float m_near;
		float m_far;
	};
}
