#include "scene.h"
#include "../geometries/boxGeometry.h"
#include "../material/cubeMaterial.h"
#include "../objects/mesh.h"

namespace JB
{
	Scene::Ptr Scene::create()
	{
		return std::shared_ptr<Scene>(new Scene());
	}

	Scene::~Scene()
	{
	}

	void Scene::setSkyBoxTexture(const TextureCube::Ptr& textureCube)
	{
		textureCube->setTextureFilterMag(TextureFilter::LinearFilter);
		textureCube->setTextureFilterMin(TextureFilter::LinearFilter);
		textureCube->setTextureWrappingS(TextureWrapping::ClampToEdgeWrapping);
		textureCube->setTextureWrappingT(TextureWrapping::ClampToEdgeWrapping);
		textureCube->setTextureWrappingR(TextureWrapping::ClampToEdgeWrapping);

		auto boxGeometry = std::make_shared<BoxGeometry>(2.0f, 2.0f, 2.0f);

		auto material = std::make_shared<CubeMaterial>();
		material->setTextureCube(textureCube);


		m_skyBox = Mesh::create(boxGeometry, material);

		RenderableObject::OnBeforeRenderCallback fun = [&](Renderer* render, Scene* scene, Camera* camera) {
			m_skyBox->setPosition(camera->getWorldPosition());
			m_skyBox->updateModelMatrix();
			};
		m_skyBox->setOnBeforeRenderCallback(fun);
	}

	Mesh::Ptr Scene::getSkyBox()
	{
		return m_skyBox;
	}

	void Scene::setMaterial(const Material::Ptr& material)
	{
		m_material = material;
	}

	Material::Ptr Scene::getMaterial()
	{
		return m_material;
	}

	Scene::Scene()
	{
	}
}
