#ifndef NIRVANA_ORB_ABI_H_
#define NIRVANA_ORB_ABI_H_

#include <type_traits>
#include <new>
#include <utility>

namespace CORBA {
namespace Nirvana {

void _check_pointer (const void* p);
bool uncaught_exception ();

template <class T, class = void> struct ABI;

template <class T>
struct ABI_Base
{
	typedef T Var;

	typedef const T* ABI_in;
	typedef T* ABI_out;
	typedef T* ABI_inout;
	typedef Var ABI_ret;

	typedef T C_ret;

	typedef const T& In;
	typedef T& InOut;
	typedef T& Out;

	static const T& in (const T* p)
	{
		_check_pointer (p);
		return *p;
	}

	static T& inout (T* p)
	{
		_check_pointer (p);
		return *p;
	}

	static T& out (T* p)
	{
		return inout (p);
	}
};

/// Base for fixed length data types ABI
template <class T>
struct FixedABI : public ABI_Base <T>
{
	static const bool is_fixed = true;
};

/// ABI for fixed length data types
template <class T>
struct ABI <T, typename std::enable_if <std::is_trivially_copyable <T>::value && !std::is_fundamental <T>::value>::type> :
	public FixedABI <T>
{};

/// ABI for fundamental data types
template <class T>
struct ABI <T, typename std::enable_if <std::is_fundamental <T>::value>::type> :
	public FixedABI <T>
{
	typedef T ABI_in;
	typedef T In;

	static T in (T v)
	{
		return v;
	}
};

/// Base for variable length data types ABI
template <class T>
struct VariableABI : public ABI_Base <T>
{
	static const bool is_fixed = false;

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

	class C_ret
	{
	public:
		C_ret (typename ABI <T>::Var&& val) :
			val_ (std::move (val))
		{
			check_or_clear (val_);
		}

		operator typename ABI <T>::Var ()
		{
			return std::move (val_);
		}

	private:
		typename ABI <T>::Var val_;
	};

	static const T& in (const T* p)
	{
		_check_pointer (p);
		ABI <T>::check (*p);
		return *p;
	}

	static T& inout (T* p)
	{
		_check_pointer (p);
		ABI <T>::check (*p);
		return *p;
	}

	static T& out (T* p)
	{
		return inout (p);
	}

	static void check_or_clear (T& v);
};

/// Outline for compact code
template <class T>
void VariableABI <T>::check_or_clear (T& v)
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
VariableABI <T>::InOut::~InOut () noexcept (false)
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
	typedef T value_type;
	typedef typename CORBA::Nirvana::ABI <T>::In in_type;
	typedef typename CORBA::Nirvana::ABI <T>::Out out_type;
	typedef typename CORBA::Nirvana::ABI <T>::InOut inout_type;
};

}

#endif
