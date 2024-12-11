#include "constant.h"
#include <stb_image.h>

namespace JB
{
	const std::unordered_map<uint32_t, KeyAction> KeyActionMap = {
		{GLFW_KEY_W, KeyAction::Key_W},
		{GLFW_KEY_S, KeyAction::Key_S},
		{GLFW_KEY_A, KeyAction::Key_A},
		{GLFW_KEY_D, KeyAction::Key_D}
	};

	uint32_t toGL(const BufferAllocType& value) {
		switch (value)
		{
		case BufferAllocType::StaticDrawBuffer:
			return GL_STATIC_DRAW;
		case BufferAllocType::DynamicDrawBuffer:
			return GL_DYNAMIC_DRAW;
		case BufferAllocType::StreamDrawBuffer:
			return GL_STREAM_DRAW;
		default:
			return 0;
		}
	}

	uint32_t toGL(const BufferType& value)
	{
		switch (value) {
		case BufferType::ArrayBuffer:
			return GL_ARRAY_BUFFER;
		case BufferType::IndexBuffer:
			return GL_ELEMENT_ARRAY_BUFFER;
		default:
			return 0;
		}
	}

	uint32_t toGL(DataType value)
	{
		switch (value) {
		case DataType::UnsignedByte:
			return GL_UNSIGNED_BYTE;
		case DataType::Float:
			return GL_FLOAT;
		case DataType::Byte:
			return GL_BYTE;
		case DataType::Int32:
			return GL_INT;
		case DataType::UInt32:
			return GL_UNSIGNED_INT;
		default:
			return 0;
		}
	}

	uint32_t toSize(DataType type)
	{
		switch (type) {
		case DataType::UnsignedByte:
			return sizeof(unsigned char);
		case DataType::Float:
			return sizeof(float);
		case DataType::Byte:
			return sizeof(char);
		case DataType::Int32:
			return sizeof(int);
		case DataType::UInt32:
			return sizeof(uint32_t);
		default:
			return 0;
		}
	}

	uint8_t toLocation(AttributeType type)
	{
		return static_cast<uint8_t>(type);
	}

	GLuint toGL(const TextureType& value)
	{
		switch (value) {
		case TextureType::Texture2D:
			return GL_TEXTURE_2D;
		case TextureType::TextureCube:
			return GL_TEXTURE_CUBE_MAP;
		default:
			return GL_NONE;
		}
	}

	GLuint toGL(const TextureFormat& format)
	{
		switch (format) {
		case TextureFormat::RGB:
			return GL_RGB;
		case TextureFormat::RGBA:
			return GL_RGBA;
		case TextureFormat::DepthFormat:
			return GL_DEPTH_COMPONENT;
		default:
			return 0;
		}
	}

	uint32_t toStbImageFormat(const TextureFormat& format)
	{
		switch (format) {
		case TextureFormat::RGB:
			return STBI_rgb;
		case TextureFormat::RGBA:
			return STBI_rgb_alpha;
		default:
			return 0;
		}
	}

	uint32_t toByteSize(const TextureFormat& format)
	{
		switch (format) 
		{
		case TextureFormat::RGB:
			return 3;
		case TextureFormat::RGBA:
			return 4;
		default:
			return 0;
		}
	}

	uint32_t toGL(TextureCubeTarget target)
	{
		return GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<uint8_t>(target);
	}

	uint32_t toGL(const TextureWrapping& value)
	{
		switch (value) 
		{
		case TextureWrapping::RepeatWrapping:
			return GL_REPEAT;
		case TextureWrapping::ClampToEdgeWrapping:
			return GL_CLAMP_TO_EDGE;
		case TextureWrapping::MirroredRepeatWrapping:
			return GL_MIRRORED_REPEAT;
		case TextureWrapping::ClampToBorder:
			return GL_CLAMP_TO_BORDER;
		default:
			return 0;
		}
	}
	
	uint32_t toGL(const TextureFilter& value)
	{
		switch (value) 
		{
		case TextureFilter::LinearFilter:
			return GL_LINEAR;
		case TextureFilter::NearestFilter:
			return GL_NEAREST;
		case TextureFilter::NearestMipmapNearest:
			return GL_NEAREST_MIPMAP_NEAREST;
		case TextureFilter::LinearMipmapNearest:
			return GL_LINEAR_MIPMAP_NEAREST;
		case TextureFilter::NearestMipmapLinear:
			return GL_NEAREST_MIPMAP_LINEAR;
		case TextureFilter::LinearMipmapLinear:
			return GL_LINEAR_MIPMAP_LINEAR;
		default:
			return 0; // 或者使用一个合适的错误代码
		}
	}
}
