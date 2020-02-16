#ifndef NIRVANA_ORB_ENVIRONMENT_C_H_
#define NIRVANA_ORB_ENVIRONMENT_C_H_

#include "Bridge.h"

namespace CORBA {

class Environment;

namespace Nirvana {

template <>
class Bridge < ::CORBA::Environment> :
	public BridgeMarshal < ::CORBA::Environment>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			void (*exception_set) (Bridge < ::CORBA::Environment>*, Long code, const char* rep_id, const void* param);
			const Char* (*exception_id) (Bridge < ::CORBA::Environment>*);
			const void* (*exception_value) (Bridge < ::CORBA::Environment>*);
			void (*exception_free) (Bridge < ::CORBA::Environment>*);
		}
		epv;
	};

	const EPV& _epv () const
	{
		return (EPV&)Bridge <Interface>::_epv ();
	}

	static const Char interface_id_ [];

protected:
	Bridge (const EPV& epv) :
		BridgeMarshal <Environment> (epv.interface)
	{}
};

}
}

#endif
