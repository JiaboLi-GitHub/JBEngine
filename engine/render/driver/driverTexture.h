#pragma once
#include <queue>
#include "../../texture/texture2D.h"
#include "../../texture/textureCube.h"

namespace JB
{
	class DriverTexture
	{
	public:
		using Ptr = std::shared_ptr<DriverTexture>;

		static DriverTexture::Ptr getOrAdd(std::weak_ptr<Texture> texture);

		DriverTexture(const std::weak_ptr<Texture>& texture);

		~DriverTexture();

		void use();

		void nonUse();

		GLuint getHandle();

		static void init();

		static int bindTexture(TextureType type, size_t id, int handle);

		static int getTextureUnits(size_t id);

	private:
		void update();

	private:
		GLuint	m_handle;
		std::weak_ptr<Texture> m_texture;

		static int m_maxTextureUnits;
		static std::queue<int> m_availableTextureUnits;
		static std::unordered_map<size_t, int> m_usedTextureUnits;
	};
}
