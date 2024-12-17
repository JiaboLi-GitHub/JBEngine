#include "assimpLoader.h"

#include <iostream>

#include "../material/phongLightingMaterial.h"
#include "../texture/texture2D.h"
#include "../objects/group.h"
#include "../objects/mesh.h"
#include "../geometries/geometry.h"
#include "../geometries/attribute.h"

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

		std::size_t lastIndex = path.find_last_of("//");
		std::string rootPath = path.substr(0, lastIndex + 1);

		buildMaterial(scene, rootPath, materials);
		buildNode(scene->mRootNode, scene, rootObject, materials);

		result->object3D = rootObject;
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
			//TODO Ƕģ
		}
		else
		{
			std::string fullPath = rootPath + aiPath.C_Str();
			texture = std::make_shared<Texture2D>(fullPath);
		}

		return texture;
	}

	void AssimpLoader::buildNode(const aiNode* node, const aiScene* scene, Object3D::Ptr parentObject, const std::vector<Material::Ptr>& materials)
	{
		Group::Ptr group = Group::create();

		for (uint32_t i = 0; i < node->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

			group->addChild(buildMesh(mesh, scene, getGLMMat4(node->mTransformation), materials));
		}

		parentObject->addChild(group);

		for (uint32_t i = 0; i < node->mNumChildren; i++)
		{
			buildNode(node->mChildren[i], scene, group, materials);
		}
	}

	Object3D::Ptr AssimpLoader::buildMesh(const aiMesh* mesh, const aiScene* scene, const glm::mat4 localTransform, const std::vector<Material::Ptr>& materials)
	{

		Object3D::Ptr object = nullptr;
		Material::Ptr material = nullptr;
		Geometry::Ptr geometry = std::make_shared<Geometry>();

		std::vector<float> positions;
		std::vector<float> normals;

		std::vector<float> uvs;

		std::vector<uint32_t> indices;

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

		geometry->setAttribute(AttributeType::Position, std::make_shared<AttributeF>(positions, 3));
		geometry->setAttribute(AttributeType::Normal, std::make_shared<AttributeF>(normals, 3));
		geometry->setAttribute(AttributeType::UV, std::make_shared<AttributeF>(uvs, 2));
		geometry->setIndex(std::make_shared<AttributeUInt32>(indices, 1));

		if (mesh->mMaterialIndex >= 0)
		{
			material = materials[mesh->mMaterialIndex];
		}

		object = Mesh::create(geometry, material);
		object->setLocalMatrix(localTransform);

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
}
