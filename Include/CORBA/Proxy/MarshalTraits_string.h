#ifndef NIRVANA_ORB_MARSHALTRAITS_STRING_H_
#define NIRVANA_ORB_MARSHALTRAITS_STRING_H_

#include "MarshalTraits_forward.h"
#include "../String.h"
#include "Marshal.h"
#include "Unmarshal.h"

namespace CORBA {
namespace Nirvana {

template <typename C>
struct MarshalTraits <StringT <C> >
{
	static const bool has_marshal = true;

	typedef StringT <C> Var;
	typedef ABI <Var> ABI;

	static bool _small_copy (const ABI& src, ABI& dst);

	static void marshal_in (const Var& src, Marshal_ptr marshaler, ABI& dst);

	static void marshal_out (Var& src, Marshal_ptr marshaler, ABI& dst);

	static void unmarshal (ABI& src, Unmarshal_ptr unmarshaler, Var& dst)
	{
		if (Type <Var>::has_check)
			Type <Var>::check (src);

		if (src.is_large ()) {
			size_t cb = src.allocated ();
			if (cb)
				unmarshaler->adopt_memory (src.large_pointer (), cb);
			else
				dst.assign_internal (src.large_size (), src.large_pointer ());
		} else
			static_cast <ABI&> (dst) = src;
	}
};

template <typename C>
void MarshalTraits <StringT <C> >::marshal_in (const Var& src, Marshal_ptr marshaler, ABI& dst)
{
	assert (&src != &dst);
	if (!_small_copy (src, dst)) {
		size_t size = src.large_size ();
		size_t cb = StringT <C>::byte_size (size);
		dst.large_pointer ((C*)marshaler->marshal_memory (const_cast <C*> (src.large_pointer ()), cb, 0));
		dst.large_size (size);
		dst.allocated (cb);
	}
}

template <typename C>
void MarshalTraits <StringT <C> >::marshal_out (Var& src, Marshal_ptr marshaler, ABI& dst)
{
	assert (&src != &dst);
	if (!_small_copy (src, dst)) {
		size_t size = src.large_size ();
		size_t cb = StringT <C>::byte_size (size);
		dst.large_pointer ((C*)marshaler->marshal_memory (src.large_pointer (), cb, src.allocated ()));
		dst.large_size (size);
		dst.allocated (cb);
		src.reset ();
	}
}

template <typename C>
bool MarshalTraits <StringT <C> >::_small_copy (const ABI& src, ABI& dst)
{
	const C* p;
	size_t len;
	if (src.is_large ()) {
		len = src.large_size ();
		if (len > ABI::SMALL_CAPACITY)
			return false;
		p = src.large_pointer ();
	} else {
		p = src.small_pointer ();
		len = src.small_size ();
	}
	::Nirvana::real_copy (p, p + len + 1, dst.small_pointer ());
	dst.small_size (len);
	return true;
}

}
}

#endif
