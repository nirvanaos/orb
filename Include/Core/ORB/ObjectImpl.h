#ifndef NIRVANA_ORB_CORE_OBJECTIMPL_H_
#define NIRVANA_ORB_CORE_OBJECTIMPL_H_

#include <CORBA/CORBA.h>

namespace CORBA {
namespace Nirvana {
namespace Core {

/// Implements Object operations over ServantBase.
class ObjectImpl
{
public:
	// Object operations

	ULong _hash (ULong maximum) const
	{
		return (ULong)(uintptr_t)this % maximum;
	}
	// TODO: More Object operations shall be here...

};

}
}
}

#endif
