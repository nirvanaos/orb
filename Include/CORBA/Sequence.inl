/// \file
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
#ifndef NIRVANA_ORB_SEQUENCE_INL_
#define NIRVANA_ORB_SEQUENCE_INL_
#pragma once

#include "Sequence.h"
#include "IORequestClient.h"

namespace CORBA {
namespace Internal {

template <typename T>
void Type <Sequence <T> >::marshal_in (const Var& src, IORequest_ptr rq)
{
	typedef typename Type <T>::Var T_Var;
	if (Type <T>::is_CDR)
		rq->marshal_seq (alignof (T_Var), sizeof (T_Var), src.size (), const_cast <T_Var*> (src.data ()), 0);
	else {
		rq->marshal_seq_begin (src.size ());
		Type <T>::marshal_in_a (src.data (), src.size (), rq);
	}
}

template <typename T>
void Type <Sequence <T> >::marshal_out (Var& src, IORequest_ptr rq)
{
	typedef typename Type <T>::Var T_Var;
	if (Type <T>::is_CDR) {
		rq->marshal_seq (alignof (T_Var), sizeof (T_Var), src.size (), src.data (), static_cast <ABI&> (src).allocated);
		static_cast <ABI&> (src).reset ();
	} else {
		rq->marshal_seq_begin (src.size ());
		Type <T>::marshal_out_a (src.data (), src.size (), rq);
	}
}

template <typename T>
void Type <Sequence <T> >::unmarshal (IORequest_ptr rq, Var& dst)
{
	typedef typename Type <T>::Var T_Var;
	typedef typename Type <T>::ABI T_ABI;
	Var tmp;
	if (Type <T>::is_CDR) {
		ABI abi;
		bool swap_bytes = rq->unmarshal_seq (alignof (T_Var), sizeof (T_Var), abi.size, (void*&)abi.ptr, abi.allocated);
		if (abi.size) {
			assert (abi.allocated);

#ifdef _DEBUG // In the release we trust the IORequest implementation
			check_ABI (abi);
#endif

			if (sizeof (T_Var) > 1 && swap_bytes) {
				T_Var* p = abi.ptr, *end = p + abi.size;
				do {
					Type <T>::byteswap (*p);
				} while (end != ++p);
			}

			if (Type <T>::has_check) {
				try {
					T_ABI* p = (T_ABI*)abi.ptr, * end = p + abi.size;
					do {
						Type <T>::check (*p);
					} while (end != ++p);
				} catch (...) {
					Nirvana::g_memory->release (abi.ptr, abi.allocated);
					throw;
				}
			}

			static_cast <ABI&> (tmp)= abi;
		}

	} else {

		size_t size = rq->unmarshal_seq_begin ();
		if (size) {
			size_t cb = sizeof (T_Var) * size;
			T_Var* p = (T_Var*)Nirvana::g_memory->allocate (nullptr, cb, 0);
			try {
				Type <T>::unmarshal_a (rq, size, p);
			} catch (...) {
				Nirvana::g_memory->release (p, cb);
				throw;
			}
			static_cast <ABI&> (tmp).ptr = p;
			static_cast <ABI&> (tmp).size = size;
			static_cast <ABI&> (tmp).allocated = cb;
		}
	}
	dst = std::move (tmp);
}

template <> inline
void Type <Sequence <Char> >::marshal_in (const Var& src, IORequest_ptr rq)
{
	rq->marshal_char_seq (const_cast <Var&> (src), false);
}

template <> inline
void Type <Sequence <Char> >::marshal_out (Var& src, IORequest_ptr rq)
{
	rq->marshal_char_seq (const_cast <Var&> (src), true);
}

template <> inline
void Type <Sequence <Char> >::unmarshal (IORequest_ptr rq, Var& dst)
{
	rq->unmarshal_char_seq (dst);
}

template <> inline
void Type <Sequence <WChar> >::marshal_in (const Var& src, IORequest_ptr rq)
{
	rq->marshal_wchar_seq (const_cast <Var&> (src), false);
}

template <> inline
void Type <Sequence <WChar> >::marshal_out (Var& src, IORequest_ptr rq)
{
	rq->marshal_wchar_seq (const_cast <Var&> (src), true);
}

template <> inline
void Type <Sequence <WChar> >::unmarshal (IORequest_ptr rq, Var& dst)
{
	rq->unmarshal_wchar_seq (dst);
}

}
}

#endif
