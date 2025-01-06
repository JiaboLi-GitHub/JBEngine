#include "lightShadow.h"

namespace JB
{
	LightShadow::LightShadow(const Camera::Ptr camera)
	{
		m_camera = camera;

		m_renderTarget = std::make_shared<RenderTarget>();
		m_renderTarget->setSize(1024 * 1, 1024 * 1);
		m_renderTarget->setUseRbo(false);
	}

	LightShadow::~LightShadow()
	{
	}

	Camera::Ptr LightShadow::getCamera()
	{
		return m_camera;
	}

	RenderTarget::Ptr LightShadow::getRenderTarget()
	{
		return m_renderTarget;
	}
}
