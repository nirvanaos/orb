#ifndef NIRVANA_ORB_TYPE_H_
#define NIRVANA_ORB_TYPE_H_

#include "TypeVarLen.h"
#include "TypeEnum.h"
#include "Type_interface.h"

// Compatibility with modern C11 mapping
namespace IDL {

template <class T>
struct traits
{
	typedef typename CORBA::Nirvana::Type <T>::C_var value_type;
	typedef typename CORBA::Nirvana::Type <T>::C_in in_type;
	typedef typename CORBA::Nirvana::Type <T>::C_out out_type;
	typedef typename CORBA::Nirvana::Type <T>::C_inout inout_type;
};

}

#endif
