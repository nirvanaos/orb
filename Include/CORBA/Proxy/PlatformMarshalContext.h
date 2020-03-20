// enum PlatformMarshalContext
#ifndef NIRVANA_ORB_PLATFORMMARSHALCONTEXT_H_
#define NIRVANA_ORB_PLATFORMMARSHALCONTEXT_H_

#include "../TypeEnum.h"
#include <Nirvana/throw_exception.h>

namespace CORBA {
namespace Nirvana {

enum class PlatformMarshalContext : ABI_enum
{
	SHARED_MEMORY,
	SHARED_PROTECTION_DOMAIN,
	OTHER_PROTECTION_DOMAIN
};

template <> struct Type <PlatformMarshalContext> : 
	TypeEnum <PlatformMarshalContext, PlatformMarshalContext::OTHER_PROTECTION_DOMAIN>
{};

}
}

#endif
