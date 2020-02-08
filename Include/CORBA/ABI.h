#ifndef NIRVANA_ORB_ABI_H_
#define NIRVANA_ORB_ABI_H_

#include <type_traits>
#include <new>
#include <utility>

namespace CORBA {
namespace Nirvana {

void _check_pointer (const void* p);
bool uncaught_exception ();

template <class T, class = void> struct ABI;

/// Base for fixed length data types ABI
template <class T>
struct FixedABI
{
	static const bool is_fixed = true;

	typedef T* ABI_out;
	typedef T* ABI_inout;
	typedef T ABI_ret;

	typedef T& C_out;
	typedef T& C_inout;
	typedef T C_ret;

	static T& inout (T* p)
	{
		_check_pointer (p);
		return *p;
	}

	static T& out (T* p)
	{
		return inout (p);
	}
};

/// ABI for fundamental data types
template <class T>
struct ABI <T, typename std::enable_if <std::is_fundamental <T>::value>::type> :
	public FixedABI <T>
{
	typedef T ABI_in;
//	typedef T* ABI_out;
//	typedef T* ABI_inout;

	typedef T C_in;
//	typedef T& C_out;
//	typedef T& C_inout;
//	typedef T C_ret;

	static T in (T v)
	{
		return v;
	}
};

/// ABI for fixed length data types
template <class T>
struct ABI <T, typename std::enable_if <std::is_trivially_copyable <T>::value && !std::is_fundamental <T>::value>::type> :
	public FixedABI <T>
{
	typedef const T* ABI_in;
//	typedef T* ABI_out;
//	typedef T* ABI_inout;

	typedef const T& C_in;

	static const T& in (const T* p)
	{
		_check_pointer (p);
		return *p;
	}
};

/// Base for variable length data types ABI
template <class T>
struct VariableABI
{
	static const bool is_fixed = false;

	typedef const T* ABI_in;
	typedef T* ABI_out;
	typedef T* ABI_inout;
	typedef T ABI_ret;

	typedef const T& C_in;

	class C_inout
	{
	public:
		C_inout (T& val) :
			ref_ (val)
		{}

		operator T* () const
		{
			return &ref_;
		}

		~C_inout () noexcept (false);

	private:
		T& ref_;
	};

	class C_out : public C_inout
	{
	public:
		C_out (T& val) :
			C_inout (val)
		{
			val = T ();	// Clear
		}
	};

	class C_ret
	{
	public:
		C_ret (T&& val) :
			val_ (std::move (val))
		{
			check_or_clear (val_);
		}

		operator T ()
		{
			return std::move (val_);
		}

	private:
		T val_;
	};

	static const T& in (const T* p)
	{
		_check_pointer (p);
		ABI <T>::check (*p);
		return *p;
	}

	static T& inout (T* p)
	{
		_check_pointer (p);
		ABI <T>::check (*p);
		return *p;
	}

	static T& out (T* p)
	{
		return inout (p);
	}

	static void check_or_clear (T& v);
};

/// Outline for compact code
template <class T>
void VariableABI <T>::check_or_clear (T& v)
{
	try {
		ABI <T>::check (v);
	} catch (...) {
		v.~T ();
		new (&v) T ();
		throw;
	}
}

/// Outline for compact code
template <class T>
VariableABI <T>::C_inout::~C_inout () noexcept (false)
{
	bool ex = uncaught_exception ();
	try {
		ABI <T>::check_or_clear (ref_);
	} catch (...) {
		if (!ex)
			throw;
	}
}

/*
template <class T, class Enable = void> class C_in;

template <class T, typename std::enable_if <std::is_fundamental <T>::value>::type>
class C_in
{
public:
	C_in (const T& val) :
		val_ (val)
	{}

	operator T () const
	{
		return val_;
	}

private:
	const T val_;
};

template <class T>
class C_in
{
public:
	C_in (const T& val) :
		ref_ (val)
	{}

	operator const T* () const
	{
		return &ref_;
	}

private:
	const T& ref_;
};

template <class T>
class C_inout
{
public:
	C_inout (T& val) :
		ref_ (val)
	{}

	operator T* () const
	{
		return &ref_;
	}

	~C_inout () noexcept (false);

private:
	T& ref_;
};

template <class T>
C_inout <T>::~C_inout () noexcept (false)
{
	if (!ABI <T>::is_fixed) {
		bool ex = uncaught_exception ();
		try {
			ABI <T>::check_or_clear (ref_);
		} catch (...) {
			if (!ex)
				throw;
		}
	}
}

template <class T>
class C_out : public C_inout <T>
{
public:
	C_out (T& val) :
		ref_ (val)
	{
		val = T ();
	}
};


template <class T>
class C_ret : public T
{
public:
	C_ret (T&& val)
		T (std::move (val))
	{
		ABI <T>::check_or_clear (*this);
	}

	T _retn ()
	{
		return std::move (*this);
	}
};
*/
}
}
#endif
