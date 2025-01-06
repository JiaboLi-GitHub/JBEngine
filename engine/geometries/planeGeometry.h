#pragma once

#include "geometry.h"

namespace JB
{
	class PlaneGeometry : public Geometry
	{
	public:
		using Ptr = std::shared_ptr<PlaneGeometry>;

		PlaneGeometry();

		~PlaneGeometry();
	};
}

