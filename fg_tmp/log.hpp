#ifndef __LOG_HPP__
#define __LOG_HPP__
#include <iostream>
#include "namespace_macro.h"
NAMESPACE_BEGIN(fg_tmp)
NAMESPACE_BEGIN(impl_log)
/////////////////////////////////////// 函数 声明 /////////////////////////////////////////

// 打印基础实现 采用::std::cout
template<typename T>void _log(T const& value); 
// 打印基础类型
void _log(bool const value);                           // 修正 bool 类型 打印
template<size_t N>void _log(char const(&)[N]);         // 修正 字符 数组 类型 打印
template<size_t N>void _log(unsigned char const(&)[N]);// 修正 字符 数组 类型 打印
template<size_t N>void _log(signed char const(&)[N]);  // 修正 字符 数组 类型 打印
template<size_t N>void _log(char16_t const(&)[N]);     // 修正 字符 数组 类型 打印
template<size_t N>void _log(char32_t const(&)[N]);     // 修正 字符 数组 类型 打印
template<size_t N>void _log(wchar_t const(&)[N]);      // 修正 字符 数组 类型 打印
// 打印C风格数组引用类型
template<typename T, size_t N>void _log(T(&c_arr_ref)[N]);
// 打印多个参数
template<typename T, typename...Ts>void _log(T const& first, Ts const & ... values);

/////////////////////////////////////// 函数 实现 /////////////////////////////////////////

template<typename T>void _log(T const& value) { ::std::cout << value; }
void _log(bool const value) { _log(value ? "true":"false"); }
template<size_t N>void _log(char const(&c_str)[N])
{ for (size_t idx = 0; idx < N - 1; ++idx)  { _log(c_str[idx]); } }
template<size_t N>void _log(unsigned char const(&c_str)[N])
{ for (size_t idx = 0; idx < N - 1; ++idx) { _log(c_str[idx]); } }
template<size_t N>void _log(signed char const(&c_str)[N])
{ for (size_t idx = 0; idx < N - 1; ++idx) { _log(c_str[idx]); } }
template<size_t N>void _log(char16_t const(&c_str)[N])
{ for (size_t idx = 0; idx < N - 1; ++idx) { _log(c_str[idx]); } }
template<size_t N>void _log(char32_t const(&c_str)[N])
{ for (size_t idx = 0; idx < N - 1; ++idx) { _log(c_str[idx]); } }
template<size_t N>void _log(wchar_t const(&c_str)[N])
{ for (size_t idx = 0; idx < N - 1; ++idx) { _log(c_str[idx]); } }
template<typename T, size_t N>void _log(T(&c_arr_ref)[N]) {
	_log("{");
	for (size_t idx = 0; idx < N - 1; ++idx) {
		_log("[", idx, "]:", c_arr_ref[idx], ",");
	}
	_log("[", N - 1, "]:", c_arr_ref[N - 1], "}");
}
template<typename T,typename...Ts>
void _log(T const & first,Ts const & ... values) {
	_log(first); 
	if constexpr (sizeof...(Ts) > 0) { _log(values...); }
}
struct _Logger {};
template<typename T>
_Logger const& operator << (_Logger const& logger, T const& var) {
	_log(var); return logger;
}
NAMESPACE_END // impl_log
// 打印多个参数
template<typename T, typename...Ts>void log(T const& first, Ts const & ... values) 
{ impl_log::_log(first,values...); }
// 打印多个参数并换行
template<typename...Ts>void logln(Ts const & ... values){
	if constexpr(sizeof...(Ts)>0){ impl_log::_log(values...); }
	impl_log::_log('\n');
}
// 采用<<运算打印多个参数
impl_log::_Logger const logger;
// 换行符
static constexpr auto endln = '\n';
NAMESPACE_END // fg_tmp
#endif //!__LOG_HPP__