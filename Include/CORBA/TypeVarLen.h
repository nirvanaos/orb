#ifndef NIRVANA_ORB_TYPEVARLEN_H_
#define NIRVANA_ORB_TYPEVARLEN_H_

#include "TypeFixLen.h"
#include <utility>
#include <new>

namespace CORBA {
namespace Nirvana {

template <class T>
struct TypeVarLenBase : TypeFixLen <T>
{
	typedef typename TypeFixLen <T>::C_in C_in;
	typedef typename TypeFixLen <T>::C_inout C_inout;

	/// C_out class clears output variable
	class C_out : public TypeFixLen <T>::C_out
	{
	public:
		C_out (T& val) :
			TypeFixLen <T>::C_out (val)
		{
			val = T ();	// Clear
		}
	};

	class C_ret
	{
	public:
		C_ret (typename TypeFixLen <T>::ABI_ret&& val) :
			val_ (val)
		{}

		operator T && ()
		{
			return reinterpret_cast <T&&> (val_);
		}

	protected:
		TypeFixLen <T>::ABI_ret val_;
	};

	// Client I_var class for the C++ IDL mapping standard conformance
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
			return std::move (static_cast <T&> (*this));
		}
	};
};

template <class T, bool with_check> struct TypeVarLen;

template <class T>
struct TypeVarLen <T, false> : TypeVarLenBase <T>
{};

/// Base for variable-length data types
template <class T>
struct TypeVarLen <T, true> : TypeVarLenBase <T>
{
	static const bool has_check = true;

	typedef ABI <T> ABI_type;

	static void check_or_clear (ABI_type& v);

	class C_inout : public TypeVarLenBase <T>::C_inout
	{
	public:
		C_inout (T& val) :
			TypeVarLenBase <T>::C_inout (val)
		{}

		~C_inout () noexcept (false);
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

	class C_ret : public TypeVarLenBase <T>::C_ret
	{
	public:
		C_ret (typename TypeFixLen <T>::ABI_ret&& val) :
			TypeVarLenBase <T>::C_ret (std::move (val))
		{
			if (Type <T>::has_check)
				check_or_clear (this->val_);
		}
	};

	// Servant-side methods

	static const T& in (typename TypeFixLen <T>::ABI_in p)
	{
		_check_pointer (p);
		Type <T>::check (*p);
		return reinterpret_cast <const T&> (*p);
	}

	static T& inout (typename TypeFixLen <T>::ABI_inout p)
	{
		_check_pointer (p);
		Type <T>::check (*p);
		return reinterpret_cast <T&> (*p);
	}

	static T& out (typename TypeFixLen <T>::ABI_out p)
	{
		return inout (p);
	}
};

/// Outline for compact code
template <class T>
void TypeVarLen <T, true>::check_or_clear (ABI_type& v)
{
	try {
		Type <T>::check (v);
	} catch (...) {
		reinterpret_cast <T&> (v).~T (); // Destructor mustn't throw exceptions
		new (&v) T ();
		throw;
	}
}

/// Outline for compact code
template <class T>
TypeVarLen <T, true>::C_inout::~C_inout () noexcept (false)
{
	bool ex = uncaught_exception ();
	try {
		check_or_clear (reinterpret_cast <typename Type <T>::ABI_type&> (this->ref_));
	} catch (...) {
		if (!ex)
			throw;
	}
}

}
}

#endif
