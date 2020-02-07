#ifndef NIRVANA_ORB_ANYABI_H_
#define NIRVANA_ORB_ANYABI_H_

#include "TypeCode.h"

namespace CORBA {
namespace Nirvana {

class AnyABI
{
public:
	static AnyABI _nil ()
	{
		AnyABI abi;
		abi.reset ();
		return abi;
	}

	AnyABI (AnyABI&& src) :
		data_ (src.data_)
	{
		src.reset ();
	}

protected:
	struct Data
	{
		uintptr_t type_code;

		struct
		{
			void* p;
			size_t padding;
			size_t size;
		} data;
	};

	static const size_t SMALL_CAPACITY = sizeof (Data::data);

	AnyABI ()
	{}

	void reset ()
	{
		data_.type_code = 0;
	}

	TypeCode_ptr type () const
	{
		return reinterpret_cast <TypeCode*> (data_.type_code & ~1);
	}

	void type (TypeCode_ptr tc)
	{
		data_.type_code = reinterpret_cast <uintptr_t> (static_cast <Bridge <TypeCode>*> (tc)) | (data_.type_code & 1);
	}

	bool is_large () const
	{
		return (data_.type_code & 1) != 0;
	}

	const void* small_pointer () const
	{
		return &data_.data;
	}

	void* small_pointer ()
	{
		return &data_.data;
	}

	void* large_pointer () const
	{
		return data_.data.p;
	}

	void large_pointer (void* p, size_t size)
	{
		data_.data.p = p;
		data_.data.size = size;
		data_.type_code |= 1;
	}

	size_t large_size ()
	{
		return data_.data.size;
	}

protected:
 Data data_;
};

}
}

#endif
