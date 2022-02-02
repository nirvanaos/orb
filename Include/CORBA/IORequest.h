// This file was generated from "IORequest.idl"
// Nirvana IDL compiler version 1.0
#ifndef IDL_IOREQUEST_H_
#define IDL_IOREQUEST_H_
#include <TypeCode.h>

namespace CORBA {
namespace Internal {

template <>
struct Definitions < IORequest>
{

	typedef Sequence <Char> CharSeq;

	typedef Sequence <Wchar> WCharSeq;
};

NIRVANA_BRIDGE_BEGIN (IORequest, "IDL:omg.org/CORBA/Internal/IORequest:1.0")
void (*marshal_CDR) (Bridge < IORequest>* _b, Type < ::Nirvana::Size>::ABI_in, Type < ::Nirvana::Size>::ABI_in, Type < ::Nirvana::ConstPointer>::ABI_in, Interface* _env);
Type <Boolean>::ABI_ret (*unmarshal_CDR) (Bridge < IORequest>* _b, Type < ::Nirvana::Size>::ABI_in, Type < ::Nirvana::Size>::ABI_in, Type < ::Nirvana::ConstPointer>::ABI_out, Interface* _env);
void (*marshal_CDR_seq) (Bridge < IORequest>* _b, Type < ::Nirvana::Size>::ABI_in, Type < ::Nirvana::Size>::ABI_in, Type < ::Nirvana::Size>::ABI_in, Type < ::Nirvana::Pointer>::ABI_in, Type < ::Nirvana::Size>::ABI_in, Interface* _env);
Type <Boolean>::ABI_ret (*unmarshal_CDR_seq) (Bridge < IORequest>* _b, Type < ::Nirvana::Size>::ABI_in, Type < ::Nirvana::Size>::ABI_in, Type < ::Nirvana::Size>::ABI_out, Type < ::Nirvana::Pointer>::ABI_out, Type < ::Nirvana::Size>::ABI_out, Interface* _env);
Type < ::Nirvana::Pointer>::ABI_ret (*marshal_get_buffer) (Bridge < IORequest>* _b, Type < ::Nirvana::Size>::ABI_in, Type < ::Nirvana::Size>::ABI_in, Interface* _env);
Type < ::Nirvana::Pointer>::ABI_ret (*marshal_get_buffer_seq) (Bridge < IORequest>* _b, Type < ::Nirvana::Size>::ABI_in, Type < ::Nirvana::Size>::ABI_in, Type < ::Nirvana::Size>::ABI_out, Interface* _env);
void (*marshal_char) (Bridge < IORequest>* _b, Type < ::Nirvana::Size>::ABI_in, Type <ConstCharPtr>::ABI_in, Interface* _env);
void (*unmarshal_char) (Bridge < IORequest>* _b, Type < ::Nirvana::Size>::ABI_in, Type <CharPtr>::ABI_in, Interface* _env);
void (*marshal_string) (Bridge < IORequest>* _b, Type <String>::ABI_out, Type <Boolean>::ABI_in, Interface* _env);
void (*unmarshal_string) (Bridge < IORequest>* _b, Type <String>::ABI_out, Interface* _env);
void (*marshal_char_seq) (Bridge < IORequest>* _b, Type <Definitions <IORequest>::CharSeq>::ABI_out, Type <Boolean>::ABI_in, Interface* _env);
void (*unmarshal_char_seq) (Bridge < IORequest>* _b, Type <Definitions <IORequest>::CharSeq>::ABI_out, Interface* _env);
void (*marshal_wchar) (Bridge < IORequest>* _b, Type < ::Nirvana::Size>::ABI_in, Type <ConstWCharPtr>::ABI_in, Interface* _env);
void (*unmarshal_wchar) (Bridge < IORequest>* _b, Type < ::Nirvana::Size>::ABI_in, Type <WCharPtr>::ABI_in, Interface* _env);
void (*marshal_wstring) (Bridge < IORequest>* _b, Type <WString>::ABI_out, Type <Boolean>::ABI_in, Interface* _env);
void (*unmarshal_wstring) (Bridge < IORequest>* _b, Type <WString>::ABI_out, Interface* _env);
void (*marshal_wchar_seq) (Bridge < IORequest>* _b, Type <Definitions <IORequest>::WCharSeq>::ABI_out, Type <Boolean>::ABI_in, Interface* _env);
void (*unmarshal_wchar_seq) (Bridge < IORequest>* _b, Type <Definitions <IORequest>::WCharSeq>::ABI_out, Interface* _env);
void (*marshal_fixed_size) (Bridge < IORequest>* _b, Type < ::Nirvana::Size>::ABI_in, Type < ::Nirvana::Size>::ABI_in, Type < ::Nirvana::Size>::ABI_in, Type < ::Nirvana::ConstPointer>::ABI_in, Interface*, Interface* _env);
Type <Boolean>::ABI_ret (*unmarshal_fixed_size) (Bridge < IORequest>* _b, Type < ::Nirvana::Size>::ABI_in, Type < ::Nirvana::Size>::ABI_in, Type < ::Nirvana::Size>::ABI_in, Type < ::Nirvana::Pointer>::ABI_in, Interface*, Interface* _env);
void (*marshal_fixed_size_seq) (Bridge < IORequest>* _b, Type < ::Nirvana::Size>::ABI_in, Type < ::Nirvana::Size>::ABI_in, Type < ::Nirvana::Size>::ABI_in, Type < ::Nirvana::Pointer>::ABI_in, Type < ::Nirvana::Size>::ABI_in, Interface*, Interface* _env);
Type <Boolean>::ABI_ret (*unmarshal_fixed_size_seq) (Bridge < IORequest>* _b, Type < ::Nirvana::Size>::ABI_in, Type < ::Nirvana::Size>::ABI_in, Type < ::Nirvana::Size>::ABI_out, Type < ::Nirvana::Pointer>::ABI_out, Type < ::Nirvana::Size>::ABI_out, Interface*, Interface* _env);
void (*marshal_interface) (Bridge < IORequest>* _b, Interface*, Interface*, Interface* _env);
Interface* (*unmarshal_interface) (Bridge < IORequest>* _b, Interface* _env);
void (*marshal_type_code) (Bridge < IORequest>* _b, Interface*, Interface* _env);
Interface* (*unmarshal_type_code) (Bridge < IORequest>* _b, Interface* _env);
void (*set_exception) (Bridge < IORequest>* _b, Type <Any>::ABI_out, Interface* _env);
void (*success) (Bridge < IORequest>* _b, Interface* _env);
Type <Boolean>::ABI_ret (*is_completed) (Bridge < IORequest>* _b, Interface* _env);
void (*wait) (Bridge < IORequest>* _b, Interface* _env);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, IORequest> :
	public T,
	public Definitions <IORequest>
{
public:
	void marshal_CDR (Type < ::Nirvana::Size>::C_in align, Type < ::Nirvana::Size>::C_in size, Type < ::Nirvana::ConstPointer>::C_in data);
	Type <Boolean>::Var unmarshal_CDR (Type < ::Nirvana::Size>::C_in align, Type < ::Nirvana::Size>::C_in size, Type < ::Nirvana::ConstPointer>::C_out data);
	void marshal_CDR_seq (Type < ::Nirvana::Size>::C_in align, Type < ::Nirvana::Size>::C_in element_size, Type < ::Nirvana::Size>::C_in element_count, Type < ::Nirvana::Pointer>::C_in data, Type < ::Nirvana::Size>::C_in allocated_size);
	Type <Boolean>::Var unmarshal_CDR_seq (Type < ::Nirvana::Size>::C_in align, Type < ::Nirvana::Size>::C_in element_size, Type < ::Nirvana::Size>::C_out element_count, Type < ::Nirvana::Pointer>::C_out data, Type < ::Nirvana::Size>::C_out allocated_size);
	Type < ::Nirvana::Pointer>::Var marshal_get_buffer (Type < ::Nirvana::Size>::C_in align, Type < ::Nirvana::Size>::C_in size_at_least);
	Type < ::Nirvana::Pointer>::Var marshal_get_buffer_seq (Type < ::Nirvana::Size>::C_in align, Type < ::Nirvana::Size>::C_in element_size, Type < ::Nirvana::Size>::C_out element_count);
	void marshal_char (Type < ::Nirvana::Size>::C_in count, Type <ConstCharPtr>::C_in data);
	void unmarshal_char (Type < ::Nirvana::Size>::C_in count, Type <CharPtr>::C_in data);
	void marshal_string (Type <String>::C_inout data, Type <Boolean>::C_in move);
	void unmarshal_string (Type <String>::C_out data);
	void marshal_char_seq (Type <Definitions <IORequest>::CharSeq>::C_inout data, Type <Boolean>::C_in move);
	void unmarshal_char_seq (Type <Definitions <IORequest>::CharSeq>::C_out data);
	void marshal_wchar (Type < ::Nirvana::Size>::C_in count, Type <ConstWCharPtr>::C_in data);
	void unmarshal_wchar (Type < ::Nirvana::Size>::C_in count, Type <WCharPtr>::C_in data);
	void marshal_wstring (Type <WString>::C_inout data, Type <Boolean>::C_in move);
	void unmarshal_wstring (Type <WString>::C_out data);
	void marshal_wchar_seq (Type <Definitions <IORequest>::WCharSeq>::C_inout data, Type <Boolean>::C_in move);
	void unmarshal_wchar_seq (Type <Definitions <IORequest>::WCharSeq>::C_out data);
	void marshal_fixed_size (Type < ::Nirvana::Size>::C_in align, Type < ::Nirvana::Size>::C_in element_size, Type < ::Nirvana::Size>::C_in element_count, Type < ::Nirvana::ConstPointer>::C_in data, Type <TypeCode>::C_in element_tc);
	Type <Boolean>::Var unmarshal_fixed_size (Type < ::Nirvana::Size>::C_in align, Type < ::Nirvana::Size>::C_in element_size, Type < ::Nirvana::Size>::C_in element_count, Type < ::Nirvana::Pointer>::C_in data, Type <TypeCode>::C_in element_tc);
	void marshal_fixed_size_seq (Type < ::Nirvana::Size>::C_in align, Type < ::Nirvana::Size>::C_in element_size, Type < ::Nirvana::Size>::C_in element_count, Type < ::Nirvana::Pointer>::C_in data, Type < ::Nirvana::Size>::C_in allocated_size, Type <TypeCode>::C_in element_tc);
	Type <Boolean>::Var unmarshal_fixed_size_seq (Type < ::Nirvana::Size>::C_in align, Type < ::Nirvana::Size>::C_in element_size, Type < ::Nirvana::Size>::C_out element_count, Type < ::Nirvana::Pointer>::C_out data, Type < ::Nirvana::Size>::C_out allocated_size, Type <TypeCode>::C_in element_tc);
	void marshal_interface (Type <Object>::C_in obj, Type <Interface>::C_in itf);
	Type <Interface>::Var unmarshal_interface ();
	void marshal_type_code (Type <TypeCode>::C_in tc);
	Type <TypeCode>::Var unmarshal_type_code ();
	void set_exception (Type <Any>::C_inout e);
	void success ();
	Type <Boolean>::Var is_completed ();
	void wait ();
};

template <class T>
void Client <T, IORequest>::marshal_CDR (Type < ::Nirvana::Size>::C_in align, Type < ::Nirvana::Size>::C_in size, Type < ::Nirvana::ConstPointer>::C_in data)
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.marshal_CDR) (&_b, &align, &size, &data, &_env);
	_env.check ();
}

