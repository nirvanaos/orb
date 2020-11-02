#ifndef NIRVANA_ORB_TYPECODEENUM_H_
#define NIRVANA_ORB_TYPECODEENUM_H_

#include "TypeCodeImpl.h"

namespace CORBA {
namespace Nirvana {

template <class E>
class TypeCodeEnum :
	public TypeCodeStatic <TypeCodeEnum <E>, TypeCodeWithId <tk_enum, RepIdOf <E> >, TypeCodeOps <E> >,
	public TypeCodeMemberCount <Type <E>::count_>
{
public:
	using TypeCodeMemberCount <Type <E>::count_>::_member_count;

	static const char* _name (Bridge <TypeCode>* _b, Interface* _env)
	{
		return name_;
	}

	static const char* _member_name (Bridge <TypeCode>* _b, ULong index, Interface* _env)
	{
		if (index >= Type <E>::count_) {
			TypeCodeBase::set_Bounds (_env);
			return nullptr;
		} else
			return members_ [index];
	}

private:
	static const Char* const members_ [Type <E>::count_];
	static const Char name_ [];
};

}
}

#endif
