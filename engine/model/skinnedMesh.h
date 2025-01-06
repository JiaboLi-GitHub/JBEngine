#pragma once

#include "../objects/mesh.h"
#include "skeleton.h"

namespace JB
{
	class SkinnedMesh : public Mesh
	{
	public:
		using Ptr = std::shared_ptr<SkinnedMesh>;

		static Ptr create(const Geometry::Ptr& geometry, const Material::Ptr& material);

		~SkinnedMesh();

		void bind(const Skeleton::Ptr& skeleton);

		Skeleton::Ptr getSkeleton() const;

	private:
		SkinnedMesh(const Geometry::Ptr& geometry, const Material::Ptr& material);

	private:
		Skeleton::Ptr	m_skeleton = nullptr;
	};
}
