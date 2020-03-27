/// \file MarshalContext.h
#ifndef NIRVANA_ORB_MARSHALCONTEXT_H_
#define NIRVANA_ORB_MARSHALCONTEXT_H_

#include "../TypeEnum.h"
#include <Nirvana/throw_exception.h>

namespace CORBA {
namespace Nirvana {

/// \brief Type of the inter-domain marshal context.
enum class MarshalContext : ABI_enum
{
	SHARED_MEMORY,            ///< Both domains share common memory heap.
	SHARED_PROTECTION_DOMAIN, ///< Different heaps in the common protection domain.
	OTHER_PROTECTION_DOMAIN   ///< Different protection domains.
};

template <> struct Type <MarshalContext> : 
	TypeEnum <MarshalContext, MarshalContext::OTHER_PROTECTION_DOMAIN>
{};

}
}

#endif
