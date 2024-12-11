#include "idAllocator.h"

namespace JB
{
	IDAllocator& IDAllocator::getInstance()
	{
		static IDAllocator instance;
		return instance;
	}

	size_t IDAllocator::allocateID()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (!m_freeIDs.empty()) {
			size_t id = m_freeIDs.top();
			m_freeIDs.pop();
			return id;
		}
		return m_nextID++;
	}
	
	void IDAllocator::releaseID(size_t id)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_allocatedIDs.erase(id);
		m_freeIDs.push(id);
	}
	
	bool IDAllocator::isAllocated(size_t id) const
	{
		return false;
	}

	IDAllocator::IDAllocator()
		: m_nextID(1)
	{

	}

	IDAllocator::~IDAllocator()
	{
	}
}
