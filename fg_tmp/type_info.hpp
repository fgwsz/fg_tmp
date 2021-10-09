#ifndef __TYPE_INFO_HPP__
#define __TYPE_INFO_HPP__
#include "namespace_macro.h"
#include "static_num_to_str.hpp"
#include "c_array.hpp"
#include <iostream>
#include "static_reflection.hpp"
NAMESPACE_BEGIN(fg_tmp)
using string_t = ::std::string;
REGIST_TYPE("string", string_t);
NAMESPACE_BEGIN(impl_type_info)
template <typename T>
struct _TypeInfo
{ 
	using type = T;
	static string_t name() { return typeid(T).name(); } 
};
template <typename T>
struct _TypeInfo<T const>
{ 
	using type = typename _TypeInfo<T>::type const;
	static string_t name() { return _TypeInfo<T>::name().append(" const"); }
};
template <typename T>
struct _TypeInfo<T volatile>
{ 
	using type = typename _TypeInfo<T>::type volatile;
	static string_t name() { return _TypeInfo<T>::name().append(" volatile");; }
};
template <typename T>
struct _TypeInfo<T const volatile>
{ 
	using type = typename _TypeInfo<T>::type const volatile;
	static string_t name() { return _TypeInfo<T>::name().append(" const volatile"); }
};
template <typename T>
struct _TypeInfo<T*>
{ 
	using type = typename _TypeInfo<T>::type*;
	static string_t name() { return _TypeInfo<T>::name().append("*"); }
};
template <typename T>
struct _TypeInfo<T&>
{
	using type = typename _TypeInfo<T>::type&;
	static string_t name() { return _TypeInfo<T>::name().append("&"); }
};
template <typename T>
struct _TypeInfo<T&&>
{
	using type = typename _TypeInfo<T>::type&&;
	static string_t name() { return _TypeInfo<T>::name().append("&&"); }
};
template <typename T>
struct _TypeInfo<T[]>
{
	using type = typename _TypeInfo<T>::type[];
	static string_t name() { return _TypeInfo<T>::name().append("[]"); } 
};
template <typename T>
struct _TypeInfo<T const[]>
{
	using type = typename _TypeInfo<T>::type const [];
	static string_t name() { return _TypeInfo<T>::name().append(" const[]"); }
};
template <typename T>
struct _TypeInfo<T volatile[]>
{
	using type = typename _TypeInfo<T>::type volatile [];
	static string_t name() { return _TypeInfo<T>::name().append(" volatile[]"); }
};
template <typename T>
struct _TypeInfo<T const volatile[]>
{
	using type = typename _TypeInfo<T>::type const volatile [];
	static string_t name() { return _TypeInfo<T>::name().append(" const volatile[]"); }
};
template <typename T>
struct _TypeInfo<T(*)[]>
{
	using type = typename _TypeInfo<T>::type(*)[];
	static string_t name() { return _TypeInfo<T>::name().append("(*)[]"); }
};
template <typename T>
struct _TypeInfo<T(&)[]>
{
	using type = typename _TypeInfo<T>::type(&)[];
	static string_t name() { return _TypeInfo<T>::name().append("(&)[]"); }
};
template <typename T, size_t N>
struct _TypeInfo<T[N]>
{
	using type = typename _TypeInfo<T>::type[N];
	static string_t getIndexString() {
		string_t ret = 
			string_t("[")
			.append( reinterpret_cast<char const *>(NumToStr<static_cast<number_t>(N)>::value) )
		    .append("]");
		if constexpr (is_c_array<decltype(T{}) > ) {
			ret.append(_TypeInfo<T>::getIndexString());
		}
		return ret;
	}
	static string_t name() { 
		return _TypeInfo<CArrayMemType<T[N]>>::name().append(getIndexString());
	}
};
template <typename T, size_t N>
struct _TypeInfo<T const [N]>
{
	using type = typename _TypeInfo<T const>::type[N];
	static string_t getIndexString() {
		return _TypeInfo<T[N]>::getIndexString();
	}
	static string_t name() {
		return _TypeInfo<CArrayMemType<T[N]>>::name()
			.append(" const")
			.append(getIndexString());
	}
};
template <typename T, size_t N>
struct _TypeInfo<T volatile [N]>
{
	using type = typename _TypeInfo<T volatile>::type[N];
	static string_t getIndexString() {
		return _TypeInfo<T[N]>::getIndexString();
	}
	static string_t name() {
		return _TypeInfo<CArrayMemType<T[N]>>::name()
			.append(" volatile")
			.append(getIndexString());
	}
};
template <typename T, size_t N>
struct _TypeInfo<T const volatile [N]>
{
	using type = typename _TypeInfo<T const volatile>::type[N];
	static string_t getIndexString() {
		return _TypeInfo<T[N]>::getIndexString();
	}
	static string_t name() {
		return _TypeInfo<CArrayMemType<T[N]>>::name()
			.append(" const volatile")
			.append(getIndexString());
	}
};
template <typename T, size_t N>
struct _TypeInfo<T(*)[N]>
{
	using type = typename _TypeInfo<T>::type(*)[N];
	static string_t name() {
		return _TypeInfo<CArrayMemType<T[N]>>::name()
			.append("(*)")
			.append(_TypeInfo<T[N]>::getIndexString());
	}
};
template <typename T, size_t N>
struct _TypeInfo<T(&)[N]>
{
	using type = typename _TypeInfo<T>::type(&)[N];
	static string_t name() {
		return _TypeInfo<CArrayMemType<T[N]>>::name()
			.append("(&)")
			.append(_TypeInfo<T[N]>::getIndexString());
	}
};
template <typename T, typename... Ts>
struct _TypeInfo<T(Ts...)>
{
	using type = typename _TypeInfo<T>::type(typename _TypeInfo<Ts>::type...);
	static string_t getArgListString() {
		string_t ret("(");
		if constexpr (sizeof...(Ts) != 0) {
			ret.append((... + (_TypeInfo<Ts>::name().append(","))));
			ret.pop_back();
		}
		ret.append(")");
		return ret;
	}
	static string_t name() {
		return _TypeInfo<T>::name().append(getArgListString());
	}
};
template <typename T, typename... Ts>
struct _TypeInfo<T(Ts...,...)>
{
	using type = typename _TypeInfo<T>::type(typename _TypeInfo<Ts>::type..., ...);
	static string_t getArgListString() {
		string_t ret("(");
		if constexpr (sizeof...(Ts) != 0) {
			ret.append((... + (_TypeInfo<Ts>::name().append(","))));
		}
		ret.append("...)");
		return ret;
	}
	static string_t name() {
		return _TypeInfo<T>::name()
			.append(getArgListString());
	}
};
template <typename T, typename... Ts>
struct _TypeInfo<T(*)(Ts...)>
{
	using type = typename _TypeInfo<T>::type(*)(typename _TypeInfo<Ts>::type...);
	static string_t name() {
		return _TypeInfo<T>::name()
			.append("(*)")
			.append(_TypeInfo<T(Ts...)>::getArgListString());
	}
};
template <typename T, typename... Ts>
struct _TypeInfo<T(*)(Ts..., ...)>
{
	using type = typename _TypeInfo<T>::type(*)(typename _TypeInfo<Ts>::type..., ...);
	static string_t name() {
		return _TypeInfo<T>::name()
			.append("(*)")
			.append(_TypeInfo<T(Ts...,...)>::getArgListString());
	} 
};
template <typename T, typename... Ts>
struct _TypeInfo<T(&)(Ts...)>
{
	using type = typename _TypeInfo<T>::type(&)(typename _TypeInfo<Ts>::type...);
	static string_t name() {
		return _TypeInfo<T>::name()
			.append("(&)")
			.append(_TypeInfo<T(Ts...)>::getArgListString());
	}
};
template <typename T, typename... Ts>
struct _TypeInfo<T(&)(Ts...,...)>
{
	using type = typename _TypeInfo<T>::type(&)(typename _TypeInfo<Ts>::type..., ...);
	static string_t name() {
		return _TypeInfo<T>::name()
			.append("(&)")
			.append(_TypeInfo<T(Ts...,...)>::getArgListString());
	}
};
template <typename T, typename Class>
struct _TypeInfo<T Class::*>
{
	using type = typename _TypeInfo<T>::type Class::*;
	static string_t name() {
		return _TypeInfo<T>::name()
			.append(" ")
			.append(_TypeInfo<Class>::name())
			.append("::*");
	}
};
template <typename T, typename Class, typename... Ts>
struct _TypeInfo<T(Class::*)(Ts...)>
{
	using type = typename _TypeInfo<T>::type(Class::*)(typename _TypeInfo<Ts>::type...);
	static string_t name() {
		return _TypeInfo<T>::name()
			.append( "(" )
			.append(_TypeInfo<Class>::name())
			.append("::*)")
			.append(_TypeInfo<T(Ts...)>::getArgListString());
	}
};
template <typename T, typename Class, typename... Ts>
struct _TypeInfo<T(Class::*)(Ts...)const>
{
	using type = typename _TypeInfo<T>::type(Class::*)(typename _TypeInfo<Ts>::type...)const;
	static string_t name() {
		return _TypeInfo<T>::name()
			.append("(")
			.append(_TypeInfo<Class>::name())
			.append("::*)")
			.append(_TypeInfo<T(Ts...)>::getArgListString())
			.append("const");
	}
};
template <typename T, typename Class, typename... Ts>
struct _TypeInfo<T(Class::*)(Ts...,...)>
{
	using type = typename _TypeInfo<T>::type(Class::*)(typename _TypeInfo<Ts>::type..., ...);
	static string_t name() {
		return _TypeInfo<T>::name()
			.append("(")
			.append(_TypeInfo<Class>::name())
			.append("::*)")
			.append(_TypeInfo<T(Ts...,...)>::getArgListString());
	}
};
template <typename T, typename Class, typename... Ts>
struct _TypeInfo<T(Class::*)(Ts..., ...)const>
{
	using type = typename _TypeInfo<T>::type(Class::*)(typename _TypeInfo<Ts>::type..., ...)const;
	static string_t name() {
		return _TypeInfo<T>::name()
			.append("(")
			.append(_TypeInfo<Class>::name())
			.append("::*)")
			.append(_TypeInfo<T(Ts..., ...)>::getArgListString())
			.append("const");
	}
};
NAMESPACE_END // impl_type_info
// 得到 模板参数 的 类型信息
template<typename T>using TypeInfo = impl_type_info::_TypeInfo<T>;
// 得到 输入量 的 真实 数据类型 的 类型信息
#define TYPE_INFO(__VAR__) ::fg_tmp::impl_type_info::_TypeInfo<decltype(__VAR__)>
NAMESPACE_END // fg_tmp
#endif //!__TYPE_INFO_HPP__