#ifndef NIRVANA_ORB_MARSHALTRAITS_SEQUENCE_H_
#define NIRVANA_ORB_MARSHALTRAITS_SEQUENCE_H_

#include "MarshalTraits_forward.h"
#include "sequence.h"
#include <Nirvana/core_objects.h>

namespace CORBA {
namespace Nirvana {

template <typename T>
struct MarshalTraits <Sequence <T> >
{
	static const bool has_move_out = true;
	static const bool has_unmarshal_in = MarshalTraits <T>::has_unmarshal_in;
	static const bool has_unmarshal_inout = true;

	typedef ABI <Sequence <T> > ABI;

	static void move_out (Sequence <T>& src, ::Nirvana::SynchronizationContext_ptr sc, Sequence <T>& dst);

	static void local_marshal (const Sequence <T>& src, Sequence <T>& dst);
	static void local_unmarshal_in (Sequence <T>& val);
	static void local_unmarshal_inout (Sequence <T>& val);
};

template <typename T>
void MarshalTraits <Sequence <T> >::move_out (Sequence <T>& src, ::Nirvana::SynchronizationContext_ptr sc, Sequence <T>& dst)
{
	assert (dst.empty ());
	if (!src.empty ()) {
		if (::Nirvana::g_current->synchronization_context ()->shared_memory (sc))
			src.swap (dst);
		else {
			ABI& asrc = static_cast <ABI&> (src);
			ABI& adst = static_cast <ABI&> (dst);
			if (!MarshalTraits <T>::has_move_out) {
				size_t size = asrc.size;
				size_t cb = asrc.allocated;
				adst.ptr = (T*)sc->adopt_output (src.ptr, size * sizeof (T), cb);
				adst.allocated = cb;
				adst.size = size;
				asrc.reset ();
			} else {
				size_t size = asrc.size;
				size_t cb = size * sizeof (T);
				adst.ptr = (T*)sc->allocate (cb);
				adst.allocated = cb;
				T* tsrc = asrc.ptr;
				T* tdst = adst.ptr;
				while (adst.size < size) {
					MarshalTraits <T>::move_out (*tsrc, sc, *tdst);
					++adst.size;
				}
				asrc.size = 0;
				src.shrink_to_fit (); // Release memory
			}
		}
	}
}

template <typename T>
void MarshalTraits <Sequence <T> >::local_marshal (const Sequence <T>& src, Sequence <T>& dst)
{
	if (src.empty ())
		dst.reset ();
	else {
		ABI& asrc = static_cast <ABI&> (src);
		ABI& adst = static_cast <ABI&> (dst);
		if (std::is_trivially_copyable <T> ()) {
			size_t cb = asrc.size * sizeof (T);
			adst.ptr = (T*)g_local_marshal->marshal_memory (asrc.ptr, cb);
			adst.allocated = cb;
			adst.size = asrc.size;
		} else {
			size_t cb = asrc.size * sizeof (T);
			void* buf;
			adst.ptr = (T*)g_local_marshal->get_buffer (cb, buf);
			T* dp = (T*)buf;
			const T* sp = asrc.ptr, * end = sp + asrc.size;
			do {
				MarshalTraits <T>::local_marshal (*(sp++), *(dp++));
			} while (sp != end);
		}
	}
}

template <typename T>
void MarshalTraits <Sequence <T> >::local_unmarshal_in (Sequence <T>& val)
{
	if (MarshalTraits <T>::has_unmarshal_in)
		for (T* p = val.data (), *end = p + val.size (); p != end; ++p)
			MarshalTraits <T>::local_unmarshal_in (*p);
}

template <typename T>
void MarshalTraits <Sequence <T> >::local_unmarshal_inout (Sequence <T>& val)
{
	if (!val.empty ()) {
		ABI& aval = static_cast <ABI&> (val);
		if (aval.allocated)
			g_local_marshal->adopt_memory (aval.ptr, aval.allocated);
		else
			aval.ptr = (T*)val.memory ()->copy (nullptr, aval.ptr, aval.allocated, 0);
		if (MarshalTraits <T>::has_unmarshal_inout) {
			T* p = aval.ptr, * end = p + aval.size;
			try {
				do {
					MarshalTraits <T>::local_unmarshal_inout (*p);
				} while (end != ++p);
			} catch (...) {
				aval.size = p - aval.ptr;
				val.~vector ();
				throw;
			}
		}
	}
}

}
}

#endif
