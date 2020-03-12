// enum PlatformMarshalContext
#ifndef NIRVANA_ORB_PLATFORMMARSHALCONTEXT_H_
#define NIRVANA_ORB_PLATFORMMARSHALCONTEXT_H_

namespace CORBA {
namespace Nirvana {

enum class PlatformMarshalContext : ABI_enum
{
	SHARED_MEMORY,
	SHARED_PROTECTION_DOMAIN,
	OTHER_PROTECTION_DOMAIN
};

template <> struct Type <PlatformMarshalContext> : TypeEnum <PlatformMarshalContext>
{
	static void check (ABI_enum val)
	{
		if (val > (ABI_enum)PlatformMarshalContext::OTHER_PROTECTION_DOMAIN)
			::Nirvana::throw_BAD_PARAM ();
	}
};

}
}

#endif