/// \file InterfaceMetadata.h
/*
* Nirvana IDL support library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
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
#ifndef NIRVANA_ORB_INTERFACEMETADATA_H_
#define NIRVANA_ORB_INTERFACEMETADATA_H_

#include "../IORequestClient.h"
#include <Nirvana/ImportInterface.h>

namespace CORBA {
namespace Internal {

/// \brief Function to serve request.
/// \param servant Interface to servant implementation.
///                This interface haven't to be checked. The Proxy Manager
///                guarantees that it is compatible with the proxy primary interface.
/// \param call    IORequest object.
/// 
/// \returns       `true` on success, `false` if unexpected unhandled error occured (rarely).
typedef bool (*RequestProc) (Interface* servant, Interface* call);

/// Counted array for metadata.
template <class T>
struct CountedArray
{
	const T* p;
	size_t size;
};

/// Function to return TypeCode
typedef I_ptr <TypeCode> (*GetTypeCode) ();

/// Parameter metadata.
struct Parameter
{
	const Char* name;
	const GetTypeCode type;
};

/// Operation metadata.
struct Operation
{
	const Char* name;
	CountedArray <Parameter> input;
	CountedArray <Parameter> output;
	GetTypeCode return_type;
	RequestProc invoke;
};

/// Interface metadata.
struct InterfaceMetadata
{
	/// List of all interfaces. Primary interface must be first.
	CountedArray <const Char*> interfaces;

	/// List of all operations for the primary interface.
	CountedArray <Operation> operations;
};

// native InterfaceMetadataPtr;
typedef const InterfaceMetadata* InterfaceMetadataPtr;

template <>
struct Type <InterfaceMetadataPtr> : TypeByVal <InterfaceMetadataPtr>
{};

// native InterfacePtr;
typedef Interface* InterfacePtr;

template <>
struct Type <InterfacePtr> : TypeByVal <InterfacePtr>
{};

}
}

#endif
