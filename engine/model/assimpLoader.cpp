#include "assimpLoader.h"

#include <iostream>

#include "../material/phongLightingMaterial.h"
#include "../texture/texture2D.h"
#include "../objects/group.h"
#include "../objects/mesh.h"
#include "../geometries/geometry.h"
#include "../geometries/attribute.h"
#include "../animation/keyframeTracks/vectorKeyframeTrack.h"
#include "../animation/keyframeTracks/quaternionKeyframeTrack.h"
#include "../animation/interpolant/linearInterpolant.h"
#include "../animation/interpolant/quaternionInterpolant.h"

namespace JB
{
	AssimpLoader::AssimpLoader()
	{
	}

	AssimpLoader::~AssimpLoader()
	{
	}

	AssimpResult::Ptr AssimpLoader::load(const std::string& path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "Error:model read fail!" << std::endl;
			return nullptr;
		}

		AssimpResult::Ptr result = std::make_shared<AssimpResult>();
		std::vector<Material::Ptr> materials;
		Object3D::Ptr rootObject = Group::create();

		Bone::Ptr rootBone = Bone::create();
		std::vector<Bone::Ptr> bones;

		std::size_t lastIndex = path.find_last_of("//");
		std::string rootPath = path.substr(0, lastIndex + 1);

		if (scene->mNumAnimations) 
		{
			buildSkeleton(scene->mRootNode, scene, rootBone, bones);
			rootObject->addChild(rootBone);
		}

		buildMaterial(scene, rootPath, materials);
		buildNode(scene->mRootNode, scene, rootObject, materials, bones);

		std::vector<AnimationAction::Ptr> actions;
		if (scene->mNumAnimations) 
		{
			auto clips = processAnimation(scene);
			for (uint32_t c = 0; c < clips.size(); ++c) 
			{
				auto action = std::make_shared<AnimationAction>(clips[c], rootObject);
				actions.push_back(action);
			}
		}

