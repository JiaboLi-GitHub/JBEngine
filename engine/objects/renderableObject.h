#pragma once
#include <functional>
#include "../objects/object3D.h"
#include "../geometries/geometry.h"
#include "../material/material.h"
#include "../scene/scene.h"
#include "../camera/camera.h"

namespace JB
{
	class Renderer;
	class RenderableObject : public Object3D 
	{
	public:
		using Ptr = std::shared_ptr<RenderableObject>;

		using OnBeforeRenderCallback = std::function<void(Renderer*, Scene*, Camera*)>;

		using OnBeforeRenderCallback = std::function<void(Renderer*, Scene*, Camera*)>;

		static RenderableObject::Ptr create(Geometry::Ptr geometry, Material::Ptr material);

		~RenderableObject();

		Geometry::Ptr getGeometry() const;

		Material::Ptr getMaterial() const;

		void setOnBeforeRenderCallback(OnBeforeRenderCallback fun);

		//在本物体渲染前，会调用本函数，允许用户指定渲染前做哪些处理
		void onBeforeRender(Renderer* renderer, Scene* scene, Camera* camera);

	protected:
		RenderableObject(Geometry::Ptr geometry, Material::Ptr material);

	protected:
		Geometry::Ptr m_geometry;
		Material::Ptr m_material;
		OnBeforeRenderCallback m_onBeforeRenderCallback;
	};
}
