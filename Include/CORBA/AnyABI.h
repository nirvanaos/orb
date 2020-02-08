//! \file AnyABI.h.
//!
//! \brief Declares the any ABI
//! This file have not be changed in future

#ifndef NIRVANA_ORB_ANYABI_H_
#define NIRVANA_ORB_ANYABI_H_

#include <Nirvana/NirvanaBase.h>

namespace CORBA {

class TypeCode;

namespace Nirvana {

#pragma pack (push, 1)

struct AnyABI
{
	/// Pointer to TypeCode and large data tag in the least significant bit.
	uintptr_t type_code;

	struct
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

	size_t large_size ()
	{
		return data.size;
	}
};

#pragma pack (pop)

}
}

#endif
