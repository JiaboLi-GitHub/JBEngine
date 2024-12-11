#include "driverTexture.h"
#include "../../cache/cache.h"

namespace JB
{
	int DriverTexture::m_maxTextureUnits = -1;
	std::queue<int> DriverTexture::m_availableTextureUnits;
	std::unordered_map<size_t, int> DriverTexture::m_usedTextureUnits;

	DriverTexture::Ptr DriverTexture::getOrAdd(std::weak_ptr<Texture> texture)
	{
		auto texturePtr = texture.lock();
		if (!texturePtr)
			return nullptr;

		auto hash = texturePtr->getId();
		auto driverTexture = DriverTextureCache::getInstance().get(hash);
		if (!driverTexture)
		{
			driverTexture = std::make_shared<DriverTexture>(texture);
			DriverTextureCache::getInstance().add(hash, driverTexture);
		}
		return driverTexture;
	}

	DriverTexture::DriverTexture(const std::weak_ptr<Texture>& texture)
	{
		m_texture = texture;
		glGenTextures(1, &m_handle);
		update();
	}

	DriverTexture::~DriverTexture()
	{
		if (m_handle)
			glDeleteTextures(1, &m_handle);
	}

	void DriverTexture::use()
	{
		auto texturePtr = m_texture.lock();
		if (!texturePtr)
			return;
		auto textureTypeGL = toGL(texturePtr->getType());

		update();

		DriverTexture::bindTexture(texturePtr->getType(), texturePtr->getId(), m_handle);
	}

	void DriverTexture::nonUse()
	{
		auto texturePtr = m_texture.lock();
		if (!texturePtr)
			return;
		auto textureTypeGL = toGL(texturePtr->getType());

		glBindTexture(textureTypeGL, 0);
	}

	GLuint DriverTexture::getHandle()
	{
		return m_handle;
	}

	void DriverTexture::init()
	{
		m_availableTextureUnits = std::queue<int>();
		m_usedTextureUnits.clear();

		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_maxTextureUnits);

		for (int i = 0; i < m_maxTextureUnits - 1; ++i)
			m_availableTextureUnits.push(i);
	}

	int DriverTexture::bindTexture(TextureType type, size_t id, int handle)
	{
		int textureUnits = -1;
		if (m_usedTextureUnits.count(id))
		{
			textureUnits = m_usedTextureUnits[id];
		}
		else
		{
			if (m_availableTextureUnits.size())
			{
				textureUnits = m_availableTextureUnits.front();
				m_availableTextureUnits.pop();
			}
		}

		auto textureTypeGL = toGL(type);

		glActiveTexture(GL_TEXTURE0 + textureUnits);
		glBindTexture(textureTypeGL, handle);
		m_usedTextureUnits.emplace(id, textureUnits);

		return textureUnits;
	}

	int DriverTexture::getTextureUnits(size_t id)
	{
		int textureUnits = -1;
		if (m_usedTextureUnits.count(id))
		{
			textureUnits = m_usedTextureUnits[id];
		}
		return textureUnits;
	}

	void DriverTexture::update()
	{
		auto texturePtr = m_texture.lock();
		if (!texturePtr)
			return;

		if (!texturePtr->getNeedsUpdate())
			return;

		auto textureTypeGL = toGL(texturePtr->getType());

		glActiveTexture(GL_TEXTURE0 + m_maxTextureUnits - 1);
		glBindTexture(textureTypeGL, m_handle);

		glTexParameteri(textureTypeGL, GL_TEXTURE_MIN_FILTER, toGL(texturePtr->getTextureFilterMin()));
		glTexParameteri(textureTypeGL, GL_TEXTURE_MAG_FILTER, toGL(texturePtr->getTextureFilterMag()));
		glTexParameteri(textureTypeGL, GL_TEXTURE_WRAP_S, toGL(texturePtr->getTextureWrappingS()));
		glTexParameteri(textureTypeGL, GL_TEXTURE_WRAP_T, toGL(texturePtr->getTextureWrappingT()));

		auto edgeColor = texturePtr->getEdgeColor();
		GLfloat borderColor[] = { edgeColor.r, edgeColor.g, edgeColor.b, edgeColor.a };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		switch (texturePtr->getType())
		{
		case TextureType::Texture2D:
		{
			auto texture2D = std::static_pointer_cast<Texture2D>(texturePtr);
			auto glTextureFormat = toGL(texturePtr->getTextureFormat());
			auto glDataType = toGL(texture2D->getDataType());
			glTexImage2D(GL_TEXTURE_2D, 0, glTextureFormat, texturePtr->getWidth(), texturePtr->getHeight(), 0, glTextureFormat, glDataType, texture2D->getData());
			glGenerateMipmap(GL_TEXTURE_2D);
			break;
		}
		case TextureType::TextureCube:
		{
			auto textureCube = std::static_pointer_cast<TextureCube>(texturePtr);
			auto glTextureFormat = toGL(textureCube->getTextureFormat());
			auto glDataType = toGL(textureCube->getDataType());
			glTexParameteri(textureTypeGL, GL_TEXTURE_WRAP_R, toGL(textureCube->getTextureWrappingR()));
			for (const auto& [target, data] : textureCube->getDataMap())
			{
				glTexImage2D(toGL(target), 0, glTextureFormat, texturePtr->getWidth(), texturePtr->getHeight(), 0, glTextureFormat, glDataType, data.data());
			}
			break;
		}
		default:
			break;
		}

		glBindTexture(textureTypeGL, 0);

		texturePtr->setNeedsUpdate(false);
	}
}
