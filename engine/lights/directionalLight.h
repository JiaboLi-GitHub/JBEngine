#pragma once
#include "light.h"

namespace JB
{
	class DirectionalLight : public Light
	{
	public:
		using Ptr = std::shared_ptr<DirectionalLight>;

		DirectionalLight();

		~DirectionalLight();

	private:
	};
}
