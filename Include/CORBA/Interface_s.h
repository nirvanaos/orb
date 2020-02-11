// TODO: Remove!
#ifndef NIRVANA_ORB_INTERFACE_S_H_
#define NIRVANA_ORB_INTERFACE_S_H_

#include "Interface.h"

namespace CORBA {
namespace Nirvana {

template <class I>
T_ptr <I> _unmarshal_in (BridgeMarshal <I>* bridge)
{
	return T_ptr <I> (I::unmarshal (bridge));
}

template <class I>
T_var <I>& _unmarshal_out (BridgeMarshal <I>** bridge)
{
	_check_pointer (bridge);
	if (*bridge)
		::Nirvana::throw_MARSHAL ();
	return reinterpret_cast <T_var <I>&> (*bridge);
}

template <class I>
T_var <I>& _unmarshal_inout (BridgeMarshal <I>** bridge)
{
	_check_pointer (bridge);
	I::unmarshal (*bridge);
	return reinterpret_cast <T_var <I>&> (*bridge);
}

}
}

#endif
