#include "renderableObject.h"

namespace JB
{
	RenderableObject::RenderableObject(Geometry::Ptr geometry, Material::Ptr material)
	{
		m_type = Object3DType::RenderableObject;
		m_geometry = geometry;
		m_material = material;
		m_onBeforeRenderCallback = nullptr;
	}

	RenderableObject::Ptr RenderableObject::create(Geometry::Ptr geometry, Material::Ptr material)
	{
		return std::shared_ptr<RenderableObject>(new RenderableObject(geometry, material));
	}

	RenderableObject::~RenderableObject()
	{
	}

	Geometry::Ptr RenderableObject::getGeometry() const
	{
		return m_geometry;
	}

	Material::Ptr RenderableObject::getMaterial() const
	{
		return m_material;
	}

	void RenderableObject::setOnBeforeRenderCallback(OnBeforeRenderCallback fun)
	{
		m_onBeforeRenderCallback = fun;
	}

	void RenderableObject::onBeforeRender(Renderer* renderer, Scene* scene, Camera* camera)
	{
		if (m_onBeforeRenderCallback)
		{
			m_onBeforeRenderCallback(renderer, scene, camera);
		}
	}
}
