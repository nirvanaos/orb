#pragma once
#include "BasicTypes.h"

#ifdef _MSC_VER
#include <intrin.h>
#else
#include <atomic>
#endif

namespace CORBA {

namespace Nirvana {

class RefCount
{
public:
	RefCount () :
		m_refcnt (1)
	{}

	operator ULong () const
	{
		return m_refcnt;
	}

#ifdef _MSC_VER

	void increment ()
	{
		_InterlockedIncrement ((long*)&m_refcnt);
	}

	ULong decrement ()
	{
		return _InterlockedDecrement ((long*)&m_refcnt);
	}

private:
	ULong m_refcnt;

#else

	void increment ()
	{
		++m_refcnt;
	}

	ULong decrement ()
	{
		return --m_refcnt;
	}

private:
	std::atomic <ULong> m_refcnt;
#endif
};

class RefCountBase
{
public:
	void _add_ref ()
	{
		m_ref_count.increment ();
	}

	void _remove_ref ()
	{
		if (!m_ref_count.decrement ())
			delete this;
	}

	ULong _refcount_value () const
	{
		return m_ref_count;
	}

protected:
	RefCountBase ()
	{}

	virtual ~RefCountBase ()
	{}

private:
	RefCount m_ref_count;
};

}
}
