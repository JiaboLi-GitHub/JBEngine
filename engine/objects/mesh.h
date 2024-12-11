#pragma once
#include "renderableObject.h"

namespace JB
{
	class Mesh : public RenderableObject
	{
	public:
		using Ptr = std::shared_ptr<Mesh>;

		static Mesh::Ptr create(Geometry::Ptr geometry, Material::Ptr material);

		~Mesh();

	protected:
		Mesh(Geometry::Ptr geometry, Material::Ptr material);
	};
}
