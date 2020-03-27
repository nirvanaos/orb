#ifndef NIRVANA_ORB_INTERFACEMETADATA_H_
#define NIRVANA_ORB_INTERFACEMETADATA_H_

#include "IORequest.h"
#include "Unmarshal.h"
#include <Nirvana/ImportInterface.h>

namespace CORBA {
namespace Nirvana {

/// Function to serve request.
typedef void (*PlatformRequestProc) (Interface* target,
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

struct Operation
{
	const char* name;
	CountedArray <Parameter> input;
	CountedArray <Parameter> output;
	const ::Nirvana::ImportInterfaceT <TypeCode>& return_type;
	PlatformRequestProc invoke;
};

struct InterfaceMetadata
{
	CountedArray <const Char*> base_interfaces;
	CountedArray <Operation> operations;
};

}
}

#endif
