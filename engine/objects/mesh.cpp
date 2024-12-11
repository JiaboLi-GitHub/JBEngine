#include "mesh.h"

namespace JB
{
	Mesh::Mesh(Geometry::Ptr geometry, Material::Ptr material)
		: RenderableObject(geometry, material)
	{
		m_type = Object3DType::Mesh;
	}

	Mesh::Ptr Mesh::create(Geometry::Ptr geometry, Material::Ptr material)
	{
		return std::shared_ptr<Mesh>(new Mesh(geometry, material));
	}

	Mesh::~Mesh()
	{
	}
}
