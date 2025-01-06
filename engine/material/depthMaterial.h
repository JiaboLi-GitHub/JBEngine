#pragma once
#include "material.h"

namespace JB
{
	class DepthMaterial : public Material
	{
	public:
		using Ptr = std::shared_ptr<DepthMaterial>;

		DepthMaterial();

		~DepthMaterial();

	private:


	};
}

