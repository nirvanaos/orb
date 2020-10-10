#ifndef NIRVANA_ORB_INTERFACE_H_
#define NIRVANA_ORB_INTERFACE_H_

#include <Nirvana/NirvanaBase.h>
#include "basic_types.h"
#include "StringBase.h"

namespace CORBA {
namespace Nirvana {

template <class I> class I_ptr;
class Interface;
template <> class I_ptr <Interface>;

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

	static I_ptr <Interface> _nil ();

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
