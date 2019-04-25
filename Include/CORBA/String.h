#ifndef NIRVANA_ORB_STRING_H_
#define NIRVANA_ORB_STRING_H_

#include "StringManager_c.h"

namespace CORBA {

inline
Char* string_alloc (ULong len)
{
	return Nirvana::StringManager <Char>::singleton ()->string_alloc (len);
}

inline
Char* string_dup (const Char* s)
{
	return Nirvana::StringManager <Char>::singleton ()->string_dup (s);
}

inline
void string_free (Char* s)
{
	Nirvana::StringManager <Char>::singleton ()->string_free (s);
}

inline
WChar* wstring_alloc (ULong len)
{
	return Nirvana::StringManager <WChar>::singleton ()->string_alloc (len);
}

inline
WChar* wstring_dup (const WChar* s)
{
	return Nirvana::StringManager <WChar>::singleton ()->string_dup (s);
}

inline
void wstring_free (WChar* s)
{
	Nirvana::StringManager <WChar>::singleton ()->string_free (s);
}

template <class C> class StringT_out;

template <class C>
class StringT_var
{
public:
	StringT_var () :
		p_ (nullptr)
	{}

	StringT_var (C* p) :
		p_ (p)
	{}

	StringT_var (const C* p) :
		p_ (Nirvana::StringManager <C>::singleton ()->string_dup (p))
	{}

	StringT_var (const String_var &s) :
		p_ (Nirvana::StringManager <C>::singleton ()->string_dup (s.p_))
	{}

	~StringT_var ()
	{
		release ();
	}

	StringT_var &operator = (C *p)
	{
		if (p_ != p) {
			release ();
			p_ = p;
		}
		return *this;
	}

	StringT_var &operator = (const C *p)
	{
		if (p_ != p) {
			clear (); // To be NULL on exception.
			p_ = Nirvana::StringManager <C>::singleton ()->string_dup (p);
		}
		return *this;
	}

	StringT_var& operator = (const StringT_var& s)
	{
		if (&s != this) {
			clear (); // To be NULL on exception.
			p_ = Nirvana::StringManager <C>::singleton ()->string_dup (s.p_);
		}
		return *this;
	}

	operator C*& ()
	{
		clear ();
		return p_;
	}

	operator const C* () const
	{
		return p_;
	}

	const C* operator()
	{
		return p_;
	}

	const C* in () const
	{
		return p_;
	}

	C*& inout ()
	{
		return p_;
	}

	C*& out ()
	{
		clear ();
		return p_;
	}
	
	C* _retn ()
	{
		C* p = p_;
		p_ = nullptr;
		return p;
	}

	C &operator [] (ULong index)
	{
		return Nirvana::StringManager <C>::singleton ()->at (p_, index);
	}

	C operator [] (ULong index) const
	{
		return p_ [index];
	}

private:
	friend class StringT_out <C>;

	void release ()
	{
		if (p_)
			Nirvana::StringManager <C>::singleton ()->string_free (p_);
	}

	void clear ()
	{
		release ();
		p_ = nullptr;
	}

private:
	C* p_;
};

template <class C>
class StringT_out
{
public:
	StringT_out (C*& p) :
		p_ (p)
	{}

	StringT_out (StringT_var <C>& s) :
		p_ (s.p_)
	{
		s.clear ();
	}

	StringT_out (const StringT_out& s) :
		p_ (s.p_)
	{}

	StringT_out& operator = (const StringT_out& s)
	{
		p_ = s.p_;
		return *this;
	}

	StringT_out& operator = (C* p)
	{
		p_ = p;
		return *this;
	}

	StringT_out& operator = (const C* p)
	{
		p_ = Nirvana::StringManager <C>::singleton ()->string_dup (p);
		return *this;
	}

	operator C*& ()
	{
		return p_;
	}

	C*& ptr ()
	{
		return p_;
	}

private:
	// assignment from String_var disallowed
	void operator = (const String_var <C>&) = delete;

private:
	C*& p_;
};

typedef StringT_var <Char> String_var;
typedef StringT_out <Char> String_out;

typedef StringT_var <WChar> WString_var;
typedef StringT_out <WChar> WString_out;

}

#endif
