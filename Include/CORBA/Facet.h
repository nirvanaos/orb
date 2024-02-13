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
#ifndef NIRVANA_ORB_FACET_H_
#define NIRVANA_ORB_FACET_H_

#include "ServantBaseLink.h"
#include "LocalObjectLink.h"

namespace CORBA {
namespace Internal {

template <class Base, class Parent>
class Facet : public Base
{
public:
	typedef Parent ParentServant;

	Facet (ParentServant& parent) :
		Base (parent._object ()),
		parent_ (parent)
	{}

#ifndef LEGACY_CORBA_CPP
protected:
	template <class> friend class CORBA::Internal::LifeCycleRefCnt;
	template <class> friend class CORBA::servant_reference;
	template <class, class> friend class CORBA::Internal::Skeleton;
	template <class> friend class CORBA::Internal::ServantPOA;
#endif

	void _add_ref ()
	{
		parent_._add_ref ();
	}

	void _remove_ref ()
	{
		parent_._remove_ref ();
	}

private:
	ParentServant& parent_;

};

}

/// \brief Facet servant. Nirvana extension for implementation of the Corba Component Model.
/// Used as child member of component interface servant class.
/// 
/// \tparam Base Facet interface servant base, like servant_traits <...>::base_type.
/// 
template <class Servant>
using Facet = Internal::Facet <Servant, Internal::ServantBaseLink>;

/// \brief Facet servant. Nirvana extension for implementation of the Corba Component Model.
/// Used as child member of component local interface servant class.
/// 
/// \tparam Base Facet interface servant base, like servant_traits <...>::base_type.
/// 
template <class Servant>
using FacetLocal = Internal::Facet <Servant, Internal::LocalObjectLink>;

}

#endif
