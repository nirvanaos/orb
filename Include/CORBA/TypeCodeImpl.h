#ifndef NIRVANA_ORB_TYPECODEIMPL_H_
#define NIRVANA_ORB_TYPECODEIMPL_H_

#include "TypeCode_s.h"
#include <new>

namespace CORBA {
namespace Nirvana {

class TypeCodeBase
{
public:
	static const char* id ()
	{
		throw_BadKind ();
	}

	static const char* name ()
	{
		throw_BadKind ();
	}

	static ULong member_count ()
	{
		throw_BadKind ();
	}

	static Identifier member_name (ULong index)
	{
		throw_BadKind ();
	}

	static TypeCode_ptr member_type (ULong index)
	{
		throw_BadKind ();
	}

	static Any* member_label (ULong index)
	{
		throw_BadKind ();
	}

	static TypeCode_ptr discriminator_type ()
	{
		throw_BadKind ();
	}

	static Long default_index ()
	{
		throw_BadKind ();
	}

	static ULong length ()
	{
		throw_BadKind ();
	}

	static TypeCode_ptr content_type ()
	{
		throw_BadKind ();
	}

	static UShort fixed_digits ()
	{
		throw_BadKind ();
	}

	static Short fixed_scale ()
	{
		throw_BadKind ();
	}

	static Visibility member_visibility (ULong index)
	{
		throw_BadKind ();
	}

	static ValueModifier type_modifier ()
	{
		throw_BadKind ();
	}

	static TypeCode_ptr concrete_base_type ()
	{
		throw_BadKind ();
	}

protected:
	NIRVANA_NORETURN static void throw_BadKind ();
	NIRVANA_NORETURN static void throw_Bounds ();

	static Boolean equal (TCKind tk, TypeCode_ptr other);
	static Boolean equivalent (TCKind tk, TypeCode_ptr other);
};

template <typename T>
class TypeCodeOps
{
public:
	static ULong _size ()
	{
		return sizeof (T);
	}

	static void _construct (::Nirvana::Pointer p)
	{
		new (reinterpret_cast <T*> (p)) T ();
	}

	static void _destruct (::Nirvana::Pointer p)
	{
		reinterpret_cast <T*> (p)->~T ();
	}

	static void _copy (::Nirvana::Pointer dst, ::Nirvana::ConstPointer src)
	{
		new (dst) T (*reinterpret_cast <const T*> (src));
	}

	static void _move (::Nirvana::Pointer dst, ::Nirvana::Pointer src)
	{
		new (dst) T (std::move (*reinterpret_cast <T*> (src)));
	}
};

}
}

#endif
