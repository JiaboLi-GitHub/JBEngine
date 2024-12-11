#pragma once
#include <iostream>
#include <atomic>
#include <mutex>
#include <unordered_set>
#include <stack>
#include <stdexcept>

namespace JB
{
	class IDAllocator {
	public:
		static IDAllocator& getInstance();

		size_t allocateID();
		void releaseID(size_t id);
		bool isAllocated(size_t id) const;

	private:
		IDAllocator();
		~IDAllocator();

	private:
		std::atomic<size_t> m_nextID;
		mutable std::mutex m_mutex;   // 保护 allocatedIDs 和 freeIDs 的互斥锁
		std::unordered_set<size_t> m_allocatedIDs;
		std::stack<size_t> m_freeIDs;
	};
}
