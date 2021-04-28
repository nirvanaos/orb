/// \file Unmarshal_s.h
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
#ifndef NIRVANA_ORB_UNMARSHAL_S_H_
#define NIRVANA_ORB_UNMARSHAL_S_H_

#include "Unmarshal.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, Unmarshal>
{
public:
	static const typename Bridge <Unmarshal>::EPV epv_;

protected:
	static ABI_enum _context (Bridge <Unmarshal>* _b, Interface* _env)
	{
		try {
			return Type <MarshalContext>::ret (S::_implementation (_b).context ());
		} catch (Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static void _adopt_memory (Bridge <Unmarshal>* _b, ::Nirvana::ConstPointer p, 
		::Nirvana::Size size, Interface* _env)
	{
		try {
			S::_implementation (_b).adopt_memory (p, size);
		} catch (Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static Interface* _unmarshal_interface (Bridge <Unmarshal>* _b, ::Nirvana::ConstPointer data,
		Type <String>::ABI_in iid, Interface* _env)
	{
		try {
			return S::_implementation (_b).unmarshal_interface (data, Type <String>::in (iid));
		} catch (Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return nullptr;
	}
};

template <class S>
const Bridge <Unmarshal>::EPV Skeleton <S, Unmarshal>::epv_ = {
	{ // header
		Bridge <Unmarshal>::repository_id_,
		S::template __duplicate <Unmarshal>,
		S::template __release <Unmarshal>
	},
	{ // epv
		S::_context,
		S::_adopt_memory,
		S::_unmarshal_interface
	}
};

}
}

#endif