template <class T>
Type <Boolean>::Var Client <T, IORequest>::unmarshal_CDR (Type < ::Nirvana::Size>::C_in align, Type < ::Nirvana::Size>::C_in size, Type < ::Nirvana::ConstPointer>::C_out data)
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	Type <Boolean>::C_ret _ret = (_b._epv ().epv.unmarshal_CDR) (&_b, &align, &size, &data, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, IORequest>::marshal_CDR_seq (Type < ::Nirvana::Size>::C_in align, Type < ::Nirvana::Size>::C_in element_size, Type < ::Nirvana::Size>::C_in element_count, Type < ::Nirvana::Pointer>::C_in data, Type < ::Nirvana::Size>::C_in allocated_size)
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.marshal_CDR_seq) (&_b, &align, &element_size, &element_count, &data, &allocated_size, &_env);
	_env.check ();
}

template <class T>
Type <Boolean>::Var Client <T, IORequest>::unmarshal_CDR_seq (Type < ::Nirvana::Size>::C_in align, Type < ::Nirvana::Size>::C_in element_size, Type < ::Nirvana::Size>::C_out element_count, Type < ::Nirvana::Pointer>::C_out data, Type < ::Nirvana::Size>::C_out allocated_size)
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	Type <Boolean>::C_ret _ret = (_b._epv ().epv.unmarshal_CDR_seq) (&_b, &align, &element_size, &element_count, &data, &allocated_size, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Type < ::Nirvana::Pointer>::Var Client <T, IORequest>::marshal_get_buffer (Type < ::Nirvana::Size>::C_in align, Type < ::Nirvana::Size>::C_in size_at_least)
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	Type < ::Nirvana::Pointer>::C_ret _ret = (_b._epv ().epv.marshal_get_buffer) (&_b, &align, &size_at_least, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Type < ::Nirvana::Pointer>::Var Client <T, IORequest>::marshal_get_buffer_seq (Type < ::Nirvana::Size>::C_in align, Type < ::Nirvana::Size>::C_in element_size, Type < ::Nirvana::Size>::C_out element_count)
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	Type < ::Nirvana::Pointer>::C_ret _ret = (_b._epv ().epv.marshal_get_buffer_seq) (&_b, &align, &element_size, &element_count, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, IORequest>::marshal_char (Type < ::Nirvana::Size>::C_in count, Type <ConstCharPtr>::C_in data)
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.marshal_char) (&_b, &count, &data, &_env);
	_env.check ();
}

template <class T>
void Client <T, IORequest>::unmarshal_char (Type < ::Nirvana::Size>::C_in count, Type <CharPtr>::C_in data)
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.unmarshal_char) (&_b, &count, &data, &_env);
	_env.check ();
}

