#ifndef NIRVANA_CORE_RUNNABLE_H_
#define NIRVANA_CORE_RUNNABLE_H_

#include <Nirvana/Runnable_s.h>
#include "LifeCycle.h"

namespace Nirvana {
namespace Core {

template <class S>
class Runnable :
	public LifeCycle <S>,
	public CORBA::Nirvana::Servant <S, Nirvana::Runnable>
{};

}
}

#endif
