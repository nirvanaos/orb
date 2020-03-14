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
	static void marshal_in (const T& src, PlatformMarshal_ptr marshaler, typename Type <T>::ABI_type& dst) NIRVANA_NOEXCEPT
	{
		dst = (typename Type <T>::ABI_type&)src;
	}

	static void marshal_out (T& src, PlatformMarshal_ptr marshaler, typename Type <T>::ABI_type& dst) NIRVANA_NOEXCEPT
	{
		dst = (typename Type <T>::ABI_type&)src;
	}

	static void unmarshal (typename Type <T>::ABI_type& src, PlatformUnmarshal_ptr unmarshaler, T& dst) NIRVANA_NOEXCEPT
	{
		dst = Type <T>::in (src);
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
struct MarshalTraits <TypeCode_var>
{
	static void marshal_in (const TypeCode_ptr src, PlatformMarshal_ptr marshaler, Interface*& dst)
	{
		reinterpret_cast <uintptr_t&> (dst) = marshaler->marshal_type_code (src);
	}

	static void marshal_out (TypeCode_var& src, PlatformMarshal_ptr marshaler, Interface*& dst)
	{
		reinterpret_cast <uintptr_t&> (dst) = marshaler->marshal_type_code (src);
	}

	static void unmarshal (Interface* src, PlatformUnmarshal_ptr unmarshaler, TypeCode_var& dst)
	{
		dst = unmarshaler->unmarshal_type_code (src);
	}
};

template <class I> // I must derive from Object
struct MarshalTraits <I_var <I> >
{
	static void marshal_in (const I_ptr <I> src, PlatformMarshal_ptr marshaler, Interface*& dst)
	{
		reinterpret_cast <uintptr_t&> (dst) = marshaler->marshal_object (src);
	}

	static void marshal_out (I_var <I>& src, PlatformMarshal_ptr marshaler, Interface*& dst)
	{
		reinterpret_cast <uintptr_t&> (dst) = marshaler->marshal_object (src);
	}

	static void unmarshal (Interface* src, PlatformUnmarshal_ptr unmarshaler, I_var <I>& dst)
	{
		dst = static_cast <I*> (static_cast <Bridge <I>*> (unmarshaler->unmarshal_interface (src, Bridge <I>::interface_id_)));
	}
};

}
}

#endif
