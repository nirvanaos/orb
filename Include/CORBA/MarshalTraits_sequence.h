#ifndef NIRVANA_ORB_MARSHALTRAITS_SEQUENCE_H_
#define NIRVANA_ORB_MARSHALTRAITS_SEQUENCE_H_

#include "MarshalTraits_forward.h"
#include "sequence.h"
#include "PlatformMarshal.h"
#include "PlatformUnmarshal.h"

namespace CORBA {
namespace Nirvana {

template <typename T>
struct MarshalTraits <Sequence <T> >
{
	typedef Sequence <T> Seq;
	typedef ABI <Sequence <T> > SeqABI;

	static void marshal_in (const Seq& src, PlatformMarshal_ptr marshaler, SeqABI& dst);
	static void marshal_out (Seq& src, PlatformMarshal_ptr marshaler, SeqABI& dst);
	static void unmarshal (SeqABI& src, PlatformUnmarshal_ptr unmarshaler, Seq& dst);

	static bool has_unmarshal (PlatformMarshalContext mctx)
	{
		return true;
	}
};

template <typename T>
void MarshalTraits <Sequence <T> >::marshal_in (const Seq& src, PlatformMarshal_ptr marshaler, SeqABI& dst)
{
	assert (&src != &dst);
	if (src.empty ())
		dst.reset ();
	else {
		SeqABI& asrc = static_cast <SeqABI&> (src);
		if (std::is_trivially_copyable <T> ()) {
			size_t cb = asrc.size * sizeof (T);
			dst.ptr = (ABI <T>*)marshaler->marshal_memory (asrc.ptr, cb, 0);
			dst.allocated = cb;
			dst.size = asrc.size;
		} else {
			size_t cb = asrc.size * sizeof (T);
			void* buf;
			dst.ptr = (ABI <T>*)marshaler->get_buffer (cb, buf);
			ABI <T>* dp = (ABI <T>*)buf;
			const T* sp = src.data (), * end = sp + src.size ();
			do {
				MarshalTraits <T>::marshal_in (*(sp++), marshaler, *(dp++));
			} while (sp != end);
		}
	}
}

template <typename T>
void MarshalTraits <Sequence <T> >::marshal_out (Seq& src, PlatformMarshal_ptr marshaler, SeqABI& dst)
{
	assert (&src != &dst);
	if (src.empty ())
		dst.reset ();
	else {
		SeqABI& asrc = static_cast <SeqABI&> (src);
		if (std::is_trivially_copyable <T> ()) {
			size_t cb = asrc.size * sizeof (T);
			dst.ptr = (ABI <T>*)marshaler->marshal_memory (asrc.ptr, cb, asrc.allocated ());
			dst.allocated = cb;
			dst.size = asrc.size;
			asrc.reset ();
		} else {
			size_t cb = asrc.size * sizeof (T);
			void* buf;
			adst.ptr = (ABI <T>*)marshaler->get_buffer (cb, buf);
			ABI <T>* dp = (ABI <T>*)buf;
			const T* sp = src.data (), *end = sp + src.size ();
			do {
				MarshalTraits <T>::marshal_out (*(sp++), marshaler, *(dp++));
			} while (sp != end);
		}
	}
}

template <typename T>
void MarshalTraits <Sequence <T> >::unmarshal (SeqABI& src, PlatformUnmarshal_ptr unmarshaler, Seq& dst)
{
	if (src.empty ())
		dst.reset ();
	else {
		SeqABI& adst = static_cast <SeqABI&> (dst);
		if (src.allocated)
			unmarshaler->adopt_memory (adst.ptr = src.ptr, src.allocated);
		else
			adst.ptr = (ABI <T>*)dst.memory ()->copy (nullptr, src.ptr, src.size * sizeof (T), 0);
		if (MarshalTraits <T>::has_unmarshal (unmarshaler->context())) {
			ABI <T>* sp = src.ptr, *end = sp + src.size;
			T* dp = dst.data ();
			try {
				do {
					MarshalTraits <T>::unmarshal (*(sp++), unmarshaler, *(dp++));
				} while (sp != end);
			} catch (...) {
				adst.size = dp - adst.ptr;
				throw;
			}
		}
	}
}

}
}

#endif
