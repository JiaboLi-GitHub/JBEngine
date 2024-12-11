#pragma once
#include "../objects/object3D.h"
#include "lightShadow.h"

namespace JB
{
	class Light : public Object3D
	{
	public:
		using Ptr = std::shared_ptr<Light>;

		virtual ~Light();

		void setAmbient(const glm::vec3& color);

		void setDiffuse(const glm::vec3& color);

		void setSpecular(const glm::vec3& color);

		void setCastShadow(bool castShadow);

		LightsType getLightsType() const;

		glm::vec3 getAmbient() const;

		glm::vec3 getDiffuse() const;

		glm::vec3 getSpecular() const;

		bool getCastShadow();

		LightShadow::Ptr getLightShadow();

	protected:
		Light();

	protected:
		LightsType m_lightsType;
		glm::vec3 m_ambient = glm::vec3(0.0f);		//环境光照
		glm::vec3 m_diffuse = glm::vec3(0.0f);		//漫反射光照
		glm::vec3 m_specular = glm::vec3(0.0f);		//镜面光照
		bool m_castShadow = true;					//是否产生阴影
		LightShadow::Ptr m_shadow = nullptr;
	};
}
