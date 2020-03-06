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
struct MarshalTraitsSimple
{
	static const bool has_move_out = false;
	static const bool has_unmarshal_in = false;
	static const bool has_unmarshal_inout = false;

	static void move_out (T& src, ::Nirvana::SynchronizationContext_ptr sc, T& dst)
	{
		dst = src;
	}

	static void local_marshal (const T& src, T& dst) NIRVANA_NOEXCEPT
	{
		dst = src;
	}

	static void local_unmarshal_in (T& val) NIRVANA_NOEXCEPT
	{}

	static void local_unmarshal_inout (T& val) NIRVANA_NOEXCEPT
	{}
};

template <class T> struct MarshalTraits :
	MarshalTraitsSimple <T
#ifdef NIRVANA_C11
	, typename std::enable_if <std::is_trivially_copyable <T>::value>::type
#endif
	>
{};

template <>
struct MarshalTraits <TypeCode_var> :
	public MarshalTraitsSimple <TypeCode_var>
{
	static const bool has_unmarshal_in = true;
	static const bool has_unmarshal_inout = true;

	static void local_marshal (TypeCode_ptr src, TypeCode_var& dst)
	{
		reinterpret_cast <uintptr_t&> (dst) = g_local_marshal->marshal_type_code (src);
	}

	static void local_unmarshal_in (TypeCode_var& val)
	{
		val = g_local_marshal->unmarshal_type_code (reinterpret_cast <void*&> (val));
	}

	static void local_unmarshal_inout (TypeCode_var& val)
	{
		local_unmarshal_in (val);
	}
};

template <class I> // I must derive from Object
struct MarshalTraits <I_var <I> > :
	public MarshalTraitsSimple <I_var <I> >
{
	static const bool has_unmarshal_in = true;
	static const bool has_unmarshal_inout = true;

	static void local_marshal (Object_ptr src, I_var <I>& dst)
	{
		reinterpret_cast <uintptr_t&> (dst) = g_local_marshal->marshal_object (src);
	}

	static void local_unmarshal_in (I_var <I>& val)
	{
		Interface_ptr ip = g_local_marshal->unmarshal_interface (reinterpret_cast <void*&> (val), Bridge <I>::interface_id_);
		val = I_var <I> (static_cast <I*> (&ip));
	}

	static void local_unmarshal_inout (I_var <I>& val)
	{
		local_unmarshal_in (val);
	}
};

}
}

#endif
