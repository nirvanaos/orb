//! \file LocalMarshal.h.
//!
//! \brief Local marshaling functions for use in proxy code.

#ifndef NIRVANA_ORB_MARSHALTRAITS_H_
#define NIRVANA_ORB_MARSHALTRAITS_H_

#include "MarshalTraits_string.h"
#include "MarshalTraits_sequence.h"

namespace CORBA {
namespace Nirvana {

template <class T
#ifdef NIRVANA_C11
	, typename Enable = void
#endif
>
struct MarshalTraitsTrivial
{
	static void marshal_in (const T& src, PlatformMarshal_ptr marshaler, T& dst) NIRVANA_NOEXCEPT
	{
		dst = src;
	}

	static void marshal_out (T& src, PlatformMarshal_ptr marshaler, T& dst) NIRVANA_NOEXCEPT
	{
		dst = src;
	}

	static void unmarshal (T& src, PlatformUnmarshal_ptr unmarshaler, T& dst) NIRVANA_NOEXCEPT
	{
		dst = src;
	}

	static bool has_unmarshal (PlatformMarshalContext mctx)
	{
		return false;
	}
};

template <class T> struct MarshalTraits :
	MarshalTraitsTrivial <T
#ifdef NIRVANA_C11
	, typename std::enable_if <std::is_trivially_copyable <T>::value>::type
#endif
	>
{};

template <>
struct MarshalTraits <TypeCode_var> :
	public MarshalTraitsTrivial <TypeCode_var>
{
	static void marshal_in (const TypeCode_ptr src, PlatformMarshal_ptr marshaler, Interface*& dst)
	{
		if (marshaler->context () == PlatformMarshalContext::OTHER_PROTECTION_DOMAIN)
			reinterpret_cast <uintptr_t&> (dst) = marshaler->marshal_type_code (src);
		else
			dst = interface_duplicate (&src);
	}

	static void marshal_out (TypeCode_var& src, PlatformMarshal_ptr marshaler, Interface*& dst)
	{
		if (marshaler->context () == PlatformMarshalContext::OTHER_PROTECTION_DOMAIN) {
			reinterpret_cast <uintptr_t&> (dst) = marshaler->marshal_type_code (src);
			src._retn ();
		} else
			dst = &src._retn ();
	}

	static void unmarshal (Interface* src, PlatformUnmarshal_ptr unmarshaler, TypeCode_var& dst)
	{
		if (unmarshaler->context () == PlatformMarshalContext::OTHER_PROTECTION_DOMAIN)
			dst = unmarshaler->unmarshal_type_code (src);
		else
			dst = I_ret <TypeCode> (src);
	}

	static bool has_unmarshal (PlatformMarshalContext mctx)
	{
		return mctx == PlatformMarshalContext::OTHER_PROTECTION_DOMAIN;
	}
};

template <class I> // I must derive from Object
struct MarshalTraits <I_var <I> > :
	public MarshalTraitsTrivial <I_var <I> >
{
	static void marshal_in (const I_ptr <I> src, PlatformMarshal_ptr marshaler, Interface*& dst)
	{
		if (marshaler->context () == PlatformMarshalContext::OTHER_PROTECTION_DOMAIN)
			reinterpret_cast <uintptr_t&> (dst) = marshaler->marshal_object (src);
		else
			dst = interface_duplicate (&src);
	}

	static void marshal_out (I_var <I>& src, PlatformMarshal_ptr marshaler, Interface*& dst)
	{
		if (marshaler->context () == PlatformMarshalContext::OTHER_PROTECTION_DOMAIN) {
			reinterpret_cast <uintptr_t&> (dst) = marshaler->marshal_object (src);
			src._retn ();
		} else
			dst = &src._retn ();
	}

	static void unmarshal (Interface* src, PlatformUnmarshal_ptr unmarshaler, I_var <I>& dst)
	{
		if (unmarshaler->context () == PlatformMarshalContext::OTHER_PROTECTION_DOMAIN)
			dst = unmarshaler->unmarshal_type_code (src);
		else
			dst = I_ret <I> (src);
	}

	static bool has_unmarshal (PlatformMarshalContext mctx)
	{
		return mctx == PlatformMarshalContext::OTHER_PROTECTION_DOMAIN;
	}
};

}
}

#endif