template <class T>
void Client <T, IORequest>::marshal_string (Type <String>::C_inout data, Type <Boolean>::C_in move)
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.marshal_string) (&_b, &data, &move, &_env);
	_env.check ();
}

template <class T>
void Client <T, IORequest>::unmarshal_string (Type <String>::C_out data)
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.unmarshal_string) (&_b, &data, &_env);
	_env.check ();
}

template <class T>
void Client <T, IORequest>::marshal_char_seq (Type <Definitions <IORequest>::CharSeq>::C_inout data, Type <Boolean>::C_in move)
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.marshal_char_seq) (&_b, &data, &move, &_env);
	_env.check ();
}

template <class T>
void Client <T, IORequest>::unmarshal_char_seq (Type <Definitions <IORequest>::CharSeq>::C_out data)
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.unmarshal_char_seq) (&_b, &data, &_env);
	_env.check ();
}

template <class T>
void Client <T, IORequest>::marshal_wchar (Type < ::Nirvana::Size>::C_in count, Type <ConstWCharPtr>::C_in data)
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.marshal_wchar) (&_b, &count, &data, &_env);
	_env.check ();
}

template <class T>
void Client <T, IORequest>::unmarshal_wchar (Type < ::Nirvana::Size>::C_in count, Type <WCharPtr>::C_in data)
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.unmarshal_wchar) (&_b, &count, &data, &_env);
	_env.check ();
}

