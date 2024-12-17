#pragma once

#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../global/constant.h"
#include "../material/material.h"
#include "../objects/object3D.h"
#include "../texture/texture.h"

namespace JB
{
	struct AssimpResult
	{
		using Ptr = std::shared_ptr<AssimpResult>;

		//std::vector<AnimationAction::Ptr> mActions;
		Object3D::Ptr	object3D = nullptr;
	};

	class AssimpLoader
	{
	public:
		AssimpLoader();

		~AssimpLoader();

		static AssimpResult::Ptr load(const std::string& path);

	private:
		static void buildMaterial(const aiScene* scene, const std::string& rootPath, std::vector<Material::Ptr>& materials);

		static Texture::Ptr buildTexture(const aiTextureType& type, const aiScene* scene, const aiMaterial* material, const std::string& rootPath);

		static void buildNode(const aiNode* node, const aiScene* scene, Object3D::Ptr parentObject, const std::vector<Material::Ptr>& materials);

		static Object3D::Ptr buildMesh(const aiMesh* mesh, const aiScene* scene, const glm::mat4 localTransform, const std::vector<Material::Ptr>& materials);

		static glm::mat4 getGLMMat4(aiMatrix4x4 value);

	};
}
