/// \file InterfaceMetadata.h
#ifndef NIRVANA_ORB_INTERFACEMETADATA_H_
#define NIRVANA_ORB_INTERFACEMETADATA_H_

#include "IORequest.h"
#include "Unmarshal.h"
#include <Nirvana/ImportInterface.h>

namespace CORBA {
namespace Nirvana {

/// \brief Function to serve request.
/// \param servant               Interface to servant implementation.
///                              This interface haven't to be checked. The Proxy Manager
///                              guarantees that it is compatible with the proxy primary interface.
/// \param call                  IORequest object.
/// \param in_params             Pointer to the input parameters structure.
///                              The order of values in the structure must correspond to order
///                              of the `input` parameters in the operation metadata.
/// \param [in, out] unmarshaler Unmarshaler should be released after the unmarshal completion.
/// \param [out] out_params      Pointer to the output parameters structure.
///                              The order of values in the structure must correspond to order
///                              of the `output` parameters in the operation metadata.
///                              The return value, if it is not void, must be at the end of structure.
typedef void (*RequestProc) (Interface* servant, Interface* call,
	::Nirvana::ConstPointer in_params,
	Interface** unmarshaler,
	::Nirvana::Pointer out_params);

template <class I, void (*proc) (I_ptr <I>, IORequest_ptr, ::Nirvana::ConstPointer, Unmarshal_var&, ::Nirvana::Pointer)>
void RqProcWrapper (Interface* servant, Interface* call,
	::Nirvana::ConstPointer in_params,
	Interface** unmarshaler,
	::Nirvana::Pointer out_params)
{
	try {
		IORequest_ptr rq = IORequest::_check (call);
		try {
			proc (&static_cast <I&> (*servant), rq, in_params, TypeI <Unmarshal>::inout (unmarshaler), out_params);
			rq->success ();
		} catch (Exception & e) {
			rq->exception (std::move (e));
		}
	} catch (...) {
	}
}

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
