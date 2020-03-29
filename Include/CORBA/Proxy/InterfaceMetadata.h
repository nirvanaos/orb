/// \file InterfaceMetadata.h
#ifndef NIRVANA_ORB_INTERFACEMETADATA_H_
#define NIRVANA_ORB_INTERFACEMETADATA_H_

#include "IORequest.h"
#include "Unmarshal.h"
#include <Nirvana/ImportInterface.h>

namespace CORBA {
namespace Nirvana {

/// Function to serve request.
typedef void (*RequestProc) (Interface* target,
	IORequest_ptr call,
	::Nirvana::ConstPointer* in_params,
	Unmarshal_var unmarshaler, // Unmarshaler should be released after the unmarshal completion.
	::Nirvana::Pointer* out_params);

/// Counted array for metadata.
template <class T>
struct CountedArray
{
	const T* p;
	ULong size;
};

/// Parameter metadata.
struct Parameter
{
	const Char* name;
	const ::Nirvana::ImportInterfaceT <TypeCode>& type;
};

/// Operation metadata.
struct Operation
{
	const Char* name;
	CountedArray <Parameter> input;
	CountedArray <Parameter> output;
	const ::Nirvana::ImportInterfaceT <TypeCode>& return_type;
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

}
}

#endif
