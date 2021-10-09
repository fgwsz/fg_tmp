#ifndef    __STATIC_STRING_HPP__
#define    __STATIC_STRING_HPP__
#include "namespace_macro.h"
#include "static_if.hpp"
#include "index_seq.hpp"
NAMESPACE_BEGIN(fg_tmp)
// 编译期字符类型
using char_t = unsigned char;
// 编译期字符串
template<char_t...values> struct StaticString 
{   static constexpr auto size = sizeof...(values);
	static constexpr const char_t value[size + 1] = {values...,'\0'}; };
NAMESPACE_BEGIN(impl_static_string)
template<typename CStr, size_t...values>
static constexpr auto _cStrToString(CStr, IndexSeq<values...>) 
{ return StaticString<CStr::value()[values]...>{}; }
template<typename CStr>
static constexpr auto _cStrToString(CStr obj) {
	return _cStrToString(obj, IndexSeq_Make<
		sizeof(CStr::value()) / sizeof(CStr::value()[0]) - 1
	>{});
}
// 输入字符串字面量 得到 编译期字符串
#define STATIC_STRING(__C_STR__) \
decltype( \
	::fg_tmp::impl_static_string::_cStrToString( \
		[]() { \
			struct CStr { static constexpr decltype(auto) value(){ return __C_STR__;} }; \
			return CStr{}; \
		}() \
    ) \
) \
/* */
template<typename Type>struct _Is_StaticString {
	static constexpr auto value = false;
};
template<char_t...values>struct _Is_StaticString<StaticString<values...>> {
	static constexpr auto value = true;
};
template<typename static_string, char_t value> struct _StaticString_PushFront;
template<char_t...values, char_t value>
struct _StaticString_PushFront<StaticString<values...>, value> {
	using type = StaticString<value, values...>;
};
template<typename static_string, char_t value> struct _StaticString_PushBack;
template<char_t...values, char_t value> 
struct _StaticString_PushBack<StaticString<values...>, value> {
	using type = StaticString<values...,value>;
};
template<size_t n, char_t value>struct _StaticString_Make {
	using type = typename _StaticString_PushBack<
		typename _StaticString_Make<n - 1, value>::type,
		value
	>::type;
};
template<char_t value>struct _StaticString_Make<0, value> { using type = StaticString<>; };
template<typename...static_string_package>struct _StaticString_Concat;
template<char_t...values, typename...static_string_package>
struct _StaticString_Concat<StaticString<values...>,static_string_package...> {
	using type = typename _StaticString_Concat <
		StaticString<values...>
		, If_t<sizeof...(static_string_package)
		, typename _StaticString_Concat<static_string_package...>::type
		, StaticString<>
		>
	>::type;
};
template<char_t...values1, char_t...values2>
struct _StaticString_Concat<StaticString<values1...>, StaticString<values2...>> 
{ using type = StaticString<values1..., values2...>; };
template<>struct _StaticString_Concat<> { using type = StaticString<>; };
NAMESPACE_END // impl_static_string
// 判断 是否为 编译期字符串
template<typename Type>
static constexpr auto is_static_string = impl_static_string::_Is_StaticString<Type>::value;
// 构造编译期字符串(n 个 value 字符)
template<size_t n, char_t value>
using StaticString_Make = typename impl_static_string::_StaticString_Make<n, value>::type;
// 编译期字符串：头部插入一个字符
template<typename static_string, char_t value> 
using StaticString_PushFront = typename impl_static_string::_StaticString_PushFront<static_string, value>::type;
// 编译期字符串：尾部插入一个字符
template<typename static_string, char_t value> 
using StaticString_PushBack = typename impl_static_string::_StaticString_PushBack<static_string, value>::type;
// 首尾连接多个编译期字符串为一个编译期字符串
template<typename...static_string_package>
using StaticString_Concat = typename impl_static_string::_StaticString_Concat<static_string_package...>::type;
NAMESPACE_END // fg_tmp
#endif // !__STATIC_STRING_HPP__
