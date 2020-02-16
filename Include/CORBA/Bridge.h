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

	const EPV& _epv () const
	{
		return (const EPV&)Interface::_epv ();
	}

	/// Interface repository id
	static const Char interface_id_ [];

	/// Helper for widening to a base interface
	template <class Base>
	struct Wide
	{
		typedef Bridge <Base>* (*Func) (Bridge <I>*, const Char* base_id, Interface* environment);
	};

protected:
	Bridge (const EPV& epv) :
		Interface (epv.interface)
	{
#ifdef NIRVANA_C11
		static_assert (offsetof(epv, interface) == 0, "interface must be at the beginning of EPV.");
#endif
	}
};

#define BASE_STRUCT_ENTRY(type, name) Wide < type>::Func name;\
operator const Wide < type>::Func () const { return name; }

#define BRIDGE_BEGIN(I) template <> struct Bridge < I>::EPV { Interface::EPV interface; struct {
#define BRIDGE_END() } epv;};

template <>
class Bridge <Interface> :
	public Interface
{};

}
}

#endif
