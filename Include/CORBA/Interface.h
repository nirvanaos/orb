#ifndef NIRVANA_ORB_INTERFACE_H_
#define NIRVANA_ORB_INTERFACE_H_

#include <Nirvana/NirvanaBase.h>
#include "BasicTypes.h"

namespace CORBA {
namespace Nirvana {

/// Base for all interface ABIs.
/// Provides life-cycle management and interface identification.
struct Interface
{
	/// Entry point vector
	struct EPV
	{
		const Char* interface_id;
		Interface* (*duplicate) (Interface* obj, Interface* environment);
		void (*release) (Interface*);
	};

	const EPV* const _epv_ref;

	Interface (const EPV& epv) :
		_epv_ref (&epv)
	{}

	const EPV& _epv () const
	{
		return *_epv_ref;
	}

	Interface& operator = (const Interface&)
	{
		return *this;	// Do nothing
	}

	static NIRVANA_NODISCARD Interface* _check (Interface* obj, const Char* interface_id);
	static NIRVANA_NODISCARD Interface* _check (Interface* obj)
	{
		return obj;
	}

	static Interface* _nil ()
	{
		return nullptr;
	}
};

NIRVANA_NODISCARD Interface* duplicate_interface (Interface* obj);

}

/// CORBA::is_nil()
inline bool is_nil (Nirvana::Interface* itf)
{
	return !itf;
}

/// CORBA::release()
void release (Nirvana::Interface* itf);

}

#endif
