#ifndef NIRVANA_ORB_TYPEVARLEN_H_
#define NIRVANA_ORB_TYPEVARLEN_H_

#include "TypeBase.h"
#include <utility>
#include <new>

namespace CORBA {
namespace Nirvana {

/// Base for variable-length data types
template <class T>
struct TypeVarLen :
	public TypeBase <T>
{
	static const bool has_check = true;

	using TypeBase <T>::ABI_in;
	using TypeBase <T>::ABI_out;
	using TypeBase <T>::ABI_inout;
	using TypeBase <T>::ABI_ret;

	using TypeBase <T>::C_in;

	class C_inout
	{
	public:
		C_inout (T& val) :
			ref_ (val)
		{}

		~C_inout () noexcept (false);

		T* operator & () const
		{
			return &ref_;
		}

	private:
		T& ref_;
	};

	class C_out : public C_inout
	{
	public:
		C_out (T& val) :
			C_inout (val)
		{
			val = T ();	// Clear
		}
	};

	class C_ret
	{
	public:
		C_ret (T val) :
			val_ (std::move (val))
		{
			check_or_clear (val_);
		}

		operator T ()
		{
			return std::move (val_);
		}

	private:
		T val_;
	};

	// Client T_var class for the C++ IDL mapping standard conformance
	class C_var :
		public T
	{
	public:
		C_var (const T& src) :
			T (src)
		{}

		C_var (T&& src) :
			T (std::move (src))
		{}

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

		typename C_out out ()
		{
			return *this;
		}

		typename C_inout inout ()
		{
			return *this;
		}

		T _retn ()
		{
			return std::move (static_cast <T&> (*this));
		}
	};

	static const T& in (ABI_in p)
	{
		_check_pointer (p);
		Type <T>::check (*p);
		return *p;
	}

	static T& inout (ABI_inout p)
	{
		_check_pointer (p);
		Type <T>::check (*p);
		return *p;
	}

	static T& out (ABI_out p)
	{
		return inout (p);
	}

	static void check_or_clear (T& v);
};


/// Outline for compact code
template <class T>
void TypeVarLen <T>::check_or_clear (T& v)
{
	try {
		Type <T>::check (v);
	} catch (...) {
		v.~T (); // Destructor mustn't throw exceptions
		new (&v) T ();
		throw;
	}
}

/// Outline for compact code
template <class T>
TypeVarLen <T>::C_inout::~C_inout () noexcept (false)
{
	bool ex = uncaught_exception ();
	try {
		Type <T>::check_or_clear (ref_);
	} catch (...) {
		if (!ex)
			throw;
	}
}

}
}

#endif
