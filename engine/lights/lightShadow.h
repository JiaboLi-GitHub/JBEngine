#pragma once
#include "../camera/camera.h"
#include "../render/renderTarget.h"

namespace JB
{
	class LightShadow
	{
	public:
		using Ptr = std::shared_ptr<LightShadow>;

		LightShadow(const Camera::Ptr camera);

		~LightShadow();

		Camera::Ptr getCamera();

		RenderTarget::Ptr getRenderTarget();

	private:
		Camera::Ptr m_camera = nullptr;
		RenderTarget::Ptr m_renderTarget = nullptr;
	};
}
