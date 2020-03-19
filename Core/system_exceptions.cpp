#include <CORBA/Proxy/TypeCodeException.h>

namespace CORBA {
namespace Nirvana {

class SystemExceptionMembers
{
protected:
	static const Parameter members_ [];
};

template <class E>
class TypeCodeExceptionSystem :
	public TypeCodeWithMembersImpl <2, TypeCodeExceptionRoot <E>, SystemExceptionMembers>
{};

const Parameter SystemExceptionMembers::members_ [] = {

};

}
}
