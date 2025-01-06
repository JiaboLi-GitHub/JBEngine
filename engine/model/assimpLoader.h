#pragma once

#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/quaternion.hpp>

#include "../global/constant.h"
#include "../material/material.h"
#include "../objects/object3D.h"
#include "../texture/texture.h"
#include "../model/bone.h"
#include "../model/skeleton.h"
#include "../model/skinnedMesh.h"
#include "../animation/animationAction.h"

namespace JB
{
	struct AssimpResult 
	{
		using Ptr = std::shared_ptr<AssimpResult>;

		std::vector<AnimationAction::Ptr> actions;
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
	
		static void buildNode(const aiNode* node, const aiScene* scene, Object3D::Ptr parentObject, const std::vector<Material::Ptr>& materials, const std::vector<Bone::Ptr>& bones);
	
		static Object3D::Ptr buildMesh(const aiMesh* mesh, const aiScene* scene, const glm::mat4 localTransform, const std::vector<Material::Ptr>& materials, const std::vector<Bone::Ptr>& bones);
	
		static glm::mat4 getGLMMat4(aiMatrix4x4 value);

		static void buildSkeleton(const aiNode* node, const aiScene* scene, Bone::Ptr parentBone, std::vector<Bone::Ptr>& bones);

		static Bone::Ptr AssimpLoader::getBone(const std::string& name, const std::vector<Bone::Ptr>& bones);

		static void setVertexSkinData(const uint32_t& id, const float& boneId, const float& skinWeight, std::vector<float>& skinIndices, std::vector<float>& skinWeights);
	
		static std::vector<AnimationClip::Ptr> processAnimation(const aiScene* scene);

		static glm::vec3 getGLMVec3(aiVector3D value);

		static glm::quat getGLMQuat(aiQuaternion value);
	};
}
