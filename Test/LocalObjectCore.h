#ifndef NIRVANA_TESTORB_LOCALOBJECTCORE_H_
#define NIRVANA_TESTORB_LOCALOBJECTCORE_H_

#include <CORBA/Object_s.h>
#include "ObjectImpl.h"
#include "ReferenceCounterImpl.h"

namespace CORBA {
namespace Nirvana {

class LocalObjectCore :
	public ObjectImpl <LocalObjectCore>,
	public LifeCycleNoCopy <LocalObjectCore>
{
public:
	LocalObjectCore (AbstractBase_ptr servant) :
		ObjectImpl <LocalObjectCore> (servant)
	{}
};

}
}

#endif