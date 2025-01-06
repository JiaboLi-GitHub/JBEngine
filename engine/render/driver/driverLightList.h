#pragma once
#include "driverShadow.h"
#include "../../lights/light.h"
#include "../../lights/directionalLight.h"

namespace JB
{
	struct DriverLight
	{
		std::weak_ptr<Light> light;
		DriverShadow::Ptr driverShadow;
	};


	class DriverLightList
	{
	public:
		using Ptr = std::shared_ptr<DriverLightList>;

		DriverLightList();

		~DriverLightList();

		void push(const std::weak_ptr<Light>& light);

		std::vector<DriverLight> getAllDriverLights();

		std::vector<DriverLight> getDriverDirLights();

		void clear();

	private:
		void init();

	private:
		std::vector<DriverLight> m_driverLights;
	};
}

