#ifndef NIRVANA_ORB_ABI_FORWARD_H_
#define NIRVANA_ORB_ABI_FORWARD_H_

namespace CORBA {
namespace Nirvana {

template <class T, class = void> struct ABI;

void _check_pointer (const void* p);
bool uncaught_exception ();

template <class T>
struct ABI_Base
{
	typedef T Var;

	typedef const T* ABI_in;
	typedef T* ABI_out;
	typedef T* ABI_inout;
	typedef Var ABI_ret;

	typedef const T& In;
	typedef T& InOut;
	typedef T& Out;

	static const T& in (ABI_in p)
	{
		_check_pointer (p);
		return *p;
	}

	static T& inout (ABI_inout p)
	{
		_check_pointer (p);
		return *p;
	}

	static T& out (ABI_out p)
	{
		return inout (p);
	}

	inline static Var ret (ABI_ret val)
	{
		return val;
	}

	static void check (const T&)
	{}
};

/// Base for fixed length data types ABI
template <class T>
struct ABI_FixedLen : public ABI_Base <T>
{
	static const bool is_checked = false;
};

/// Base for variable length data types ABI
template <class T>
struct ABI_VariableLen : public ABI_Base <T>
{
	static const bool is_checked = true;

	typedef typename ABI_Base <T>::ABI_in ABI_in;
	typedef typename ABI_Base <T>::ABI_out ABI_out;
	typedef typename ABI_Base <T>::ABI_inout ABI_inout;
	typedef typename ABI_Base <T>::ABI_ret ABI_ret;

	typedef typename ABI_Base <T>::Var Var;

	class InOut
	{
	public:
		InOut (T& val) :
			ref_ (val)
		{}

		~InOut () noexcept (false);

		T* operator & () const
		{
			return &ref_;
		}

	private:
		T& ref_;
	};

	class Out : public InOut
	{
	public:
		Out (T& val) :
			InOut (val)
		{
			val = T ();	// Clear
		}
	};

	static const T& in (ABI_in p)
	{
		_check_pointer (p);
		ABI <T>::check (*p);
		return *p;
	}

	static T& inout (ABI_inout p)
	{
		_check_pointer (p);
		ABI <T>::check (*p);
		return *p;
	}

	static T& out (ABI_out p)
	{
		return inout (p);
	}

	inline static Var ret (ABI_ret val)
	{
		check_or_clear (val);
		return val;
	}

	static void check_or_clear (T& v);
};

}
}

#endif
