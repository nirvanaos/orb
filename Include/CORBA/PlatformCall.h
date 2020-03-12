/*
~~~{.idl}
module CORBA {
module Nirvana {

pseudo interface PlatformCall
{
  /// Creates PlatformMarshal object.
  PlatformMarshal create_marshaler ();

  void system_exception (long code, RepositoryId id, unsigned long minor, CompletionStatus completed);

  void user_exception (RepositoryId id, ::Nirvana::ConstPointer data, ::Nirvana::Size data_size, PlatformMarshal marshaler);
};

}
}
~~~
*/
#ifndef NIRVANA_ORB_PLATFORMCALL_H_
#define NIRVANA_ORB_PLATFORMCALL_H_

#include "PlatformMarshal.h"

namespace CORBA{
namespace Nirvana {

class PlatformCall;
typedef I_ptr <PlatformCall> PlatformCall_ptr;
typedef I_var <PlatformCall> PlatformCall_var;
typedef I_out <PlatformCall> PlatformCall_out;


BRIDGE_BEGIN (PlatformCall, CORBA_NIRVANA_REPOSITORY_ID ("PlatformCall"))
Interface* (*create_marshaler) (Bridge <PlatformCall>*, EnvironmentBridge*);
void (*system_exception) (Bridge <PlatformMarshal>*, Long, Type <String>::ABI_in, ULong, ABI_enum, EnvironmentBridge*);
void (*user_exception) (Bridge <PlatformMarshal>*, Type <String>::ABI_in, ::Nirvana::ConstPointer, ::Nirvana::Size, Interface*, EnvironmentBridge*);
BRIDGE_END ()


template <class T>
class Client <T, PlatformCall> :
  public T
{
public:
  PlatformMarshal_var create_marshaler ();
  void system_exception (Long code, String_in id, ULong minor, CompletionStatus completed);
  void user_exception (String_in id, ::Nirvana::ConstPointer data, ::Nirvana::Size data_size, I_in <PlatformMarshal> marshaller);
};

template <class T>
PlatformMarshal_var Client <T, PlatformCall>::create_marshaler ()
{
  Environment _env;
  Bridge <PlatformCall>& _b (T::_get_bridge (_env));
  I_ret <PlatformMarshal> _ret = (_b._epv ().epv.create_marshaler) (&_b, &_env);
  _env.check ();
  return _ret;
}

template <class T>
void Client <T, PlatformCall>::system_exception (Long code, String_in id, ULong minor, CompletionStatus completed)
{
  Environment _env;
  Bridge <PlatformCall>& _b (T::_get_bridge (_env));
  (_b._epv ().epv.system_exception) (&_b, code, &id, minor, completed, &_env);
  _env.check ();
}

template <class T>
void Client <T, PlatformCall>::user_exception (String_in id, ::Nirvana::ConstPointer data, ::Nirvana::Size data_size, I_in <PlatformMarshal> marshaler)
{
  Environment _env;
  Bridge <PlatformCall>& _b (T::_get_bridge (_env));
  (_b._epv ().epv.user_exception) (&_b, &id, data, data_size, &marshaler, &_env);
  _env.check ();
}

}
}

#endif
