#pragma once
#include <glm/glm.hpp>
#include "../../material/material.h"
#include "driverGLSL.h"

namespace JB
{
	class DriverShader
	{
	public:
		using Ptr = std::shared_ptr<DriverShader>;

		static DriverShader::Ptr getOrAdd(std::weak_ptr<Material> material);

		DriverShader(std::weak_ptr<Material> material);

		~DriverShader();

		void use();

		void nonUse();

		void setBool(const std::string& name, bool value);

		void setInt(const std::string& name, int value);

		void setFloat(const std::string& name, float value);

		void setVec3(const std::string& name, const glm::vec3& value);

		void setMat4(const std::string& name, const glm::mat4& value);

		void setGLSLMaterial(const std::string& name, const GLSL::Material& value);

		void setGLSLDirectionalLights(const std::string& name, const GLSL::DirLight& value);

		void setGLSLDirectionalLightsArray(const std::string& name, const std::vector<GLSL::DirLight>& value);

		void setMat4Array(const std::string& name, const std::vector<glm::mat4>& value);

		void compile();

		void replace(const std::string& placeholder, const std::string& value);

	private:
		void build();

		static void replace(std::string& original, const std::string& placeholder, const std::string& value);

	private:
		std::weak_ptr<Material> m_material;
		GLuint m_handle;
		std::string m_vsCode;
		std::string m_fsCode;
	};
}
