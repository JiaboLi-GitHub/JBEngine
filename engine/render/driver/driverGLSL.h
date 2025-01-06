#pragma once
#include <string>
#include <glm/glm.hpp>

namespace JB
{
	namespace GLSL
	{
		struct Material
		{
			int diffuse;
			int specular;
			float shininess;
		};

		struct DirLight {
			glm::vec3 direction;

			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;

			bool castShadow;
			int shadowMap;

			glm::mat4 lightSpaceMatrix;
		};
	}

	extern std::string MeshBasicMaterialVSCode;
	extern std::string MeshBasicMaterialFSCode;

	extern std::string CubeMaterialVSCode;
	extern std::string CubeMaterialFSCode;

	extern std::string PhongLightingMaterialVSCode;
	extern std::string PhongLightingMaterialFSCode;

	extern std::string DepthMaterialVSCode;
	extern std::string DepthMaterialFSCode;
}
