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
#pragma once

#include "CORBA.h"
#include "Implementation.h"
#include "ImplementationStatic.h"
#include "ImplementationLocal.h"
#include "ImplementationLocalStatic.h"
#include "ImplementationTied.h"
#include "ServantBasePOA.h"
#include "LocalObjectPOA.h"
#include "PseudoBase_s.h"
#include "ValueFactoryBase_s.h"
#include "make_stateless.h"
#include "make_pseudo.h"
#include "TraitsServant.h"
#include <CORBA/AMI_s.h>
#include "ValueRefCountBase.h"
#include "Receptacle.h"
#include "CCMObjectImpl.h"
#include "Facet.h"

#ifdef LEGACY_CORBA_CPP
#include "Servant_var.h"
#endif

#endif
