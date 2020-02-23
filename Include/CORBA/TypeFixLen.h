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
	using TypeBase <T>::ABI_vt_ret;

	using TypeBase <T>::C_in;
	using TypeBase <T>::C_out;
	using TypeBase <T>::C_inout;
	using TypeBase <T>::C_ret;

};

}
}

#endif
