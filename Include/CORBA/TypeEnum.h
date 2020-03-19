#ifndef NIRVANA_ORB_TYPEENUM_H_
#define NIRVANA_ORB_TYPEENUM_H_

#include "BasicTypes.h"
#include <Nirvana/throw_exception.h>

namespace CORBA {
namespace Nirvana {

typedef ULong ABI_enum;

/// Base for enum data types
template <class T, ABI_enum last>
struct TypeEnum
{
	static_assert (sizeof (T) == sizeof (ABI_enum), "IDL enumerations must be declared as 32-bit.");
	static const bool has_check = true;
	static const ABI_enum count_ = last + 1;

	typedef ABI_enum ABI_type;

	typedef ABI_type ABI_in;
	typedef ABI_type* ABI_out;
	typedef ABI_type* ABI_inout;
	typedef ABI_type ABI_ret;

	static void check (ABI_enum val)
	{
		if (val >= count_)
			::Nirvana::throw_BAD_PARAM ();
	}

	class C_in
	{
	public:
		C_in (T val) :
			val_ (val)
		{}

		ABI_in operator & () const
		{
			return (ABI_type)val_;
		}

	private:
		T val_;
	};

	class C_inout
	{
	public:
		C_inout (T& val) :
			ref_ (val),
			val_ ((ABI_type)val)
		{}

		~C_inout () noexcept (false)
		{
			if (!uncaught_exception ()) {
				Type <T>::check (val_);
				ref_ = (T*)val_;
			}
		}

		ABI_type* operator & () const
		{
			return &val_;
		}

	private:
		T& ref_;
		ABI_type val_;
	};

	typedef C_inout C_out;

	class C_ret
	{
	public:
		C_ret (ABI_ret val) :
			val_ ((T)val)
		{
			Type <T>::check (val_);
		}

		operator T ()
		{
			return val_;
		}

	private:
		T val_;
	};

	static T in (ABI_in v)
	{
		Type <T>::check (v);
		return (T)v;
	}

	static T& inout (ABI_inout p)
	{
		_check_pointer (p);
		Type <T>::check (*p);
		return (T&)*p;
	}

	static T& out (ABI_out p)
	{
		_check_pointer (p);
		return (T&)*p;
	}

	static ABI_ret ret (T val)
	{
		return (ABI_ret)val;
	}
};

}
}

#endif
