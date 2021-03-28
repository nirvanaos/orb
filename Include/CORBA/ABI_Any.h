//! \file ABI_Any.h.
//!
//! \brief Declares the any ABI.
//! This file have not be changed in future

#ifndef NIRVANA_ORB_ABI_ANY_H_
#define NIRVANA_ORB_ABI_ANY_H_

#include <Nirvana/NirvanaBase.h>
#include "Type_forward.h"
#include <assert.h>

namespace CORBA {

class Any;
class TypeCode;

namespace Nirvana {

template <>
struct alignas (sizeof (uintptr_t)) ABI <Any>
{
	/// Pointer to TypeCode and large data flag in the least significant bit.
	uintptr_t type_code;

	struct alignas (sizeof (void*))
	{
		void* p;	//< Pointer to large data
		size_t size;	//< Large data size
		size_t padding;
	} data;

	static const size_t SMALL_CAPACITY = sizeof (data);

	void reset ()
	{
		type_code = 0;
	}

	TypeCode* type () const
	{
		return reinterpret_cast <TypeCode*> (type_code & ~1);
	}

	void type (void* tc)
	{
		assert (((uintptr_t)tc & 1) == 0);
		type_code = reinterpret_cast <uintptr_t> (tc) | (type_code & 1);
	}

	bool is_large () const
	{
		return (type_code & 1) != 0;
	}

	const void* small_pointer () const
	{
		return &data;
	}

	void* small_pointer ()
	{
		return &data;
	}

	void* large_pointer () const
	{
		return data.p;
	}

	void large_pointer (void* p, size_t size)
	{
		data.p = p;
		data.size = size;
		type_code |= 1;
	}

	size_t large_size () const
	{
		return data.size;
	}
};

}
}

#endif
