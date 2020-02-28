#ifndef NIRVANA_ORB_SERVANTPOA_H_
#define NIRVANA_ORB_SERVANTPOA_H_

#include "ServantImpl.h"

namespace CORBA {
namespace Nirvana {

template <class I> class ServantPOA;

class ServantTraitsPOA :
	public ServantTraits <ServantTraitsPOA>
{
public:
	template <class I, class IS>
	static ServantPOA <IS>& __implementation (Bridge <I>* bridge)
	{
		_check_pointer (bridge, Skeleton <ServantPOA <IS>, I>::epv_.header);
		return static_cast <ServantPOA <IS>&> (*bridge);
	}

	template <class I>
	static ServantPOA <I>& _implementation (Bridge <I>* bridge)
	{
		return __implementation <I, I> (bridge);
	}
};

}
}

#endif
