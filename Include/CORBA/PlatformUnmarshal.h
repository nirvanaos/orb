/*
~~~{.idl}
module CORBA {
module Nirvana {

/// Unmarshals data between objects in the same platform domain.
pseudo interface PlatformUnmarshal
{
  /// Returns marshal context.
  /// Used for proxy code optimization.
  attribute readonly PlatformMarshalContext context;

  void adopt_memory (::Nirvana::Pointer p, ::Nirvana::Size size);
  Interface unmarshal_interface (::Nirvana::ConstPointer marshal_data, RepositoryId iid);
  TypeCode unmarshal_type_code (::Nirvana::ConstPointer marshal_data);
};

}
}
~~~
*/
#ifndef NIRVANA_ORB_PLATFORMUNMARSHAL_H_
#define NIRVANA_ORB_PLATFORMUNMARSHAL_H_

#include "Object.h"
#include "TypeCode_b.h"
#include "PlatformMarshalContext.h"

namespace CORBA {

class Object;

namespace Nirvana {

class PlatformUnmarshal;
typedef I_ptr <PlatformUnmarshal> PlatformUnmarshal_ptr;
typedef I_var <PlatformUnmarshal> PlatformUnmarshal_var;
typedef I_out <PlatformUnmarshal> PlatformUnmarshal_out;

/// \interface PlatformUnmarshal
template <class T>
class Client <T, PlatformUnmarshal> :
  public T
{
public:
  PlatformMarshalContext context ();

  void adopt_memory (void* p, size_t size);
  Interface_var unmarshal_interface (const void* marshal_data, const Char* interface_id);
  TypeCode_var unmarshal_type_code (const void* marshal_data);
};

BRIDGE_BEGIN (PlatformUnmarshal, CORBA_NIRVANA_REPOSITORY_ID ("PlatformUnmarshal"))
void (*adopt_memory) (Bridge <PlatformUnmarshal>*, void* p, size_t size, EnvironmentBridge*);
Interface* (*unmarshal_interface) (Bridge <PlatformUnmarshal>*, const void*, const Char*, EnvironmentBridge*);
BRIDGE_END ()

template <class T>
void Client <T, PlatformUnmarshal>::adopt_memory (void* p, size_t size)
{
  Environment _env;
  Bridge <PlatformUnmarshal>& _b (T::_get_bridge (_env));
  (_b._epv ().epv.adopt_memory) (&_b, p, size, &_env);
  _env.check ();
}

template <class T>
Interface_var Client <T, PlatformUnmarshal>::unmarshal_interface (const void* marshal_data, const Char* interface_id)
{
  Environment _env;
  Bridge <PlatformUnmarshal>& _b (T::_get_bridge (_env));
  I_ret <Interface> _ret = (_b._epv ().epv.unmarshal_interface) (&_b, marshal_data, interface_id, &_env);
  _env.check ();
  return _ret;
}

template <class T>
TypeCode_var Client <T, PlatformUnmarshal>::unmarshal_type_code (const void* marshal_data)
{
  Environment _env;
  Bridge <PlatformUnmarshal>& _b (T::_get_bridge (_env));
  I_ret <TypeCode> _ret = (_b._epv ().epv.unmarshal_type_code) (&_b, marshal_data, &_env);
  _env.check ();
  return _ret;
}

class PlatformUnmarshal : public ClientInterface <PlatformUnmarshal>
{};

}
}

#endif
