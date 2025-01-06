#pragma once
#include "driverRenderList.h"
#include "driverRenderTarget.h"
#include "../../lights/light.h"
#include "../../scene/scene.h"

namespace JB
{
	class Renderer;
	class DriverShadow
	{
	public:
		using Ptr = std::shared_ptr<DriverShadow>;

		DriverShadow(std::weak_ptr<Light> light);

		~DriverShadow();

		void render(std::shared_ptr<Renderer> renderer, DriverRenderList::Ptr driverRenderList, Scene::Ptr scene);

		DriverRenderTarget::Ptr getDriverRenderTarget();

	private:
		std::weak_ptr<Light> m_light;
		DriverRenderTarget::Ptr m_driverRenderTarget = nullptr;
	};
}
