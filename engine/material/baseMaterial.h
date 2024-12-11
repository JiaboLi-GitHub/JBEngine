#pragma once
#include "material.h"
#include "../texture/texture2D.h"

namespace JB
{
	class BaseMaterial : public Material
	{
	public:
		using Ptr = std::shared_ptr<BaseMaterial>;

		BaseMaterial();

		~BaseMaterial();

		void setTexture(const Texture2D::Ptr& texture);

		Texture::Ptr getTexture();

	private:
		Texture2D::Ptr m_texture;
	};
}
