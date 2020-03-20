#ifndef NIRVANA_ORB_INTERFACE_H_
#define NIRVANA_ORB_INTERFACE_H_

#include <Nirvana/NirvanaBase.h>
#include "basic_types.h"
#include "StringBase.h"

namespace CORBA {
namespace Nirvana {

/// Base for all interface ABIs.
/// Provides life-cycle management and interface identification.
class Interface
{
	Interface (const Interface&) = delete;

public:
	/// Entry point vector
	struct EPV
	{
		const Char* interface_id;
		Interface* (*duplicate) (Interface* obj, Interface* environment);
		void (*release) (Interface*);
	};

	Interface (const EPV& epv) :
		_epv_ref (epv)
	{}

	const EPV& _epv () const
	{
		return _epv_ref;
	}

	Interface& operator = (const Interface&)
	{
		return *this;	// Do nothing
	}

	static Interface* _check (Interface* obj, String_in interface_id);

	static Interface* _nil ()
	{
		return 0;
	}

private:
	const EPV& _epv_ref;
};

Interface* interface_duplicate (Interface* obj);
void interface_release (Nirvana::Interface* itf) NIRVANA_NOEXCEPT;

typedef Interface EnvironmentBridge;

}

class Environment;

}

#endif
