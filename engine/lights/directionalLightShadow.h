#pragma once
#include "lightShadow.h"

namespace JB
{
	class DirectionalLightShadow : public LightShadow
	{
	public:
		using Ptr = std::shared_ptr<DirectionalLightShadow>;

		DirectionalLightShadow();

		~DirectionalLightShadow();

	};
}
