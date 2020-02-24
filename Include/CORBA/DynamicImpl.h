#ifndef NIRVANA_ORB_DYNAMICIMPL_H_
#define NIRVANA_ORB_DYNAMICIMPL_H_

#include "BasicTypes.h"

namespace CORBA {
namespace Nirvana {

/// Dynamic object implementation
/// Core always calls _release synchronously so we don't need atomic counter here.
template <class T>
class DynamicImpl
{
public:
	DynamicImpl () :
		ref_cnt_ (1)
	{}

	DynamicImpl (const DynamicImpl&) :
		ref_cnt_ (1)
	{}

	DynamicImpl& operator = (const DynamicImpl&)
	{
		return *this; // Do nothing
	}

	void _add_ref ()
	{
		++ref_cnt_;
	}

	void _remove_ref ()
	{
		assert (ref_cnt_);
		if (!--ref_cnt_)
			delete& static_cast <T&> (*this);
	}

	ULong _refcount_value () const
	{
		return ref_cnt_;
	}

private:
	ULong ref_cnt_;
};

}
}

#endif
