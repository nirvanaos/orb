/// Provides ABI for the bool type.
#ifndef NIRVANA_ORB_BOOLEAN_H_
#define NIRVANA_ORB_BOOLEAN_H_

#include <Nirvana/NirvanaBase.h>
#include "Type_forward.h"

namespace CORBA {

typedef bool Boolean;

namespace Nirvana {

/// We can not use `bool' built-in type across the binary boundaries because
/// it is compiler-specific, but we have to achieve the binary compatibility.
/// So we use int as ABI for boolean in assumption that bool implementation can't be wide.
/// Note that Sequence <bool> is implemented as vector <bool> template specialization
/// where element size is 1 byte.
typedef int ABI_boolean;

template <>
struct Type <bool>
{
	typedef ABI_boolean ABI_type;

	static const bool has_check = false;
	void check (ABI_type&) {}

	typedef ABI_type ABI_in;
	typedef ABI_type* ABI_out;
	typedef ABI_type* ABI_inout;
	typedef ABI_type ABI_ret;
	typedef ABI_type ABI_VT_ret;

	class C_in
	{
	public:
		C_in (bool b) :
			b_ (b)
		{}

		ABI_type operator & () const
		{
			return b_;
		}

	private:
		bool b_;
	};

	class C_inout
	{
	public:
		C_inout (bool& b) :
			ref_ (b),
			abi_ (b)
		{}

		~C_inout ()
		{
			ref_ = abi_ != 0;
		}

		ABI_inout operator & ()
		{
			return &abi_;
		}

	protected:
		C_inout (bool& b, ABI_type init) :
			ref_ (b),
			abi_ (init)
		{}

	private:
		bool& ref_;
		ABI_type abi_;
	};

	class C_out : public C_inout
	{
	public:
		C_out (bool& b) :
			C_inout (b, 0)
		{}
	};

	class C_ret
	{
	public:
		C_ret (ABI_type abi) :
			b_ (abi != 0)
		{}

		operator bool () const
		{
			return b_;
		}

	private:
		bool b_;
	};

	typedef C_ret C_VT_ret;

	static bool in (ABI_in abi)
	{
		return abi != 0;
	}

	static bool& inout (ABI_inout p)
	{
		_check_pointer (p);
		return reinterpret_cast <bool&> (*p);
	}

	static bool& out (ABI_out p)
	{
		return inout (p);
	}

	static ABI_ret ret (bool b)
	{
		return b;
	}

	static ABI_VT_ret VT_ret (bool b)
	{
		return b;
	}
};

typedef Type <bool>::C_in Boolean_in;
typedef Type <bool>::C_out Boolean_out;
typedef Type <bool>::C_inout Boolean_inout;

}
}

#endif
