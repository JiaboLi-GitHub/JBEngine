#include "directionalLightShadow.h"
#include "../camera/orthographicCamera.h"

namespace JB
{
	DirectionalLightShadow::DirectionalLightShadow()
		: LightShadow(OrthographicCamera::create(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.0f))
	{
	}

	DirectionalLightShadow::~DirectionalLightShadow()
	{
	}
}
