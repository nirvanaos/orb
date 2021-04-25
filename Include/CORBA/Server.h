/// \file
/// CORBA server main header
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
#ifndef NIRVANA_ORB_SERVER_H_
#define NIRVANA_ORB_SERVER_H_

#include "CORBA.h"
#include "Implementation.h"
#include "ImplementationStatic.h"
#include "ImplementationLocal.h"
#include "ImplementationLocalStatic.h"
#include "ImplementationPseudo.h"
#include "ImplementationPseudoStatic.h"
#include "ImplementationPseudoPOA.h"
#include "PortableServer.h"

namespace CORBA {

/// Servant traits.
/// 
/// \tparam I Interface.
template <class I>
struct servant_traits
{
	/// \brief Servant base.
	typedef Nirvana::ServantPOA <I> base_type;

	/// \brief Servant reference.
	typedef servant_reference <Nirvana::ServantPOA <I>> ref_type;

	/// \brief Tied servant.
	/// 
	/// \tparam S Servant class that implement the interface.
	template <class S>
	using tie_type = Nirvana::ServantTied <S, I>;

	// Nirvana extensions

	/// \brief Servant implementation without the virtual methods (Nirvana extension).
	/// 
	/// \tparam S Servant class that implement the interface, derived from Servant <S>.
	template <class S>
	using Servant = Nirvana::Servant <S, I>;

	/// \brief Static servant implementation (Nirvana extension).
	/// 
	/// \tparam S Servant class that implement the interface, derived from Servant <S>.
	template <class S>
	using ServantStatic = Nirvana::ServantStatic <S, I>;
};

}
#endif
