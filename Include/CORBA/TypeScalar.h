#ifndef NIRVANA_ORB_TYPESCALAR_H_
#define NIRVANA_ORB_TYPESCALAR_H_

#include "Type_forward.h"

namespace CORBA {
namespace Nirvana {

/// Scalar data type. Passes in parameter by value.
template <class T>
struct TypeScalar
{
	typedef T ABI_type;

	static const bool has_check = false;
	static void check (const ABI_type&)
	{}

	// ABI types
	typedef T ABI_in;
	typedef T* ABI_out;
	typedef T* ABI_inout;
	typedef T ABI_ret;
	typedef T ABI_VT_ret;

	// Client-side types

	/// in parameters passed by value
	class C_in
	{
	public:
		C_in (T val) :
			val_ (val)
		{}

		ABI_in operator & () const
		{
			return val_;
		}

	private:
		T val_;
	};

	typedef T& C_out;
	typedef T& C_inout;
	typedef T C_ret;
	typedef T C_VT_ret;

	// Servant-side methods

	static T in (ABI_in v)
	{
		return v;
	}

	static T& out (ABI_out p)
	{
		_check_pointer (p);
		return *p;
	}

	static T& inout (ABI_out p)
	{
		_check_pointer (p);
		return *p;
	}

	static ABI_ret ret (T v)
	{
		return v;
	}

	static ABI_VT_ret VT_ret (T v)
	{
		return v;
	}
};

}
}

#endif
