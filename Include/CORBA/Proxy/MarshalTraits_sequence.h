#ifndef NIRVANA_ORB_MARSHALTRAITS_SEQUENCE_H_
#define NIRVANA_ORB_MARSHALTRAITS_SEQUENCE_H_

#include "MarshalTraits_forward.h"
#include "../sequence.h"
#include "Marshal.h"
#include "Unmarshal.h"

namespace CORBA {
namespace Nirvana {

template <typename T>
struct MarshalTraits <Sequence <T> >
{
	static const bool has_marshal = true;

	typedef Sequence <T> Seq;
	typedef ABI <Sequence <T> > SeqABI;
	typedef typename Type <T>::ABI_type T_ABI;

	static void marshal_in (const Seq& src, Marshal_ptr marshaler, SeqABI& dst);
	static void marshal_out (Seq& src, Marshal_ptr marshaler, SeqABI& dst);
	static void unmarshal (const SeqABI& src, Unmarshal_ptr unmarshaler, Seq& dst);
};

template <typename T>
void MarshalTraits <Sequence <T> >::marshal_in (const Seq& src, Marshal_ptr marshaler, SeqABI& dst)
{
	assert (&src != &dst);
	if (src.empty ())
		dst.reset ();
	else {
		const SeqABI& asrc = static_cast <const SeqABI&> (src);
		if (!MarshalTraits <T>::has_marshal) {
			size_t cb = asrc.size * sizeof (T);
			dst.ptr = (T_ABI*)marshaler->marshal_memory (asrc.ptr, cb, 0);
			dst.allocated = cb;
			dst.size = asrc.size;
		} else {
			size_t cb = asrc.size * sizeof (T);
			void* buf;
			dst.ptr = (T_ABI*)marshaler->get_buffer (cb, buf);
			T_ABI* dp = (T_ABI*)buf;
			const T* sp = src.data (), * end = sp + src.size ();
			do {
				MarshalTraits <T>::marshal_in (*(sp++), marshaler, *(dp++));
			} while (sp != end);
		}
	}
}

template <typename T>
void MarshalTraits <Sequence <T> >::marshal_out (Seq& src, Marshal_ptr marshaler, SeqABI& dst)
{
	assert (&src != &dst);
	if (src.empty ())
		dst.reset ();
	else {
		SeqABI& asrc = static_cast <SeqABI&> (src);
		dst.size = asrc.size;
		if (!MarshalTraits <T>::has_marshal) {
			size_t cb = asrc.size * sizeof (T);
			dst.ptr = (T_ABI*)marshaler->marshal_memory (asrc.ptr, cb, asrc.allocated ());
			dst.allocated = cb;
			asrc.reset ();
		} else {
			const T* sp = src.data (), * end = sp + src.size ();
			T_ABI* dp;
			size_t cb;
			MarshalContext mctx = marshaler->context ();
			if (MarshalContext::SHARED_MEMORY == mctx)
				dp = (T_ABI*)sp;
			else {
				cb = asrc.size * sizeof (T);
				void* buf;
				dst.ptr = (T_ABI*)marshaler->get_buffer (cb, buf);
				dp = (T_ABI*)buf;
			}

			do {
				MarshalTraits <T>::marshal_out (*(sp++), marshaler, *(dp++));
			} while (sp != end);

			if (MarshalContext::SHARED_MEMORY == mctx)
				dst.ptr = (T_ABI*)marshaler->marshal_memory (asrc.ptr, cb, asrc.allocated ());
			else {
				Seq tmp;
				src.swap (tmp);
			}

			dst.allocated = cb;
		}
	}
}

template <typename T>
void MarshalTraits <Sequence <T> >::unmarshal (const SeqABI& src, Unmarshal_ptr unmarshaler, Seq& dst)
{
	if (Type <Seq>::has_check)
		Type <Seq>::check (src);

	if (!src.size)
		dst.reset ();
	else {
		Seq tmp;
		SeqABI& adst = static_cast <SeqABI&> (tmp);
		if (src.allocated)
			unmarshaler->adopt_memory (adst.ptr = src.ptr, src.allocated);
		else
			adst.ptr = (T_ABI*)dst.memory ()->copy (nullptr, src.ptr, src.size * sizeof (T), 0);
		if (MarshalTraits <T>::has_marshal) {
			T_ABI* sp = src.ptr, *end = sp + src.size;
			T* dp = adst.ptr;
			try {
				do {
					MarshalTraits <T>::unmarshal (*(sp++), unmarshaler, *(dp++));
				} while (sp != end);
			} catch (...) {
				adst.size = dp - adst.ptr;
				throw;
			}
		}
		tmp.swap (dst);
	}
}

}
}

#endif
