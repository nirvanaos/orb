#ifndef NIRVANA_ORB_REFERENCECOUNTERSTATIC_H_
#define NIRVANA_ORB_REFERENCECOUNTERSTATIC_H_

#include "ReferenceCounter_s.h"
#include "ServantStatic.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class InterfaceStatic <S, ReferenceCounter> :
	public InterfaceStaticBase <S, ReferenceCounter>
{
public:
	static void __add_ref (Bridge <ReferenceCounter>* obj, Interface* env)
	{}

	static void __remove_ref (Bridge <ReferenceCounter>* obj, Interface* env)
	{}

	static ULong __refcount_value (Bridge <ReferenceCounter>* obj, Interface* env)
	{
		return 1;
	}
};

}
}

#endif
