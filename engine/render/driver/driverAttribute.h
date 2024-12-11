#pragma once
#include <glad/glad.h>
#include "../../geometries/attribute.h"

namespace JB
{
	template<typename T>
	class DriverAttribute
	{
	public:
		using Ptr = std::shared_ptr<DriverAttribute>;

		DriverAttribute(std::weak_ptr<Attribute<T>> attribute, const BufferType bufferType);

		~DriverAttribute();

		void use();

		void nonUse();
		
		std::weak_ptr<Attribute<T>> getAttribute();

	private:
		void update();

	private:
		GLuint m_handle;	//VBO EBO
		std::weak_ptr<Attribute<T>> m_attribute;
		BufferType m_bufferType;
	};

	using DriverAttributeF = DriverAttribute<float>;
	using DriverAttributeUInt32 = DriverAttribute<uint32_t>;

	using DriverAttributeFMap = std::unordered_map<AttributeType, DriverAttributeF::Ptr>;
	using DriverAttributeUInt32Map = std::unordered_map<AttributeType, DriverAttributeUInt32::Ptr>;

	template<typename T>
	inline DriverAttribute<T>::DriverAttribute(std::weak_ptr<Attribute<T>> attribute, const BufferType bufferType)
	{
		glGenBuffers(1, &m_handle);
		m_attribute = attribute;
		m_bufferType = bufferType;
	}

	template<typename T>
	inline DriverAttribute<T>::~DriverAttribute()
	{
		if (m_handle)
			glDeleteBuffers(1, &m_handle);
	}

	template<typename T>
	inline void DriverAttribute<T>::use()
	{
		glBindBuffer(toGL(m_bufferType), m_handle);

		update();
	}

	template<typename T>
	inline void DriverAttribute<T>::nonUse()
	{
		glBindBuffer(toGL(m_bufferType), 0);
	}

	template<typename T>
	inline void DriverAttribute<T>::update()
	{
		if (m_attribute.expired())
			return;
		
		auto attributePtr = m_attribute.lock();
		if (!attributePtr->getNeedsUpdate())
			return;

		auto data = attributePtr->getData();
		glBufferData(toGL(m_bufferType), data.size() * sizeof(T), data.data(), toGL(attributePtr->getBufferAllocType()));
	}

	template<typename T>
	inline std::weak_ptr<Attribute<T>> DriverAttribute<T>::getAttribute()
	{
		return m_attribute;
	}
}
