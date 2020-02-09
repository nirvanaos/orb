#ifndef NIRVANA_ORB_ABI_H_
#define NIRVANA_ORB_ABI_H_

#include "ABI_forward.h"
#include <type_traits>
#include <new>

namespace CORBA {
namespace Nirvana {

/// Outline for compact code
template <class T>
void ABI_VariableLen <T>::check_or_clear (T& v)
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
ABI_VariableLen <T>::InOut::~InOut () noexcept (false)
{
	bool ex = uncaught_exception ();
	try {
		ABI <T>::check_or_clear (ref_);
	} catch (...) {
		if (!ex)
			throw;
	}
}

/// ABI for fixed length data types
template <class T>
struct ABI <T, typename std::enable_if <std::is_trivially_copyable <T>::value && !std::is_fundamental <T>::value>::type> :
	public ABI_FixedLen <T>
{};

/// ABI for fundamental data types
template <class T>
struct ABI <T, typename std::enable_if <std::is_fundamental <T>::value>::type> :
	public ABI_FixedLen <T>
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
