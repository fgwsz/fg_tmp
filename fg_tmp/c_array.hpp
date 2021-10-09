#ifndef __C_ARRAY_HPP__
#define __C_ARRAY_HPP__
#include "namespace_macro.h"
#include "static_if.hpp"
NAMESPACE_BEGIN(fg_tmp)
NAMESPACE_BEGIN(impl_c_array)
template<typename T, size_t N>struct _AddArr { using type = T[N]; };
template<typename T, size_t N>struct _AddArr<T[],N> { using type = T[N]; };
template<typename T, size_t... Ns>struct _CArray;
template<typename T, size_t N, size_t... Ns>
struct _CArray<T, N, Ns...> {
	using type = If_t<
		sizeof...(Ns) != 0,
		typename _AddArr<typename _CArray<T,Ns...>::type,N>::type,
		typename _AddArr<T, N>::type
	>;
};
template<typename T>
struct _CArray <T> {
	using type = T[];
};
template<typename T>struct _AddPtr { using type = T*; };
template<typename T>struct _AddRef { using type = T&; };
template<typename T>struct _IsCArray { static constexpr auto value = false; };
template<typename T>struct _IsCArray<T[]> { static constexpr auto value = true; };
template<typename T,size_t N>struct _IsCArray<T[N]> { static constexpr auto value = true; };
template<typename T>struct _IsCArrayPtr { static constexpr auto value = false; };
template<typename T>struct _IsCArrayPtr<T(*)[]> { static constexpr auto value = true; };
template<typename T, size_t N>struct _IsCArrayPtr<T(*)[N]> { static constexpr auto value = true; };
template<typename T>struct _IsCArrayRef { static constexpr auto value = false; };
template<typename T>struct _IsCArrayRef<T(&)[]> { static constexpr auto value = true; };
template<typename T, size_t N>struct _IsCArrayRef<T(&)[N]> { static constexpr auto value = true; };
template<typename T>struct _CArrayMemType { using type = void; };
template<typename T>struct _CArrayMemType<T[]>
{ using type = If_t<_IsCArray<T>::value,typename _CArrayMemType<T>::type,T>; };
template<typename T,size_t N>struct _CArrayMemType<T[N]>
{ using type = If_t<_IsCArray<T>::value, typename _CArrayMemType<T>::type, T>; };
template<typename T, size_t N>struct _CArrayMemType<T(*)[N]>
{ using type = If_t<_IsCArray<T>::value, typename _CArrayMemType<T>::type, T>; };
template<typename T, size_t N>struct _CArrayMemType<T(&)[N]>
{ using type = If_t<_IsCArray<T>::value, typename _CArrayMemType<T>::type, T>; };
NAMESPACE_END // impl_c_array
// 表示一个C风格的多维数组类型
template<typename T, size_t... Ns>
using CArray = typename impl_c_array::_CArray<T, Ns...>::type;
// 表示一个C风格的多维数组指针 类型
template<typename T, size_t... Ns>
using CArrayPtr = typename impl_c_array::_AddPtr<CArray<T, Ns...>>::type;
// 表示一个C风格的多维数组引用 类型
template<typename T, size_t... Ns>
using CArrayRef = typename impl_c_array::_AddRef<CArray<T, Ns...>>::type;
// 判断 是否 为 C 风格 数组类型
template<typename T>
static constexpr auto is_c_array = impl_c_array::_IsCArray<T>::value;
// 判断 是否 为 C 风格 数组 指针类型
template<typename T>
static constexpr auto is_c_array_ptr = impl_c_array::_IsCArrayPtr<T>::value;
// 判断 是否 为 C 风格 数组 引用类型
template<typename T>
static constexpr auto is_c_array_ref = impl_c_array::_IsCArrayRef<T>::value;
// 得到  C 风格 数组 的 元素类型
template<typename T>
using CArrayMemType = typename impl_c_array::_CArrayMemType<T>::type;
NAMESPACE_END // fg_tmp
#endif //!__C_ARRAY_HPP__