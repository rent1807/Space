#pragma once
#include "stdafx.h"
#include "MemoryManager.h"

namespace SpaceGameEngine
{
	template<typename T>
	class AllocatorForSTL
	{
	public :
		typedef T value_type;
		typedef value_type* pointer;
		typedef value_type& reference;
		typedef value_type const* const_pointer;
		typedef value_type const& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		template<typename _other>
		struct rebind
		{
			typedef AllocatorForSTL<T> other;
		};

		pointer address(reference x)
		{
			return pointer(&x);
		}
		const_pointer address(const_reference x)
		{
			return const_pointer(&x);
		}
		pointer allocate(size_type size)
		{
			return pointer(MemoryManager::Allocate(size));
		}
		void deallocate(pointer p, size_type size)
		{
			MemoryManager::Free(p, size * sizeof(T));
		}
		void construct(pointer p, const_reference ref)
		{
			new (p) T(ref);
		}
		void destory(pointer p)
		{
			p->~T();
		}
		size_type max_size() const
		{
			return size_type(UINT_MAX / sizeof(T));
		}
		bool operator==(const AllocatorForSTL &rhs)
		{
			return true;
		}
		bool operator!=(const AllocatorForSTL &rhs)
		{
			return !operator==(rhs);
		}
		AllocatorForSTL()
		{

		}
		template<typename U>
		AllocatorForSTL(AllocatorForSTL<U>& alloc)
		{

		}
	};
}