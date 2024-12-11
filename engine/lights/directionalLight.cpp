#include "directionalLight.h"
#include "directionalLightShadow.h"

namespace JB
{
	DirectionalLight::DirectionalLight()
	{
		m_lightsType = LightsType::DirectionalLights;
		m_shadow = std::make_shared<DirectionalLightShadow>();
	}

	DirectionalLight::~DirectionalLight()
	{
	}
}
