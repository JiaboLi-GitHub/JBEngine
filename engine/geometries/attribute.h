#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include "../global/constant.h"
#include "../tools/idAllocator.h"

namespace JB
{
	//属性模板类
	template<typename T>
	class Attribute
	{
	public:
		using Ptr = std::shared_ptr<Attribute<T>>;

		Attribute(const std::vector<T>& data, uint32_t itemSize, BufferAllocType bufferAllocType = BufferAllocType::StaticDrawBuffer);
		~Attribute();

		//设置第index个顶点属性值
		void setX(const uint32_t& index, T value);
		void setY(const uint32_t& index, T value);
		void setZ(const uint32_t& index, T value);

		//获取第index个顶点属性值
		T getX(const uint32_t& index);
		T getY(const uint32_t& index);
		T getZ(const uint32_t& index);

		size_t getId();
		bool getNeedsUpdate();
		std::vector<T> getData();
		uint32_t getItemSize();
		uint32_t getCount();
		BufferAllocType getBufferAllocType();
		DataType getDataType();

	private:
		size_t m_id;
		bool m_needsUpdate;
		std::vector<T> m_data;
		uint32_t m_itemSize;
		uint32_t m_count;
		BufferAllocType m_bufferAllocType;
		DataType m_dataType;
	};

	using AttributeF = Attribute<float>;
	using AttributeUInt32 = Attribute<uint32_t>;

	using AttributeFMap = std::unordered_map<AttributeType, AttributeF::Ptr>;
	using AttributeUInt32Map = std::unordered_map<AttributeType, AttributeUInt32::Ptr>;

	template<typename T>
	Attribute<T>::Attribute(const std::vector<T>& data, uint32_t itemSize, BufferAllocType bufferAllocType)
	{
		m_id = IDAllocator::getInstance().allocateID();
		m_needsUpdate = true;
		m_data = data;
		m_itemSize = itemSize;
		m_count = static_cast<uint32_t>(data.size()) / itemSize;
		m_bufferAllocType = bufferAllocType;
		m_dataType = toDataType<T>();
	}

	template<typename T>
	Attribute<T>::~Attribute()
	{
		IDAllocator::getInstance().releaseID(m_id);
	}

	template<typename T>
	inline void Attribute<T>::setX(const uint32_t& index, T value)
	{
		m_data[index * m_itemSize] = value;
	}

	template<typename T>
	inline void Attribute<T>::setY(const uint32_t& index, T value)
	{
		m_data[index * m_itemSize + 1] = value;
	}

	template<typename T>
	inline void Attribute<T>::setZ(const uint32_t& index, T value)
	{
		m_data[index * m_itemSize + 2] = value;
	}

	template<typename T>
	inline T Attribute<T>::getX(const uint32_t& index)
	{
		return m_data[index * m_itemSize];
	}

	template<typename T>
	inline T Attribute<T>::getY(const uint32_t& index)
	{
		return m_data[index * m_itemSize + 1];
	}

	template<typename T>
	inline T Attribute<T>::getZ(const uint32_t& index)
	{
		return m_data[index * m_itemSize + 2];
	}

	template<typename T>
	inline size_t Attribute<T>::getId()
	{
		return m_id;
	}

	template<typename T>
	inline bool Attribute<T>::getNeedsUpdate()
	{
		return m_needsUpdate;
	}

	template<typename T>
	inline std::vector<T> Attribute<T>::getData()
	{
		return m_data;
	}

	template<typename T>
	inline uint32_t Attribute<T>::getItemSize()
	{
		return m_itemSize;
	}

	template<typename T>
	inline uint32_t Attribute<T>::getCount()
	{
		return m_count;
	}

	template<typename T>
	inline BufferAllocType Attribute<T>::getBufferAllocType()
	{
		return m_bufferAllocType;
	}

	template<typename T>
	inline DataType Attribute<T>::getDataType()
	{
		return m_dataType;
	}
}
