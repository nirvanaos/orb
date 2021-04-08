/// \file MarshalTraits_sequence.h
/// \brief Declares the MarshalTraits for sequences.
/*
* Nirvana IDL support library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2021 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/
#ifndef NIRVANA_ORB_MARSHALTRAITS_SEQUENCE_H_
#define NIRVANA_ORB_MARSHALTRAITS_SEQUENCE_H_

#include "MarshalTraits_forward.h"
#include "Sequence.h"
#include "Proxy/Marshal.h"
#include "Proxy/Unmarshal.h"

namespace CORBA {
namespace Nirvana {

template <typename T>
struct MarshalTraits <Vector <T> >
{
	static const bool has_marshal = true;

	typedef Vector <T> Seq;
	typedef ABI <Seq> SeqABI;
	typedef typename Type <T>::ABI_type T_ABI;

	static void marshal_in (const Seq& src, Marshal_ptr marshaler, SeqABI& dst);
	static void marshal_out (Seq& src, Marshal_ptr marshaler, SeqABI& dst);
	static void unmarshal (const SeqABI& src, Unmarshal_ptr unmarshaler, Seq& dst);
};

template <typename T>
void MarshalTraits <Vector <T> >::marshal_in (const Seq& src, Marshal_ptr marshaler, SeqABI& dst)
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
void MarshalTraits <Vector <T> >::marshal_out (Seq& src, Marshal_ptr marshaler, SeqABI& dst)
{
	assert (&src != &dst);
	if (src.empty ())
		dst.reset ();
	else {
		SeqABI& asrc = static_cast <SeqABI&> (src);
		dst.size = asrc.size;
		if (!MarshalTraits <T>::has_marshal) {
			size_t cb = asrc.size * sizeof (T);
			dst.ptr = (T_ABI*)marshaler->marshal_memory (asrc.ptr, cb, asrc.allocated);
			dst.allocated = cb;
			asrc.reset ();
		} else {
			T* sp = src.data (), * end = sp + src.size ();
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
				dst.ptr = (T_ABI*)marshaler->marshal_memory (asrc.ptr, cb, asrc.allocated);
			else {
				Seq tmp;
				src.swap (tmp);
			}

			dst.allocated = cb;
		}
	}
}

template <typename T>
void MarshalTraits <Vector <T> >::unmarshal (const SeqABI& src, Unmarshal_ptr unmarshaler, Seq& dst)
{
	if (Type <Seq>::has_check)
		Type <Seq>::check (src);

	if (!src.size)
		dst.reset ();
	else {
		Seq tmp;
		SeqABI& adst = static_cast <SeqABI&> (tmp);
		if (src.allocated) {
			unmarshaler->adopt_memory (src.ptr, src.allocated);
			adst.ptr = src.ptr;
			adst.allocated = src.allocated;
		} else
			adst.ptr = (T*)::Nirvana::MemoryHelper::assign (0, adst.allocated, 0, src.size * sizeof (T), src.ptr);
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
		adst.size = src.size;
		tmp.swap (dst);
	}
}

}
}

#endif
