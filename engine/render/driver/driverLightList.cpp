#include "driverLightList.h"

namespace JB
{
	DriverLightList::DriverLightList()
	{
		init();
	}

	DriverLightList::~DriverLightList()
	{
	}

	void DriverLightList::push(const std::weak_ptr<Light>& light)
	{
		DriverLight driverLight;
		driverLight.light = light;
		driverLight.driverShadow = std::make_shared<DriverShadow>(light);
		m_driverLights.push_back(driverLight);
	}

	std::vector<DriverLight> DriverLightList::getAllDriverLights()
	{
		return m_driverLights;
	}

	std::vector<DriverLight> DriverLightList::getDriverDirLights()
	{
		std::vector<DriverLight> driverDirLights;
		for (auto& driverLight : m_driverLights)
		{
			auto lightsPtr = driverLight.light.lock();
			if (lightsPtr->getLightsType() == LightsType::DirectionalLights)
			{
				auto directionalLightsPtr = std::static_pointer_cast<DirectionalLight>(lightsPtr);
				driverDirLights.push_back(driverLight);
			}
		}
		return driverDirLights;
	}

	void DriverLightList::clear()
	{
		m_driverLights.clear();
	}

	void DriverLightList::init()
	{
	}
}
