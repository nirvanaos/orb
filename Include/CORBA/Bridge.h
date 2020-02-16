#ifndef NIRVANA_ORB_BRIDGE_H_
#define NIRVANA_ORB_BRIDGE_H_

#include "Interface.h"

namespace CORBA {

class Environment;

namespace Nirvana {

typedef Interface EnvironmentBridge;

/// The ABI for a particular interface I.
/// A "bridge" between the client and servant sides.
template <class I>
class Bridge :
	public Interface
{
public:
	/// Entry-point vector
	struct EPV;

	/// Helper for widening to a base interface
	template <class Base>
	struct Wide
	{
		typedef Bridge <Base>* (*Func) (Bridge <I>*, const Char* base_id, Interface* environment);
	};

protected:
	Bridge (const EPV& epv)
	{
		const Interface::EPV* itf = &epv.interface;
#ifdef NIRVANA_C11
		static_assert ((const void*)itf == (const void*)&epv, "interface must be at the beginning of EPV.");
#endif
		_epv_ref = itf;
	}
};

#define BASE_STRUCT_ENTRY(type, name) Wide < type>::Func name;\
operator const Wide < type>::Func () const { return name; }

template <>
class Bridge <Interface> :
	public Interface
{};

}
}

#endif
