#ifndef NIRVANA_TESTORB_LOCALOBJECTCORE_H_
#define NIRVANA_TESTORB_LOCALOBJECTCORE_H_

#include "ObjectImpl.h"

namespace CORBA {
namespace Nirvana {

class LocalObjectCore :
	public ServantTraits <LocalObjectCore>,
	public LifeCycleNoCopy <LocalObjectCore>,
	public ObjectImpl <LocalObjectCore>
{
public:
	LocalObjectCore (DynamicServant_ptr servant) :
		ObjectImpl <LocalObjectCore> (servant)
	{}

	template <class Base, class Derived>
	static Bridge <Base>* _wide (Bridge <Derived>* derived, const Char* id, EnvironmentBridge* env)
	{ // Don't get base interfaces from this.
		env->set_exception (NO_IMPLEMENT ());
		return nullptr;
	}
};

}
}

#endif