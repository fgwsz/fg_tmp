#ifndef __STATIC_REFLECTION_HPP__
#define __STATIC_REFLECTION_HPP__
#include "namespace_macro.h"
#include "static_string.hpp"
// 将编译期字符串和数据类型type相关联
template<typename static_string>struct _RegistType {
	static_assert(is_static_string<static_string>, "请输入static_string");
	using type = void;
};
// 将字符串字面量__C_STR__(底层实现：编译期字符串)和数据类型__TYPE_NAME__相关联
// 有头文件防卫保护的类外、函数外使用 
// 配合GET_TYPE 使用
#define REGIST_TYPE(__C_STR__,__TYPE_NAME__) \
template<> struct _RegistType<STATIC_STRING(__C_STR__)> \
{ using type = __TYPE_NAME__; }; \
// 得到和字符串字面量__C_STR__相关联的数据类型
#define GET_TYPE(__C_STR__) \
typename ::_RegistType<STATIC_STRING(__C_STR__)>::type \
// 得到和编译期字符串相关联的数据类型
template<typename static_string>
using GetType = typename ::_RegistType<static_string>::type;
//////////////////////////////////////////////////////////////////////////
#define REGIST_BASE_TYPE(__BASE_TYPE_NAME__) \
REGIST_TYPE(#__BASE_TYPE_NAME__,__BASE_TYPE_NAME__) \
// 
REGIST_BASE_TYPE(void)
REGIST_BASE_TYPE(bool)
REGIST_BASE_TYPE(char)
REGIST_BASE_TYPE(short)
REGIST_BASE_TYPE(int)
REGIST_BASE_TYPE(long)
REGIST_BASE_TYPE(float)
REGIST_BASE_TYPE(double)
REGIST_BASE_TYPE(unsigned)
REGIST_BASE_TYPE(unsigned char)
REGIST_BASE_TYPE(signed char)
REGIST_BASE_TYPE(char16_t)
REGIST_BASE_TYPE(char32_t)
REGIST_BASE_TYPE(wchar_t)
REGIST_BASE_TYPE(unsigned int)
REGIST_BASE_TYPE(signed int)
REGIST_BASE_TYPE(short int)
REGIST_BASE_TYPE(unsigned short int)
REGIST_BASE_TYPE(unsigned short)
REGIST_BASE_TYPE(signed short int)
REGIST_BASE_TYPE(signed short)
REGIST_BASE_TYPE(long int)
REGIST_BASE_TYPE(unsigned long int)
REGIST_BASE_TYPE(unsigned long)
REGIST_BASE_TYPE(signed long int)
REGIST_BASE_TYPE(signed long)
REGIST_BASE_TYPE(long double)
REGIST_BASE_TYPE(long long)
REGIST_BASE_TYPE(unsigned long long)
REGIST_BASE_TYPE(signed long long)
REGIST_BASE_TYPE(size_t)
#undef REGIST_BASE_TYPE
#endif //!__STATIC_REFLECTION_HPP__