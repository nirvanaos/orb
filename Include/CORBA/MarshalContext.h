// This file was generated from "MarshalContext.idl"
// Nirvana IDL compiler version 1.0
#ifndef IDL_MARSHALCONTEXT_H_
#define IDL_MARSHALCONTEXT_H_

namespace CORBA {
namespace Nirvana {

enum class MarshalContext : ABI_enum
{
	SHARED_MEMORY,
	SHARED_PROTECTION_DOMAIN,
	OTHER_PROTECTION_DOMAIN
};
extern const ::Nirvana::ImportInterfaceT <TypeCode> _tc_MarshalContext;

template <>
const Char RepIdOf <MarshalContext>::repository_id_ [] = "IDL:CORBA/Nirvana/MarshalContext:1.0";

template <>
struct Type <MarshalContext> : public TypeEnum <MarshalContext, MarshalContext::OTHER_PROTECTION_DOMAIN>
{};

}
}

#endif
