#include "texture2D.h"


namespace JB
{
	Texture2D::Texture2D()
	{
		m_type = TextureType::Texture2D;
	}

	Texture2D::Texture2D(const std::string& filePath)
		: Texture2D()
	{
		Texture2D::loadImage(filePath, m_data, m_width, m_height);
	}

	Texture2D::Texture2D(unsigned char* dataIn, int widthIn, int heightIn)
		: Texture2D()
	{
		Texture2D::loadImage(dataIn, m_data, widthIn, heightIn, m_width, m_height);
	}

	Texture2D::~Texture2D()
	{
	}

	unsigned char* Texture2D::getData()
	{
		if(m_data.size())
			return m_data.data();
		return nullptr;
	}
}
