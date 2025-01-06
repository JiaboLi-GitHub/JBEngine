#include "skinnedMesh.h"

namespace JB
{
	SkinnedMesh::Ptr SkinnedMesh::create(const Geometry::Ptr& geometry, const Material::Ptr& material)
	{
		return std::shared_ptr<SkinnedMesh>(new SkinnedMesh(geometry, material));
	}

	SkinnedMesh::~SkinnedMesh()
	{
	}

	void SkinnedMesh::bind(const Skeleton::Ptr& skeleton)
	{
		m_skeleton = skeleton;
	}

	Skeleton::Ptr SkinnedMesh::getSkeleton() const
	{
		return m_skeleton;
	}

	SkinnedMesh::SkinnedMesh(const Geometry::Ptr& geometry, const Material::Ptr& material)
		: Mesh(geometry, material)
	{
		m_type = Object3DType::SkinnedMesh;
	}
}
