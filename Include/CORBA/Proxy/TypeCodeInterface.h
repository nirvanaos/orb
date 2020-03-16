#ifndef NIRVANA_ORB_TYPECODEINTERFACE_H_
#define NIRVANA_ORB_TYPECODEINTERFACE_H_

#include "TypeCodeImpl.h"

namespace CORBA {
namespace Nirvana {

template <class S, class I>
class TypeCodeInterface :
{
public:
};

}
}

#define DEFINE_INTERFACE_TC(ns, I) template <> const Char TypeCodeInterface <ns::I>::name_ [] = #I;

#endif
