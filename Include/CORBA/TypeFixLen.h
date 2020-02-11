#ifndef NIRVANA_ORB_TYPEFIXLEN_H_
#define NIRVANA_ORB_TYPEFIXLEN_H_

#include "TypeBase.h"

namespace CORBA {
namespace Nirvana {

/// Base for fixed-length data types
template <class T>
struct TypeFixLen : public TypeBase <T>
{
	using TypeBase <T>::ABI_in;
	using TypeBase <T>::ABI_out;
	using TypeBase <T>::ABI_inout;
	using TypeBase <T>::ABI_ret;

	using TypeBase <T>::C_in;
	using TypeBase <T>::C_out;
	using TypeBase <T>::C_inout;
	using TypeBase <T>::C_ret;

	// Client T_var class for the C++ IDL mapping standard conformance
	class C_var :
		public T
	{
	public:
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
			return *this;
		}
	};
};

}
}

#endif
