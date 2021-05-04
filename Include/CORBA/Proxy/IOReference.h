/// \file IOReference.h
/*
* Nirvana IDL support library.
*
* This is a part of the Nirvana project.
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

/*
~~~{.idl}
module CORBA {
module Nirvana {

/// High 16 bits contain the interface index.
/// Low 16 bits contain the operation index within the interface.
typedef unsigned long OperationIndex;

/// \brief Interoperable Object Reference.
///        Interface to the implementation of the some Inter-ORB protocol.
abstract valuetype IOReference
{
	/// Returns pointer to the Object implementation.
	readonly attribute Object _object;

	/// Creates Marshal object.
	Marshal create_marshaler ();

	/// Performs a synchronous call.
	/// \param op                Operation index.
	/// \param in_params         Input parameters block's pointer.
	///                          Contains input values for in and inout parameters.
	///                          May be `nil` if there aren't in and inout parameters.
	/// \param in_params_size    Size of the input parameters block.
	/// \param marshaler         `Marshal` object. May be `nil` on input. Always `nil` on output.
	/// \param out_params        Pointer to a memory buffer for output parameters.
	/// \param out_params_size   Size of the output parameters block.
	/// \returns                 `Unmarshal` object for returned data. May be `nil`.
	Unmarshal call (OperationIndex op,
		::Nirvana::ConstPointer in_params, ::Nirvana::Size in_params_size, [inout] Marshal marshaler,
		::Nirvana::Pointer out_params, ::Nirvana::Size out_params_size) raises (UnknownUserException);

	/// Performs an asynchronous call.
	/// \param op              Operation index.
	/// \param in_params       Input parameters block's pointer.
	///                        Contains input values for in and inout parameters.
	///                        May be nil if there aren't in and inout parameters.
	/// \param in_params_size  Size of the input parameters block.
	/// \param marshaler       Marshal object. May be nil.
	/// \param out_params_size Size of the output parameters block.
	/// \param _oneway         TRUE if this is oneway operation.
	/// \returns               AsyncCall object. Nil if _oneway is TRUE.
	AsyncCall async_call (OperationIndex op,
		::Nirvana::ConstPointer in_params, ::Nirvana::Size in_params_size, Marshal marshaler,
		::Nirvana::Size out_params_size, boolean _oneway);
};

}
}
~~~
*/
#ifndef NIRVANA_ORB_IOREFERENCE_H_
#define NIRVANA_ORB_IOREFERENCE_H_

namespace CORBA {
namespace Nirvana {

class Marshal;

class Unmarshal;

typedef ULong OperationIndex;

class IOReference;

template <>
struct Type <IOReference> : TypeItf <IOReference>
{};

NIRVANA_BRIDGE_BEGIN (IOReference, CORBA_NIRVANA_REPOSITORY_ID ("IOReference"))
Interface* (*_get_object) (Bridge <IOReference>*, Interface*);
Interface* (*create_marshaler) (Bridge <IOReference>*, Interface*);
Interface* (*call) (Bridge <IOReference>*, Type <OperationIndex>::ABI_in,
	::Nirvana::ConstPointer, ::Nirvana::Size, Interface**,
	::Nirvana::Pointer, ::Nirvana::Size, Interface*);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, IOReference> :
	public T
{
public:
	TypeItf <Object>::ConstRef object ();
	
	TypeItf <Marshal>::Var create_marshaler ();

	TypeItf <Unmarshal>::Var call (Type <OperationIndex>::C_in op,
		::Nirvana::ConstPointer in_params, ::Nirvana::Size in_params_size, Type <Marshal>::C_inout marshaler,
		::Nirvana::Pointer out_params, ::Nirvana::Size out_params_size);
};

class IOReference : public ClientInterface <IOReference>
{};

template <class T>
TypeItf <Object>::ConstRef Client <T, IOReference>::object ()
{
	Environment _env;
	Bridge <IOReference>& _b (T::_get_bridge (_env));
	TypeItf <Object>::C_VT_ret _ret ((_b._epv ().epv._get_object) (&_b, &_env));
	_env.check ();
	return _ret;
}

template <class T>
TypeItf <Marshal>::Var Client <T, IOReference>::create_marshaler ()
{
	Environment _env;
	Bridge <IOReference>& _b (T::_get_bridge (_env));
	TypeItf <Marshal>::C_ret _ret ((_b._epv ().epv.create_marshaler) (&_b, &_env));
	_env.check ();
	return _ret;
}

template <class T>
TypeItf <Unmarshal>::Var Client <T, IOReference>::call (Type <OperationIndex>::C_in op,
	::Nirvana::ConstPointer in_params, ::Nirvana::Size in_params_size, Type <Marshal>::C_inout marshaler,
	::Nirvana::Pointer out_params, ::Nirvana::Size out_params_size)
{
	EnvironmentEx <UnknownUserException> _env;
	Bridge <IOReference>& _b (T::_get_bridge (_env));
	TypeItf <Unmarshal>::C_ret _ret ((_b._epv ().epv.call) (&_b, &op, in_params, in_params_size, &marshaler,
		out_params, out_params_size, &_env));
	_env.check ();
	return _ret;
}

}
}

#endif
