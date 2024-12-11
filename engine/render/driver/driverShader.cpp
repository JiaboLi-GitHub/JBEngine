#include "driverShader.h"
#include "driverGLSL.h"
#include "../../cache/cache.h"
#include "../../tools/errorAnalyzer.h"

namespace JB
{
	DriverShader::Ptr DriverShader::getOrAdd(std::weak_ptr<Material> material)
	{
		auto materialHash = DriverShaderCache::hash(material);
		auto driverShader = DriverShaderCache::getInstance().get(materialHash);
		if (!driverShader)
		{
			driverShader = std::make_shared<DriverShader>(material);
			DriverShaderCache::getInstance().add(materialHash, driverShader);
		}
		return driverShader;
	}

	DriverShader::DriverShader(std::weak_ptr<Material> material)
	{
		m_material = material;
		m_handle = 0;
		build();
	}

	DriverShader::~DriverShader()
	{
	}

	void DriverShader::use()
	{
		compile();

		glUseProgram(m_handle);
	}

	void DriverShader::nonUse()
	{
		glUseProgram(0);
	}

	void DriverShader::setBool(const std::string& name, bool value)
	{
		glUniform1i(glGetUniformLocation(m_handle, name.c_str()), (int)value);
	}

	void DriverShader::setInt(const std::string& name, int value)
	{
		glUniform1i(glGetUniformLocation(m_handle, name.c_str()), value);
	}

	void DriverShader::setFloat(const std::string& name, float value)
	{
		glUniform1f(glGetUniformLocation(m_handle, name.c_str()), value);
	}

	void DriverShader::setVec3(const std::string& name, const glm::vec3& value)
	{
		glUniform3fv(glGetUniformLocation(m_handle, name.c_str()), 1, &value[0]);
	}

	void DriverShader::setMat4(const std::string& name, const glm::mat4& value)
	{
		glUniformMatrix4fv(glGetUniformLocation(m_handle, name.c_str()), 1, GL_FALSE, &value[0][0]);
	}

	void DriverShader::setGLSLMaterial(const std::string& name, const GLSL::Material& value)
	{
		setInt(name + ".diffuse", value.diffuse);
		setInt(name + ".specular", value.specular);
		setFloat(name + ".shininess", value.shininess);
	}

	void DriverShader::setGLSLDirectionalLights(const std::string& name, const GLSL::DirLight& value)
	{
		setVec3(name + ".direction", value.direction);

		setVec3(name + ".ambient", value.ambient);
		setVec3(name + ".diffuse", value.diffuse);
		setVec3(name + ".specular", value.specular);

		setInt(name + ".castShadow", value.castShadow);
		setInt(name + ".shadowMap", value.shadowMap);

		setMat4(name + ".lightSpaceMatrix", value.lightSpaceMatrix);
	}

	void DriverShader::setGLSLDirectionalLightsArray(const std::string& name, const std::vector<GLSL::DirLight>& value)
	{
		for (size_t i = 0; i < value.size(); ++i)
		{
			setGLSLDirectionalLights(name + "[" + std::to_string(i) + "]", value[i]);
		}
	}

	void DriverShader::compile()
	{
		if (m_handle)
			return;

		std::cout << m_vsCode << std::endl;
		std::cout << m_fsCode << std::endl;

		const char* vShaderCode = m_vsCode.c_str();
		const char* fShaderCode = m_fsCode.c_str();

		unsigned int vertex, fragment;

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);

		ErrorAnalyzer::checkCompileErrors(vertex, "VERTEX");
		ErrorAnalyzer::checkErrors();

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

		ErrorAnalyzer::checkCompileErrors(fragment, "FRAGMENT");
		ErrorAnalyzer::checkErrors();

		m_handle = glCreateProgram();
		glAttachShader(m_handle, vertex);
		glAttachShader(m_handle, fragment);
		glLinkProgram(m_handle);
		glDeleteShader(vertex);
		glDeleteShader(fragment);

		ErrorAnalyzer::checkCompileErrors(m_handle, "PROGRAM");
		ErrorAnalyzer::checkErrors();
	}

	void DriverShader::replace(const std::string& placeholder, const std::string& value)
	{
		DriverShader::replace(m_vsCode, placeholder, value);
		DriverShader::replace(m_fsCode, placeholder, value);
	}

	void DriverShader::build()
	{
		Material::Ptr materialPtr = m_material.lock();
		if (!materialPtr)
			return;

		switch (materialPtr->getMaterialType())
		{
		case MaterialType::BaseMaterial:
		{
			auto baseMaterial = std::dynamic_pointer_cast<BaseMaterial>(materialPtr);
			m_vsCode = m_fsCode = "#version 330 core\n";
			if (baseMaterial->getTexture())
			{
				m_vsCode += "#define USE_TEXTURE\n";
				m_fsCode += "#define USE_TEXTURE\n";
			}

			m_vsCode += MeshBasicMaterialVSCode;
			m_fsCode += MeshBasicMaterialFSCode;
			break;
		}
		case MaterialType::CubeMaterial:
		{
			m_vsCode += CubeMaterialVSCode;
			m_fsCode += CubeMaterialFSCode;
			break;
		}
		case MaterialType::PhongLightingMaterial:
		{
			m_vsCode += PhongLightingMaterialVSCode;
			m_fsCode += PhongLightingMaterialFSCode;
			break;
		}
		case MaterialType::DepthMaterial:
		{
			m_vsCode += DepthMaterialVSCode;
			m_fsCode += DepthMaterialFSCode;
			break;
		}
		default:
			break;
		}
	}

	void DriverShader::replace(std::string& original, const std::string& placeholder, const std::string& value)
	{
		size_t pos = 0;

		while ((pos = original.find(placeholder, pos)) != std::string::npos) 
		{
			original.replace(pos, placeholder.length(), value);
			pos += value.length(); 
		}
	}
}
