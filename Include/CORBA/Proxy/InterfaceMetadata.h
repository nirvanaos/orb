/// \file
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

/// Parameter metadata.
struct Parameter
{
	const Char* name;
	const GetTypeCode type;
};

typedef Nirvana::ImportInterfaceT <TypeCode> TypeCodeImport;

/// Operation metadata.
struct Operation
{
	/// The operation name. `nullptr` for local objects.
	const Char* name;

	/// in and inout parameters. Empty for local objects.
	CountedArray <Parameter> input;

	/// out and inout parameters. Empty for local objects.
	CountedArray <Parameter> output;

	/// User exception type codes. Empty for local objects.
	CountedArray <const TypeCodeImport*> user_exceptions;

	/// Context
	CountedArray <const Char*> context;

	/// Operation return type. `nullptr` for `void` operations and local objects.
	GetTypeCode return_type;

	/// Invoke request procedure.
	RequestProc invoke;

	/// Operation flags.
	uint16_t flags;

	/// Operation receives one or more parameters with complex unmarshaling:
	/// object refrence, value type, type code or any.
	static const uint16_t FLAG_IN_CPLX = 1;

	/// Operation returns one or more parameters with complex unmarshaling:
	/// object refrence, value type, type code or any.
	static const uint16_t FLAG_OUT_CPLX = 2;
};

/// Interface metadata.
struct InterfaceMetadata
{
	/// List of all interfaces. Primary interface must be first.
	CountedArray <const Char*> interfaces;

	/// List of all operations for the primary interface.
	CountedArray <Operation> operations;

	/// The interface flags.
	uint16_t flags;

	/// Local interface.
	static const uint16_t FLAG_LOCAL = 1;

	/// Stateless servant.
	static const uint16_t FLAG_STATELESS = 2;

	/// Local stateless servant.
	static const uint16_t FLAG_LOCAL_STATELESS = FLAG_LOCAL | FLAG_STATELESS;

	/// AMI poller repository id.
	const Char* poller_id;

	/// AMI handler repository id.
	const Char* handler_id;
};

/// native InterfaceMetadataPtr;
typedef const InterfaceMetadata* InterfaceMetadataPtr;

template <class I>
struct MetadataOf
{
	static const InterfaceMetadata metadata_;
};

}
}

#endif
