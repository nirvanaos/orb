#ifndef NIRVANA_ORB_TYPESCALAR_H_
#define NIRVANA_ORB_TYPESCALAR_H_

#include "TypeBase.h"

namespace CORBA {
namespace Nirvana {

/// Scalar data type. Passes in parameter by value.
template <class T>
struct TypeScalar :
	public TypeBase <T>
{
	typedef T ABI_in;
	using TypeBase <T>::ABI_out;
	using TypeBase <T>::ABI_inout;
	using TypeBase <T>::ABI_ret;

	using TypeBase <T>::C_out;
	using TypeBase <T>::C_inout;
	using TypeBase <T>::C_ret;

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

	static T in (ABI_in v)
	{
		return v;
	}
};

}
}

#endif
