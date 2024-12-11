#include "phongLightingMaterial.h"

namespace JB
{
	PhongLightingMaterial::PhongLightingMaterial()
	{
		m_type = MaterialType::PhongLightingMaterial;
	}

	PhongLightingMaterial::~PhongLightingMaterial()
	{
	}

	void PhongLightingMaterial::setDiffuse(const Texture2D::Ptr& texture)
	{
		m_diffuse = texture;
	}

	void PhongLightingMaterial::setSpecular(const Texture2D::Ptr& texture)
	{
		m_specular = texture;
	}

	void PhongLightingMaterial::setShininess(float shininess)
	{
		m_shininess = shininess;
	}

	Texture2D::Ptr PhongLightingMaterial::getDiffuse() const
	{
		return m_diffuse;
	}

	Texture2D::Ptr PhongLightingMaterial::getSpecular() const
	{
		return m_specular;
	}

	float PhongLightingMaterial::getShininess() const
	{
		return m_shininess;
	}
}
