#pragma once
#include "driverTexture.h"
#include "../../material/material.h"
#include "../../material/baseMaterial.h"
#include "../../material/cubeMaterial.h"
#include "../../material/phongLightingMaterial.h"

namespace JB
{
	class DriverMaterial
	{
	public:
		using Ptr = std::shared_ptr<DriverMaterial>;

		static DriverMaterial::Ptr getOrAdd(std::weak_ptr<Material> material);

		DriverMaterial(std::weak_ptr<Material> material);

		~DriverMaterial();

		void use();

		void nonUse();

	private:
		void update();

	private:
		std::weak_ptr<Material> m_material;
		DriverTexture::Ptr m_texture;
		DriverTexture::Ptr m_diffuseTexture;
		DriverTexture::Ptr m_specularTexture;
	};
}
