#ifndef NIRVANA_ORB_TYPEBYREF_H_
#define NIRVANA_ORB_TYPEBYREF_H_

#include "Type_forward.h"

namespace CORBA {
namespace Nirvana {

/// Data type, passed by reference.
template <class T>
struct TypeByRef
{
	typedef ABI <T> ABI_type;

	static const bool has_check = false;
	static void check (ABI_type&)
	{}

	// ABI data types
	typedef const ABI_type* ABI_in;
	typedef ABI_type* ABI_out;
	typedef ABI_type* ABI_inout;
	typedef ABI_type ABI_ret;
	typedef const ABI_type* ABI_VT_ret;

	// Client-side types

	class C_in
	{
	public:
		C_in (const T& v) :
			ref_ (v)
		{}

		ABI_in operator & () const
		{
			return &reinterpret_cast <const ABI_type&> (ref_);
		}

	protected:
		const T& ref_;
	};

	class C_inout
	{
	public:
		C_inout (T& v) :
			ref_ (v)
		{}

		ABI_type* operator & () const
		{
			return &reinterpret_cast <ABI_type&> (ref_);
		}

	protected:
		T& ref_;
	};

	typedef C_inout C_out;

	// Client C_var class for the C++ IDL mapping standard conformance
	class C_var :
		public T
	{
	public:
		C_var ()
		{}

		C_var (const T& v) :
			T (v)
		{}

		C_var (const C_var& src) :
			T (src)
		{}

		C_var& operator = (const T& v)
		{
			if (this != &v)
				T::operator = (v);
			return *this;
		}

		C_var& operator = (const C_var& v)
		{
			if (this != &v)
				T::operator = (v);
			return *this;
		}

		T& operator -> ()
		{
			return *this;
		}

		const T& operator -> () const
		{
			return *this;
		}

		C_in in () const
		{
			return *this;
		}

		C_out out ()
		{
			return *this;
		}

		C_inout inout ()
		{
			return *this;
		}

		T _retn ()
		{
			return *this;
		}
	};

	struct C_ret : ABI_ret
	{
		operator const T& () const
		{
			return reinterpret_cast <const T&> (*this);
		}
	};

	class C_VT_ret
	{
	public:
		C_VT_ret (ABI_VT_ret p) :
			p_ (reinterpret_cast <const T*> (p))
		{
			_check_pointer (p);
		}

		operator const T& () const
		{
			return *p_;
		}

	protected:
		const T* p_;
	};

	// Servant-side methods

	static const T& in (ABI_in p)
	{
		_check_pointer (p);
		return reinterpret_cast <const T&> (*p);
	}

	static T& out (ABI_out p)
	{
		return inout (p);
	}

	static T& inout (ABI_inout p)
	{
		_check_pointer (p);
		return reinterpret_cast <T&> (*p);
	}

	static ABI_ret ret (T& v)
	{
		return reinterpret_cast <ABI_ret&> (v);
	}

	static ABI_VT_ret VT_ret (const T& v)
	{
		return &reinterpret_cast <const ABI_type&> (v);
	}
};

}
}

#endif
