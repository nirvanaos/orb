#ifndef NIRVANA_ORB_LOCALOBJECT_S_H_
#define NIRVANA_ORB_LOCALOBJECT_S_H_

#include "LocalObject.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, LocalObject>
{
public:
	static const typename Bridge <LocalObject>::EPV epv_;
};

template <class S>
const Bridge <LocalObject>::EPV Skeleton <S, LocalObject>::epv_ = {
	{ // interface
		Bridge <LocalObject>::repository_id_,
		S::template __duplicate <LocalObject>,
		S::template __release <LocalObject>
	},
	{ // base
		S::template _wide_object <LocalObject>,
		S::template _wide <ReferenceCounter, LocalObject>
	}
};

}
}

#endif
