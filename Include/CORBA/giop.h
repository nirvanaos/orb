// Nirvana project
// GIOP - General Inter-ORB Protocol

#ifndef NIRVANA_ORB_GIOP_H_
#define NIRVANA_ORB_GIOP_H_

#include "iop.h"

namespace GIOP {

#ifndef GIOP_1_2
  
  // GIOP 1.0 and 1.1
  enum ReplyStatusType_1_0 { // Renamed from ReplyStatusType
    NO_EXCEPTION,
    USER_EXCEPTION,
    SYSTEM_EXCEPTION,
    LOCATION_FORWARD
  };

  // GIOP 1.0
  struct ReplyHeader_1_0 { // Renamed from ReplyHeader
    IOP::ServiceContextList service_context;
    unsigned long request_id;
    ReplyStatusType_1_0 reply_status;
  };

  // GIOP 1.1
  typedef ReplyHeader_1_0 ReplyHeader_1_1;
  // Same Header contents for 1.0 and 1.1

#else

  // GIOP 1.2
  enum ReplyStatusType_1_2 {
    NO_EXCEPTION,
    USER_EXCEPTION,
    SYSTEM_EXCEPTION,
    LOCATION_FORWARD,
    LOCATION_FORWARD_PERM,// new value for 1.2
    NEEDS_ADDRESSING_MODE // new value for 1.2
  };

  struct ReplyHeader_1_2 {
    unsigned long request_id;
    ReplyStatusType_1_2 reply_status;
    IOP::ServiceContextList service_context;
  };

#endif // GIOP_1_2

} // namespace GIOP

#endif // _GIOP_H_
