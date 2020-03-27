#ifndef NIRVANA_ORB_INTERFACEMETADATA_H_
#define NIRVANA_ORB_INTERFACEMETADATA_H_

#include "PlatformRequest.h"
#include "PlatformUnmarshal.h"
#include <Nirvana/ImportInterface.h>

namespace CORBA {
namespace Nirvana {

// TODO: Define.
class RemoteRequest;
typedef RemoteRequest* RemoteRequest_ptr;

/// Function to serve request from the same platform domain.
typedef void (*PlatformRequestProc) (Interface* target,
	PlatformRequest_ptr call,
	::Nirvana::ConstPointer* in_params,
	PlatformUnmarshal_var unmarshaler, // Unmarshaler should be released after the unmarshal completion.
	::Nirvana::Pointer* out_params);

/// Function to serve remote request or request from other platform domain.
typedef void (*RemoteRequestProc) (Interface* target, RemoteRequest* call);

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
	PlatformRequestProc invoke_platform;

	/// May be null. In this case request will be converted into platform request.
	RemoteRequestProc invoke_remote;
};

struct InterfaceMetadata
{
	CountedArray <const Char*> base_interfaces;
	CountedArray <Operation> operations;
};

}
}

#endif
