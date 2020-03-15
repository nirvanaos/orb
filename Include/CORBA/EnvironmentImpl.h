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
	void exception_set (Long code, String_in rep_id, const void* param,
		const ExceptionEntry* user_exceptions = 0);

	const Char* exception_id () const;

	const void* exception_value () const;

	void exception_free ();

	void clear ()
	{
		exception_free ();
	}

	Exception* exception () const
	{
		if (data_.is_small)
			return (Exception*)(data_.small);
		else
			return data_.ptr;
	}

	void exception (Exception* ex);

	void check () const;

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

	void move_from (EnvironmentBase& src) NIRVANA_NOEXCEPT;

private:
	bool set (const ExceptionEntry& ee);
	void set_system (const ExceptionEntry& ee, const void* data);
	void set_user (const ExceptionEntry& ee, const void* data);
	bool set_user (String_in rep_id, const void* param, const ExceptionEntry* user_exceptions);

private:
	union Data
	{
		int small [(sizeof (SystemException) + sizeof (int) - 1) / sizeof (int)];
		struct
		{
			// If small data contains Exception object, the first word of it
			// contains pointer to the virtual table and can't be zero.
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
public:
	template <class I>
	static Interface* __duplicate (Interface* itf, EnvironmentBridge* env)
	{
		set_NO_IMPLEMENT (env);
		return 0;
	}

	template <class I>
	static void __release (Interface* itf)
	{}

protected:
	EnvironmentImpl () :
		EnvironmentBase (Skeleton <S, ::CORBA::Environment>::epv_)
	{}
};

}
}

#endif
