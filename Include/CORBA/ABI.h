#ifndef NIRVANA_ORB_ABI_H_
#define NIRVANA_ORB_ABI_H_

#include <type_traits>
#include <new>
#include <utility>
#include "ABI_forward.h"

namespace CORBA {
namespace Nirvana {

void _check_pointer (const void* p);
bool uncaught_exception ();

template <class T>
struct ABI_Base
{
	typedef T Var;

	typedef const T* ABI_in;
	typedef T* ABI_out;
	typedef T* ABI_inout;
	typedef Var ABI_ret;

	typedef const T& In;
	typedef T& InOut;
	typedef T& Out;

	static const T& in (ABI_in p)
	{
		_check_pointer (p);
		return *p;
	}

	static T& inout (ABI_inout p)
	{
		_check_pointer (p);
		return *p;
	}

	static T& out (ABI_out p)
	{
		return inout (p);
	}

	static Var ret (ABI_ret ret)
	{
		return ret;
	}

	static void check (const T&)
	{}
};

/// Base for fixed length data types ABI
template <class T>
struct ABI_Fixed : public ABI_Base <T>
{
	static const bool is_checked = false;
};

/// ABI for fixed length data types
template <class T>
struct ABI <T, typename std::enable_if <std::is_trivially_copyable <T>::value && !std::is_fundamental <T>::value>::type> :
	public ABI_Fixed <T>
{};

/// ABI for fundamental data types
template <class T>
struct ABI <T, typename std::enable_if <std::is_fundamental <T>::value>::type> :
	public ABI_Fixed <T>
{
	typedef T ABI_in;

	class In
	{
	public:
		In (T val) :
			val_ (val)
		{}

		ABI_in operator & () const
		{
			return val_;
		}

	private:
		T val_;
	};

	static T in (ABI_in v)
	{
		return v;
	}
};

/// Base for variable length data types ABI
template <class T>
struct ABI_Variable : public ABI_Base <T>
{
	static const bool is_checked = true;

	using ABI_Base <T>::ABI_in;
	using ABI_Base <T>::ABI_out;
	using ABI_Base <T>::ABI_inout;
	using ABI_Base <T>::ABI_ret;

	using ABI_Base <T>::Var;

	class InOut
	{
	public:
		InOut (T& val) :
			ref_ (val)
		{}

		~InOut () noexcept (false);

		T* operator & () const
		{
			return &ref_;
		}

	private:
		T& ref_;
	};

	class Out : public InOut
	{
	public:
		Out (T& val) :
			InOut (val)
		{
			val = T ();	// Clear
		}
	};

	static const T& in (ABI_in p)
	{
		_check_pointer (p);
		ABI <T>::check (*p);
		return *p;
	}

	static T& inout (ABI_inout p)
	{
		_check_pointer (p);
		ABI <T>::check (*p);
		return *p;
	}

	static T& out (ABI_out p)
	{
		return inout (p);
	}

	static Var ret (ABI_ret val)
	{
		check_or_clear (val);
		return val;
	}

	static void check_or_clear (T& v);
};

/// Outline for compact code
template <class T>
void ABI_Variable <T>::check_or_clear (T& v)
{
	try {
		ABI <T>::check (v);
	} catch (...) {
		v.~T ();
		new (&v) T ();
		throw;
	}
}

/// Outline for compact code
template <class T>
ABI_Variable <T>::InOut::~InOut () noexcept (false)
{
	bool ex = uncaught_exception ();
	try {
		ABI <T>::check_or_clear (ref_);
	} catch (...) {
		if (!ex)
			throw;
	}
}

}
}

// Compatibility with modern C11 mapping
namespace IDL {

template <class T>
struct traits
{
	typedef typename CORBA::Nirvana::ABI <T>::Var value_type;
	typedef typename CORBA::Nirvana::ABI <T>::In in_type;
	typedef typename CORBA::Nirvana::ABI <T>::Out out_type;
	typedef typename CORBA::Nirvana::ABI <T>::InOut inout_type;
};

}

#endif
