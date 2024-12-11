#pragma once
#include <vector>
#include <unordered_map>
#include <bitset>
#include <cstddef>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "GLFW/glfw3.h"

namespace JB
{
	inline constexpr size_t MAX_KEY_NUM = 108;

	using KeyBoardState = std::bitset<MAX_KEY_NUM>;

	enum class KeyAction : uint32_t
	{
		Key_W = 0,
		Key_S,
		Key_A,
		Key_D
	};
	extern const std::unordered_map<uint32_t, KeyAction> KeyActionMap;

	//鼠标动作
	enum class MouseAction : uint8_t {
		LeftDown,
		RightDown,
		MiddleDown,
		LeftUp,
		RightUp,
		MiddleUp,
		None,
	};

	//缓冲类型
	enum class BufferAllocType : uint8_t
	{
		StaticDrawBuffer,
		DynamicDrawBuffer,
		StreamDrawBuffer
	};
	extern uint32_t toGL(const BufferAllocType& value);

	enum class BufferType 
	{
		ArrayBuffer,
		IndexBuffer
	};
	extern uint32_t toGL(const BufferType& value);

	//数据类型
	enum class DataType : uint8_t
	{
		UnsignedByte,
		Byte,
		Int32,
		UInt32,
		Float,
		Double
	};
	template<typename T>
	extern DataType toDataType();
	extern uint32_t toGL(DataType value);
	extern uint32_t toSize(DataType type);

	//属性类型
	enum class AttributeType : uint8_t
	{
		Position,
		Color,
		Normal,
		UV
	};
	extern uint8_t toLocation(AttributeType type);

	//材质类型
	enum class MaterialType : uint8_t
	{
		Material,
		BaseMaterial,
		CubeMaterial,
		PhongLightingMaterial,
		DepthMaterial
	};

	enum class Object3DType : uint8_t
	{
		Object3D,
		RenderableObject,
		Camera,
		OrthographicCamera,
		PerspectiveCamera,
		Mesh,
		Lights
	};

	enum class TextureType : uint8_t
	{
		Texture2D,
		TextureCube
	};
	extern GLuint toGL(const TextureType& value);

	enum class TextureFormat : uint8_t
	{
		RGB,
		RGBA,
		DepthFormat
	};
	extern GLuint toGL(const TextureFormat& format);
	extern uint32_t toStbImageFormat(const TextureFormat& format);
	extern uint32_t toByteSize(const TextureFormat& format);

	template<typename T>
	DataType toDataType()
	{
		if (typeid(T) == typeid(unsigned char))
			return DataType::UnsignedByte;

		if (typeid(T) == typeid(char))
			return DataType::Byte;

		if (typeid(T) == typeid(int))
			return DataType::Int32;

		if (typeid(T) == typeid(uint32_t))
			return DataType::UInt32;

		if (typeid(T) == typeid(float))
			return DataType::Float;

		if (typeid(T) == typeid(double))
			return DataType::Double;

		return DataType::UnsignedByte;
	}

	enum class TextureCubeTarget : uint8_t
	{
		POSITIVE_X = 0,
		NEGATIVE_X,
		POSITIVE_Y,
		NEGATIVE_Y,
		POSITIVE_Z,
		NEGATIVE_Z
	};
	extern uint32_t toGL(TextureCubeTarget target);

	enum class TextureWrapping 
	{
		RepeatWrapping,
		ClampToEdgeWrapping,
		ClampToBorder,
		MirroredRepeatWrapping,
	};
	extern uint32_t toGL(const TextureWrapping& value);

	enum class TextureFilter 
	{
		LinearFilter,                  // GL_LINEAR
		NearestFilter,                 // GL_NEAREST
		NearestMipmapNearest,          // GL_NEAREST_MIPMAP_NEAREST
		LinearMipmapNearest,           // GL_LINEAR_MIPMAP_NEAREST
		NearestMipmapLinear,           // GL_NEAREST_MIPMAP_LINEAR
		LinearMipmapLinear             // GL_LINEAR_MIPMAP_LINEAR
	};
	extern uint32_t toGL(const TextureFilter& value);

	enum class LightsType : uint8_t
	{
		DirectionalLights
	};
}
