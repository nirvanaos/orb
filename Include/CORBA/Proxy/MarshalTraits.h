//! \file LocalMarshal.h.
//!
//! \brief Local marshaling functions for use in proxy code.

#ifndef NIRVANA_ORB_MARSHALTRAITS_H_
#define NIRVANA_ORB_MARSHALTRAITS_H_

#include "MarshalTraits_string.h"
#include "MarshalTraits_sequence.h"
#include "MarshalTraits_any.h"

namespace CORBA {
namespace Nirvana {

template <class T
#ifdef NIRVANA_C11
	, typename Enable = void
#endif
>
struct MarshalTraitsTrivial
{
	static const bool has_marshal = false;

	static void marshal_in (const T& src, Marshal_ptr marshaler, typename Type <T>::ABI_type& dst) NIRVANA_NOEXCEPT
	{
		dst = (typename Type <T>::ABI_type&)src;
	}

	static void marshal_out (T& src, Marshal_ptr marshaler, typename Type <T>::ABI_type& dst) NIRVANA_NOEXCEPT
	{
		dst = (typename Type <T>::ABI_type&)src;
	}

	static void unmarshal (typename Type <T>::ABI_type& src, Unmarshal_ptr unmarshaler, T& dst) NIRVANA_NOEXCEPT
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
struct MarshalTraits <Boolean>
{
	static const bool has_marshal = false;

	static void marshal_in (const Boolean src, Marshal_ptr marshaler, ABI_boolean& dst) NIRVANA_NOEXCEPT
	{
		dst = src;
	}

	static void marshal_out (Boolean src, Marshal_ptr marshaler, ABI_boolean& dst) NIRVANA_NOEXCEPT
	{
		dst = src;
	}

	static void unmarshal (ABI_boolean src, Unmarshal_ptr unmarshaler, Boolean& dst) NIRVANA_NOEXCEPT
	{
		dst = src != 0;
	}
};

template <>
struct MarshalTraits <TypeCode_var>
{
	static const bool has_marshal = true;

	static void marshal_in (const TypeCode_ptr src, Marshal_ptr marshaler, Interface*& dst)
	{
		reinterpret_cast <uintptr_t&> (dst) = marshaler->marshal_type_code (src);
	}

	static void marshal_out (TypeCode_var& src, Marshal_ptr marshaler, Interface*& dst)
	{
		reinterpret_cast <uintptr_t&> (dst) = marshaler->marshal_type_code (src);
	}

	static void unmarshal (Interface* src, Unmarshal_ptr unmarshaler, TypeCode_var& dst)
	{
		dst = unmarshaler->unmarshal_type_code (src);
	}
};

template <class I> // I must derive from Object
struct MarshalTraits <I_var <I> >
{
	static const bool has_marshal = true;

	static void marshal_in (const I_ptr <I> src, Marshal_ptr marshaler, Interface*& dst)
	{
		reinterpret_cast <uintptr_t&> (dst) = marshaler->marshal_object (src);
	}

	static void marshal_out (I_var <I>& src, Marshal_ptr marshaler, Interface*& dst)
	{
		reinterpret_cast <uintptr_t&> (dst) = marshaler->marshal_object (src);
	}

	static void unmarshal (Interface* src, Unmarshal_ptr unmarshaler, I_var <I>& dst)
	{
		dst = static_cast <I*> (static_cast <Bridge <I>*> (unmarshaler->unmarshal_interface (src, Bridge <I>::repository_id_)));
	}
};

template <class T, class ABI> inline
void _marshal_in (const T& src, Marshal_ptr marshaler, ABI& dst)
{
	MarshalTraits <T>::marshal_in (src, marshaler, dst);
}

template <class I> inline
void _marshal_in (const I_ptr <I> src, Marshal_ptr marshaler, Interface*& dst)
{
	MarshalTraits <I_var <I> >::marshal_in (src, marshaler, dst);
}

inline
void _marshal_in (const TypeCode_ptr src, Marshal_ptr marshaler, Interface*& dst)
{
	MarshalTraits <TypeCode_var>::marshal_in (src, marshaler, dst);
}

template <class T, class ABI> inline
void _marshal_out (T& src, Marshal_ptr marshaler, ABI& dst)
{
	MarshalTraits <T>::marshal_out (src, marshaler, dst);
}

template <class T, class ABI> inline
void _unmarshal (ABI& src, Unmarshal_ptr unmarshaler, T& dst)
{
	MarshalTraits <T>::unmarshal (src, unmarshaler, dst);
}

}
}

#endif