		result->object3D = rootObject;
		result->actions = actions;
		return result;
	}

	void AssimpLoader::buildMaterial(const aiScene* scene, const std::string& rootPath, std::vector<Material::Ptr>& materials)
	{
		for (uint32_t i = 0; i < scene->mNumMaterials; ++i)
		{
			Material::Ptr material = nullptr;

			aiMaterial* aimaterial = scene->mMaterials[i];

			aiShadingMode shadingMode = aiShadingMode::aiShadingMode_Phong;
			aimaterial->Get(AI_MATKEY_SHADING_MODEL, shadingMode);

			switch (shadingMode)
			{
			case aiShadingMode_Phong:
			{
				material = std::make_shared<PhongLightingMaterial>();
				break;
			}
			default:
			{
				material = std::make_shared<PhongLightingMaterial>();
				break;
			}
			}

			switch (material->getMaterialType())
			{
			case MaterialType::PhongLightingMaterial:
			{
				auto phongLightingMaterial = std::dynamic_pointer_cast<PhongLightingMaterial>(material);

				auto diffuseTexture2D = buildTexture(aiTextureType_DIFFUSE, scene, aimaterial, rootPath);
				phongLightingMaterial->setDiffuse(std::dynamic_pointer_cast<Texture2D>(diffuseTexture2D));

				auto specularTexture2D = buildTexture(aiTextureType_SPECULAR, scene, aimaterial, rootPath);
				phongLightingMaterial->setSpecular(std::dynamic_pointer_cast<Texture2D>(specularTexture2D));

				break;
			}
			default:
				break;
			}

			materials.push_back(material);
		}
	}

	Texture::Ptr AssimpLoader::buildTexture(const aiTextureType& type, const aiScene* scene, const aiMaterial* material, const std::string& rootPath)
	{
		Texture::Ptr texture = nullptr;

		aiString aiPath;
		material->Get(AI_MATKEY_TEXTURE(type, 0), aiPath);

		const aiTexture* assimpTexture = scene->GetEmbeddedTexture(aiPath.C_Str());
		if (assimpTexture)
		{
			//TODO 处理纹理内嵌模型
			unsigned char* dataIn = reinterpret_cast<unsigned char*>(assimpTexture->pcData);
			int widthIn = assimpTexture->mWidth;
			int heightIn = assimpTexture->mHeight;
			std::string path = aiPath.C_Str();

			texture = std::make_shared<Texture2D>(dataIn, widthIn, heightIn);
		}
		else
		{
			std::string fullPath = rootPath + aiPath.C_Str();
			texture = std::make_shared<Texture2D>(fullPath);
		}

		return texture;
	}

	void AssimpLoader::buildNode(const aiNode* node, const aiScene* scene, Object3D::Ptr parentObject, const std::vector<Material::Ptr>& materials, const std::vector<Bone::Ptr>& bones)
	{
		Group::Ptr group = Group::create();

		for (uint32_t i = 0; i < node->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

			group->addChild(buildMesh(mesh, scene, getGLMMat4(node->mTransformation), materials, bones));
		}

		parentObject->addChild(group);

		for (uint32_t i = 0; i < node->mNumChildren; i++)
		{
			buildNode(node->mChildren[i], scene, group, materials, bones);
		}
	}

	Object3D::Ptr AssimpLoader::buildMesh(const aiMesh* mesh, const aiScene* scene, const glm::mat4 localTransform, const std::vector<Material::Ptr>& materials, const std::vector<Bone::Ptr>& bones)
	{
		Object3D::Ptr object = nullptr;
		Material::Ptr material = nullptr;
		Geometry::Ptr geometry = std::make_shared<Geometry>();

		std::vector<float> positions;
		std::vector<float> normals;

		std::vector<float> uvs;

		std::vector<uint32_t> indices;

		//骨骼动画
		std::vector<Bone::Ptr> meshBones;
		std::vector<glm::mat4> offsetMatrices;
		std::vector<float> skinIndices;
		std::vector<float> skinWeights;

		for (uint32_t i = 0; i < mesh->mNumVertices; ++i) 
		{
			positions.push_back(mesh->mVertices[i].x);
			positions.push_back(mesh->mVertices[i].y);
			positions.push_back(mesh->mVertices[i].z);

			normals.push_back(mesh->mNormals[i].x);
			normals.push_back(mesh->mNormals[i].y);
			normals.push_back(mesh->mNormals[i].z);

			uvs.push_back(mesh->mTextureCoords[0][i].x);
			uvs.push_back(mesh->mTextureCoords[0][i].y);
		}

		for (uint32_t f = 0; f < mesh->mNumFaces; f++)
		{
			aiFace	face = mesh->mFaces[f];

			for (uint32_t id = 0; id < face.mNumIndices; id++)
			{
				indices.push_back(face.mIndices[id]);
			}
		}

		skinIndices.assign(positions.size() * 4 / 3, -1.0);
		skinWeights.assign(positions.size() * 4 / 3, 0.0);

		bool hasBone = false;
		for (uint32_t b = 0; b < mesh->mNumBones; ++b) 
		{
			auto aiBone = mesh->mBones[b];

			std::string name = aiBone->mName.C_Str();
			auto bone = getBone(name, bones);
			meshBones.push_back(bone);

			offsetMatrices.push_back(getGLMMat4(aiBone->mOffsetMatrix));

			uint32_t weightsNum = aiBone->mNumWeights;
			auto weights = aiBone->mWeights;
			for (uint32_t w = 0; w < weightsNum; ++w) 
			{
				auto vertexId = weights[w].mVertexId;
				auto skinWeight = weights[w].mWeight;

				auto x = mesh->mNumBones;
				if (b > 100 || b < 0)
				{
					std::cout << b << std::endl;;
				}

				setVertexSkinData(vertexId, static_cast<float>(b), skinWeight, skinIndices, skinWeights);
			}

			hasBone = true;
		}

		geometry->setAttribute(AttributeType::Position, std::make_shared<AttributeF>(positions, 3));
		geometry->setAttribute(AttributeType::Normal, std::make_shared<AttributeF>(normals, 3));
		geometry->setAttribute(AttributeType::UV, std::make_shared<AttributeF>(uvs, 2));
		geometry->setAttribute(AttributeType::SkinIndex, std::make_shared<AttributeF>(skinIndices, 4));
		geometry->setAttribute(AttributeType::SkinWeight, std::make_shared<AttributeF>(skinWeights, 4));
		geometry->setIndex(std::make_shared<AttributeUInt32>(indices, 1));

		if (mesh->mMaterialIndex >= 0) 
		{
			material = materials[mesh->mMaterialIndex];
		}

		if (hasBone) 
		{
			auto skeleton = std::make_shared<Skeleton>(meshBones, offsetMatrices);
			auto skinnedMesh = SkinnedMesh::create(geometry, material);
			skinnedMesh->bind(skeleton);

			object = skinnedMesh;
		}
		else 
		{
			object = Mesh::create(geometry, material);
			object->setLocalMatrix(localTransform);
		}

		return object;
	}

	glm::mat4 AssimpLoader::getGLMMat4(aiMatrix4x4 value)
	{
		glm::mat4 to;

		to[0][0] = value.a1; to[1][0] = value.a2; to[2][0] = value.a3; to[3][0] = value.a4;
		to[0][1] = value.b1; to[1][1] = value.b2; to[2][1] = value.b3; to[3][1] = value.b4;
		to[0][2] = value.c1; to[1][2] = value.c2; to[2][2] = value.c3; to[3][2] = value.c4;
		to[0][3] = value.d1; to[1][3] = value.d2; to[2][3] = value.d3; to[3][3] = value.d4;

		return to;
	}

	void AssimpLoader::buildSkeleton(const aiNode* node, const aiScene* scene, Bone::Ptr parentBone, std::vector<Bone::Ptr>& bones)
	{
		auto bone = Bone::create();
		bones.push_back(bone);

		bone->setName(node->mName.C_Str());

		glm::mat4 nodeMatrix = getGLMMat4(node->mTransformation);
		bone->setNodeMatrix(nodeMatrix);

		parentBone->addChild(bone);

		for (uint32_t i = 0; i < node->mNumChildren; ++i)
		{
			buildSkeleton(node->mChildren[i], scene, bone, bones);
		}
	}

	Bone::Ptr AssimpLoader::getBone(const std::string& name, const std::vector<Bone::Ptr>& bones)
	{
		for (uint32_t i = 0; i < bones.size(); ++i) {
			if (bones[i]->getName() == name) {
				return bones[i];
			}
		}

		return nullptr;
	}

	void AssimpLoader::setVertexSkinData(const uint32_t& id, const float& boneId, const float& skinWeight, std::vector<float>& skinIndices, std::vector<float>& skinWeights)
	{
		for (uint32_t i = 0; i < 4; ++i) 
		{
			if (skinIndices[id * 4 + i] < 0.0f) 
			{
				skinIndices[id * 4 + i] = boneId;
				skinWeights[id * 4 + i] = skinWeight;
				break;
			}
		}
	}

	std::vector<AnimationClip::Ptr> AssimpLoader::processAnimation(const aiScene* scene)
	{
		std::vector<AnimationClip::Ptr> clips;
		//取得当前模型有多少个动画（跑动，出拳）
		uint32_t animationNum = scene->mNumAnimations;

		//遍历每一个动画进行读取
		for (uint32_t i = 0; i < animationNum; ++i) 
		{
			auto animation = scene->mAnimations[i];
			std::string name = animation->mName.C_Str();

			//以ticks为单位的动画总时长
			float duration = static_cast<float>(animation->mDuration);

			//一秒钟有多少ticks
			float ticksPerSecond = static_cast<float>(animation->mTicksPerSecond);

			std::vector<KeyframeTrack::Ptr> tracks;

			//遍历当前动画的所有Tracks并且构造KeyFrameTracks
			//mNumChannels是指当前的动画涉及到了多少个骨骼
			//一个Channel代表了一个骨骼（aiNode）
			for (uint32_t c = 0; c < animation->mNumChannels; ++c) 
			{
				auto channel = animation->mChannels[c];
				std::string boneName = channel->mNodeName.C_Str();

				std::vector<float> values;
				std::vector<float> times;
				std::string trackName;

				//开始遍历当前的Channel（bone）内部的平移相关的动画关键帧
				for (uint32_t p = 0; p < channel->mNumPositionKeys; ++p) 
				{
					auto position = getGLMVec3(channel->mPositionKeys[p].mValue);
					auto time = channel->mPositionKeys[p].mTime;

					values.push_back(position.x);
					values.push_back(position.y);
					values.push_back(position.z);

					times.push_back(static_cast<float>(time));
				}

				//如果当前确实读取到了第c个channel的position变换关键帧数据
				if (times.size()) {

					auto positionKeyframe = std::make_shared<VectorKeyframeTrack>(boneName, TrackType::Move, times, values);
					tracks.push_back(positionKeyframe);
				}

				values.clear();
				times.clear();

				//读取旋转相关关键帧
				for (uint32_t r = 0; r < channel->mNumRotationKeys; ++r) 
				{
					auto quaternion = getGLMQuat(channel->mRotationKeys[r].mValue);
					auto time = channel->mRotationKeys[r].mTime;

					values.push_back(quaternion.x);
					values.push_back(quaternion.y);
					values.push_back(quaternion.z);
					values.push_back(quaternion.w);

					times.push_back(static_cast<float>(time));
				}

				//如果四元数的关键帧不为0
				if (times.size() != 0) 
				{
					auto quaternionKeyframe = std::make_shared<QuaternionKeyframeTrack>(boneName, TrackType::Rotate, times, values);
					tracks.push_back(quaternionKeyframe);
				}

				values.clear();
				times.clear();

				//读取缩放相关的关键帧
				for (uint32_t s = 0; s < channel->mNumScalingKeys; ++s) 
				{
					auto scale = getGLMVec3(channel->mScalingKeys[s].mValue);
					auto time = channel->mScalingKeys[s].mTime;

					values.push_back(scale.x);
					values.push_back(scale.y);
					values.push_back(scale.z);

					times.push_back(static_cast<float>(time));
				}

				//如果缩放关键帧不为空
				if (times.size() != 0) 
				{
					auto scaleKeyframe = std::make_shared<VectorKeyframeTrack>(boneName, TrackType::Scale, times, values);
					tracks.push_back(scaleKeyframe);
				}
			}

			auto clip = std::make_shared<AnimationClip>(name, ticksPerSecond, duration, tracks);
			clips.push_back(clip);
		}

		return clips;
	}

	glm::vec3 AssimpLoader::getGLMVec3(aiVector3D value)
	{
		return glm::vec3(value.x, value.y, value.z);
	}

	glm::quat AssimpLoader::getGLMQuat(aiQuaternion value)
	{
		return glm::quat(value.w, value.x, value.y, value.z);
	}
}