template <class T>
void Client <T, IORequest>::marshal_wstring (Type <WString>::C_inout data, Type <Boolean>::C_in move)
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.marshal_wstring) (&_b, &data, &move, &_env);
	_env.check ();
}

template <class T>
void Client <T, IORequest>::unmarshal_wstring (Type <WString>::C_out data)
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.unmarshal_wstring) (&_b, &data, &_env);
	_env.check ();
}

template <class T>
void Client <T, IORequest>::marshal_wchar_seq (Type <Definitions <IORequest>::WCharSeq>::C_inout data, Type <Boolean>::C_in move)
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.marshal_wchar_seq) (&_b, &data, &move, &_env);
	_env.check ();
}

template <class T>
void Client <T, IORequest>::unmarshal_wchar_seq (Type <Definitions <IORequest>::WCharSeq>::C_out data)
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.unmarshal_wchar_seq) (&_b, &data, &_env);
	_env.check ();
}

template <class T>
void Client <T, IORequest>::marshal_fixed_size (Type < ::Nirvana::Size>::C_in align, Type < ::Nirvana::Size>::C_in element_size, Type < ::Nirvana::Size>::C_in element_count, Type < ::Nirvana::ConstPointer>::C_in data, Type <TypeCode>::C_in element_tc)
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.marshal_fixed_size) (&_b, &align, &element_size, &element_count, &data, &element_tc, &_env);
	_env.check ();
}

