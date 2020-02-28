#ifndef NIRVANA_TESTORB_LOCALOBJECTCORE_H_
#define NIRVANA_TESTORB_LOCALOBJECTCORE_H_

#include "Server.h"
#include "ObjectImpl.h"
#include "ReferenceCounterImpl.h"

namespace CORBA {
namespace Nirvana {

class LocalObjectCore :
	public ObjectImpl <LocalObjectCore>,
	public LifeCycleNoCopy <LocalObjectCore>
{
public:
	LocalObjectCore (Object_ptr servant) :
		ObjectImpl <LocalObjectCore> (servant)
	{}
};

}
}

#endif