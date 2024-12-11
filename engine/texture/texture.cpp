#include "texture.h"
#include "../tools/idAllocator.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace JB
{
	Texture::Texture()
	{
		m_id = IDAllocator::getInstance().allocateID();
		m_format = TextureFormat::RGBA;
		m_dataType = DataType::UnsignedByte;
	}

	Texture::~Texture()
	{
		IDAllocator::getInstance().releaseID(m_id);
	}

	size_t Texture::getId()
	{
		return m_id;
	}

	TextureType Texture::getType()
	{
		return m_type;
	}

	int Texture::getWidth()
	{
		return m_width;
	}

	int Texture::getHeight()
	{
		return m_height;
	}

	bool Texture::getNeedsUpdate()
	{
		return m_needsUpdate;
	}

	void Texture::setNeedsUpdate(bool needsUpdate)
	{
		m_needsUpdate = needsUpdate;
	}

	void Texture::setSize(int width, int height)
	{
		if (m_width == width && m_height == height)
			return;

		m_width = width;
		m_height = height;
		m_needsUpdate = true;
	}

	void Texture::setTextureWrappingS(TextureWrapping wrap)
	{
		m_wrapS = wrap;
		m_needsUpdate = true;
	}

	void Texture::setTextureWrappingT(TextureWrapping wrap)
	{
		m_wrapT = wrap;
		m_needsUpdate = true;
	}

	void Texture::setTextureFilterMin(TextureFilter filter)
	{
		m_minFilter = filter;
		m_needsUpdate = true;
	}

	void Texture::setTextureFilterMag(TextureFilter filter)
	{
		m_magFilter = filter;
		m_needsUpdate = true;
	}

	void Texture::setTextureFormat(TextureFormat format)
	{
		m_format = format;
		m_needsUpdate = true;
	}

	void Texture::setDataType(DataType dataType)
	{
		m_dataType = dataType;
	}

	void Texture::setEdgeColor(float r, float g, float b, float a)
	{
		setEdgeColor(glm::vec4(r, g, b, a));
	}

	void Texture::setEdgeColor(const glm::vec4& color)
	{
		m_edgeColor = color;
	}

	TextureWrapping Texture::getTextureWrappingS() const
	{
		return m_wrapS;
	}

	TextureWrapping Texture::getTextureWrappingT() const
	{
		return m_wrapT;
	}

	TextureFilter Texture::getTextureFilterMin() const
	{
		return m_minFilter;
	}

	TextureFilter Texture::getTextureFilterMag() const
	{
		return m_magFilter;
	}

	TextureFormat Texture::getTextureFormat()
	{
		return m_format;
	}

	DataType Texture::getDataType()
	{
		return m_dataType;
	}

	glm::vec4 Texture::getEdgeColor() const
	{
		return m_edgeColor;
	}

	void Texture::loadImage(const std::string filePath, std::vector<unsigned char>& data, int& width, int& height)
	{
		int nrChannels = 0, dataSize = 0;
		unsigned char* bits = nullptr;

		//stbi_set_flip_vertically_on_load(true);
		bits = stbi_load(filePath.c_str(), &width, &height, &nrChannels, toStbImageFormat(TextureFormat::RGBA));

		if (bits)
		{
			dataSize = width * height * toByteSize(TextureFormat::RGBA);

			data.resize(dataSize);
			memcpy(data.data(), bits, dataSize);
			stbi_image_free(bits);
		}
	}
}
