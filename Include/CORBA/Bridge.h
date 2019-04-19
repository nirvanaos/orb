#ifndef NIRVANA_ORB_BRIDGE_H_
#define NIRVANA_ORB_BRIDGE_H_

#include "BasicTypes.h"

namespace CORBA {

class Environment;

namespace Nirvana {

//! All interfaces derives from Interface class.
//! This class provides life-cycle management and pointer to entry-point vector (EPV).
class Interface;

//! Intermediate part of an interface, bridge between servant and client.
template <class I> class Bridge;
template <class I> class BridgeMarshal;

typedef BridgeMarshal < ::CORBA::Environment> EnvironmentBridge;

//! All bridges indirectly derives from `Bridge <Interface>'.
template <>
class Bridge <Interface>
{
public:
	Bridge& operator = (const Bridge&)
	{
		return *this;	// Do nothing
	}

	struct EPV
	{
		const Char* interface_id;
		Bridge <Interface>* (*duplicate) (Bridge <Interface>*, EnvironmentBridge*);
		void (*release) (Bridge <Interface>*);
	};

	const EPV& _epv () const
	{
		return epv_ref_;
	}

protected:
	Bridge (const EPV& epv) :
		epv_ref_ (epv)
	{}

private:
	const EPV& epv_ref_;
};

//! The bridge which was passed from a different binary file.
//! We can't be sure that the interface version is the same as for current binary.
//! To obtain the interface pointer we have to unmarshal the bridge pointer.
//! Each Bridge <I> derives from BridgeMarshal <I>.
template <class I>
class BridgeMarshal :
	public Bridge <Interface>
{
public:
	template <class Base>
	struct Wide
	{
		typedef Bridge <Base>* (*Func) (Bridge <I>*, const Char*, EnvironmentBridge*);
	};

protected:
	BridgeMarshal (const EPV& epv) :
		Bridge <Interface> (epv)
	{}
};

#define BASE_STRUCT_ENTRY(type, name) Wide < type>::Func name;\
operator const Wide < type>::Func () const { return name; }

}
}

#endif

