#pragma once
#include "material.h"
#include "../texture/texture2D.h"

namespace JB
{
	class PhongLightingMaterial : public Material
	{
	public:
		using Ptr = std::shared_ptr<PhongLightingMaterial>;

		PhongLightingMaterial();

		~PhongLightingMaterial();

		void setDiffuse(const Texture2D::Ptr& texture);

		void setSpecular(const Texture2D::Ptr& texture);

		void setShininess(float shininess);

		Texture2D::Ptr getDiffuse() const;

		Texture2D::Ptr getSpecular() const;

		float getShininess() const;

	private:
		Texture2D::Ptr m_diffuse = nullptr;			//漫反射贴图
		Texture2D::Ptr m_specular = nullptr;		//镜面光贴图
		float m_shininess = 36;						//反光度
	};
}
