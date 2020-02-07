#ifndef NIRVANA_ORB_EXCEPTIONS_H_
#define NIRVANA_ORB_EXCEPTIONS_H_

#include "SystemException.h"

#define DECLARE_SYSTEM_EXCEPTION(e) \
extern Nirvana::ExceptionEntry _tc_##e;\
class e : public SystemException {\
public: e () {}\
e (ULong minor, CompletionStatus status = COMPLETED_NO) : SystemException (minor, status) {}\
e (const Data* data) : SystemException (data) {}\
virtual Long __code () const;\
DECLARE_EXCEPTION(e)\
};

namespace CORBA {

SYSTEM_EXCEPTIONS (DECLARE_SYSTEM_EXCEPTION)

}

#undef DECLARE_SYSTEM_EXCEPTION

#endif
