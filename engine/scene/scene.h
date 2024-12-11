#pragma once
#include "../global/constant.h"
#include "../objects/object3D.h"
#include "../texture/textureCube.h"
#include "../material/material.h"

namespace JB
{
	class Mesh;
	class Scene :public Object3D
	{
	public:
		using Ptr = std::shared_ptr<Scene>;

		static Scene::Ptr create();

		~Scene();

		void setSkyBoxTexture(const TextureCube::Ptr& textureCube);

		std::shared_ptr<Mesh> getSkyBox();

		void setMaterial(const Material::Ptr& material);

		Material::Ptr getMaterial();

	protected:
		Scene();

	private:
		std::shared_ptr<Mesh> m_skyBox = nullptr;
		Material::Ptr m_material = nullptr;
	};
}
