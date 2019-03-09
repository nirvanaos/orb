#ifndef NIRVANA_ORB_INTERFACE_S_H_
#define NIRVANA_ORB_INTERFACE_S_H_

#include "Interface_c.h"
#include "RepositoryId.h"

namespace CORBA {
namespace Nirvana {

extern void _check_pointer (const void* p);
extern void _check_pointer (const Bridge <Interface>* obj, const Bridge <Interface>::EPV& epv);

template <class S, class I> class Skeleton;

//! Servant end of bridge.
template <class I>
class ServantBridge :
	public Bridge <I>
{
public:
	//! We can simple assign servant pointer to `T_ptr <I>'
	operator I& ()
	{
		return static_cast <I&> (static_cast <Bridge <I>&> (*this));
	}

protected:
	ServantBridge (const typename Bridge <I>::EPV& epv) :
		Bridge <I> (epv)
	{}
};

template <class I> class FindInterface;

}
}

#endif
