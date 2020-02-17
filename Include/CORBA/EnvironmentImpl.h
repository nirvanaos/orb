#ifndef NIRVANA_ORB_ENVIRONMENTIMPL_H_
#define NIRVANA_ORB_ENVIRONMENTIMPL_H_

#include "ServantImpl.h"
#include "Environment_s.h"
#include "SystemException.h"

namespace CORBA {
namespace Nirvana {

class EnvironmentBase :
	public Bridge < ::CORBA::Environment>
{
public:
	void exception_set (Long code, const char* rep_id, const void* param,
		const ExceptionEntry* user_exceptions = 0);

	const Char* exception_id () const;

	const void* exception_value () const;

	void exception_free ();

	Exception* exception () const
	{
		if (data_.is_small)
			return (Exception*)(data_.small);
		else
			return data_.ptr;
	}

	void exception (Exception* ex);
	void set (const Exception* ex);

	void check () const;

	EnvironmentBase& operator = (const EnvironmentBase& src)
	{
		set (src.exception ());
		return *this;
	}

protected:
	EnvironmentBase (const EPV& epv) :
		Bridge < ::CORBA::Environment> (epv)
	{
		data_.reset ();
	}

	~EnvironmentBase ()
	{
		if (data_.is_small || data_.ptr)
			exception_free ();
	}

	void move_from (EnvironmentBase& src) NIRVANA_NOEXCEPT
	{
		exception_free ();
		data_ = src.data_;
		src.data_.reset ();
	}

private:
	void set (TypeCode_ptr tc, const void* data);

private:
	union Data
	{
		int small [(sizeof (SystemException) + sizeof (int) - 1) / sizeof (int)];
		struct
		{
			uintptr_t is_small;
			Exception* ptr;
		};

		void reset ()
		{
			is_small = 0;
			ptr = 0;
		}
	} data_;
};

template <class S>
class EnvironmentImpl :
	public EnvironmentBase,
	public Skeleton <S, ::CORBA::Environment>,
	public ServantTraits <S>
{
protected:
	EnvironmentImpl () :
		EnvironmentBase (Skeleton <S, ::CORBA::Environment>::epv_)
	{}
};

}
}

#endif
