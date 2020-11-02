#ifndef NIRVANA_ORB_EXCEPTIONS_H_
#define NIRVANA_ORB_EXCEPTIONS_H_

#include "SystemException.h"
#include <Nirvana/ImportInterface.h>

#define DECLARE_SYSTEM_EXCEPTION(E) \
extern const ::Nirvana::ImportInterfaceT <::CORBA::TypeCode> _tc_##E;\
class E : public SystemException {\
DECLARE_EXCEPTION(E)\
E (ULong minor, CompletionStatus status = COMPLETED_NO) : SystemException (minor, status) {}\
virtual Code __code () const NIRVANA_NOEXCEPT { return EC_##E; }};

namespace CORBA {

SYSTEM_EXCEPTIONS (DECLARE_SYSTEM_EXCEPTION)

}

#undef DECLARE_SYSTEM_EXCEPTION

#endif
