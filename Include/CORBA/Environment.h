#ifndef NIRVANA_ORB_ENVIRONMENT_H_
#define NIRVANA_ORB_ENVIRONMENT_H_

#include "EnvironmentImpl.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class EnvironmentImpl :
	public EnvironmentBase,
	public Skeleton <S, ::CORBA::Environment>,
	public ServantTraits <S>
{
protected:
	EnvironmentImpl () :
		EnvironmentBase (Skeleton <S, ::CORBA::Environment>::epv_)
	{}
};

class Environment :
	public EnvironmentImpl <Environment>,
	public LifeCycleNoCopy <Environment>
{
public:
	Environment () {} // Inline for performance

	Environment (const EnvironmentBase& src);
	Environment (EnvironmentBase&& src) NIRVANA_NOEXCEPT;

	Environment& operator = (const EnvironmentBase& src);
	Environment& operator = (EnvironmentBase&& src) NIRVANA_NOEXCEPT;
};

}
}

#endif