template <class T>
Type <Boolean>::Var Client <T, IORequest>::unmarshal_fixed_size (Type < ::Nirvana::Size>::C_in align, Type < ::Nirvana::Size>::C_in element_size, Type < ::Nirvana::Size>::C_in element_count, Type < ::Nirvana::Pointer>::C_in data, Type <TypeCode>::C_in element_tc)
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	Type <Boolean>::C_ret _ret = (_b._epv ().epv.unmarshal_fixed_size) (&_b, &align, &element_size, &element_count, &data, &element_tc, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, IORequest>::marshal_fixed_size_seq (Type < ::Nirvana::Size>::C_in align, Type < ::Nirvana::Size>::C_in element_size, Type < ::Nirvana::Size>::C_in element_count, Type < ::Nirvana::Pointer>::C_in data, Type < ::Nirvana::Size>::C_in allocated_size, Type <TypeCode>::C_in element_tc)
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.marshal_fixed_size_seq) (&_b, &align, &element_size, &element_count, &data, &allocated_size, &element_tc, &_env);
	_env.check ();
}

template <class T>
Type <Boolean>::Var Client <T, IORequest>::unmarshal_fixed_size_seq (Type < ::Nirvana::Size>::C_in align, Type < ::Nirvana::Size>::C_in element_size, Type < ::Nirvana::Size>::C_out element_count, Type < ::Nirvana::Pointer>::C_out data, Type < ::Nirvana::Size>::C_out allocated_size, Type <TypeCode>::C_in element_tc)
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	Type <Boolean>::C_ret _ret = (_b._epv ().epv.unmarshal_fixed_size_seq) (&_b, &align, &element_size, &element_count, &data, &allocated_size, &element_tc, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, IORequest>::marshal_interface (Type <Object>::C_in obj, Type <Interface>::C_in itf)
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.marshal_interface) (&_b, &obj, &itf, &_env);
	_env.check ();
}

template <class T>
Type <Interface>::Var Client <T, IORequest>::unmarshal_interface ()
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	Type <Interface>::C_ret _ret = (_b._epv ().epv.unmarshal_interface) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, IORequest>::marshal_type_code (Type <TypeCode>::C_in tc)
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.marshal_type_code) (&_b, &tc, &_env);
	_env.check ();
}

template <class T>
Type <TypeCode>::Var Client <T, IORequest>::unmarshal_type_code ()
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	Type <TypeCode>::C_ret _ret = (_b._epv ().epv.unmarshal_type_code) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, IORequest>::set_exception (Type <Any>::C_inout e)
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.set_exception) (&_b, &e, &_env);
	_env.check ();
}

template <class T>
void Client <T, IORequest>::success ()
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.success) (&_b, &_env);
	_env.check ();
}

template <class T>
Type <Boolean>::Var Client <T, IORequest>::is_completed ()
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	Type <Boolean>::C_ret _ret = (_b._epv ().epv.is_completed) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, IORequest>::wait ()
{
	Environment _env;
	Bridge < IORequest>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.wait) (&_b, &_env);
	_env.check ();
}

class IORequest : public ClientInterface <IORequest>
{
public:
	using Definitions <IORequest>::CharSeq;
	using Definitions <IORequest>::WCharSeq;
};

}
}

#endif
