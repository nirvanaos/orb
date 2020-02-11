#ifndef NIRVANA_ORB_TYPEBASE_H_
#define NIRVANA_ORB_TYPEBASE_H_

#include "Type_forward.h"

namespace CORBA {
namespace Nirvana {

//! Base for IDL data type traits
template <class T>
struct TypeBase
{
	static const bool has_check = false;

	static void check (const T&)
	{}

	// ABI data types
	typedef const T* ABI_in;
	typedef T* ABI_out;
	typedef T* ABI_inout;
	typedef T ABI_ret;

	// Client side types
	typedef const T& C_in;
	typedef T& C_out;
	typedef T& C_inout;
	typedef T C_ret;

	// Server side methods

	static const T& in (ABI_in p)
	{
		_check_pointer (p);
		return *p;
	}

	static T& out (ABI_out p)
	{
		return inout (p);
	}

	static T& inout (ABI_inout p)
	{
		_check_pointer (p);
		return *p;
	}
};

}
}

#endif
