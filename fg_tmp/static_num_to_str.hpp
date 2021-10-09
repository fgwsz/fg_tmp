#ifndef    __STATIC_NUM_TO_STR_HPP__
#define    __STATIC_NUM_TO_STR_HPP__
#include "namespace_macro.h"
#include "static_string.hpp"
NAMESPACE_BEGIN(fg_tmp)
// 编译期数值类型
using number_t = double;
NAMESPACE_BEGIN(impl_static_num_to_str)
using integer_t = long long;
static constexpr number_t _abs(number_t number) { return number >= 0 ? number : -number; }
static constexpr number_t _power(number_t number,integer_t exponent) {
	if (exponent == 0) { return 1; }
	else if (exponent > 0) { return number * _power(number, exponent - 1); }
	else { return static_cast<number_t>(1) / _power(number, -exponent); }
}
static constexpr integer_t _bitSize(integer_t integer,integer_t counter = 0) {
	if (static_cast<integer_t>(_abs(integer)) / static_cast<integer_t>(_power(10,counter))) 
	{ return _bitSize(integer, counter + 1); }
	return counter;
}
template<integer_t value>struct _IntToStr {
private:
	static constexpr integer_t val_abs = static_cast<integer_t>(_abs(value));
	static constexpr integer_t bit_size = _bitSize(val_abs);
	static constexpr integer_t bit_num = val_abs / static_cast<integer_t>(_power(10, bit_size - 1));
	static constexpr integer_t next_val = val_abs % static_cast<integer_t>(_power(10, bit_size - 1));
	static constexpr integer_t next_bit_size = _bitSize(next_val);
	using val_abs_ret = StaticString_Concat<
		typename _IntToStr<bit_num>::type,
		If_t<next_val != 0,
		StaticString_Concat<StaticString_Make<bit_size - 1 - next_bit_size, '0'>
		, typename _IntToStr<next_val>::type>
		, StaticString_Make<bit_size - 1, '0'>
		>
	>;
public:
	using type = If_t < value >= 0,
		val_abs_ret,
		StaticString_PushFront<val_abs_ret, '-'>
	>;
};
template<>struct _IntToStr<0> { using type = StaticString<'0'>; };
template<>struct _IntToStr<1> { using type = StaticString<'1'>; };
template<>struct _IntToStr<2> { using type = StaticString<'2'>; };
template<>struct _IntToStr<3> { using type = StaticString<'3'>; };
template<>struct _IntToStr<4> { using type = StaticString<'4'>; };
template<>struct _IntToStr<5> { using type = StaticString<'5'>; };
template<>struct _IntToStr<6> { using type = StaticString<'6'>; };
template<>struct _IntToStr<7> { using type = StaticString<'7'>; };
template<>struct _IntToStr<8> { using type = StaticString<'8'>; };
template<>struct _IntToStr<9> { using type = StaticString<'9'>; };
template<number_t value, bool can_run = true,bool first_run_flag = true>struct _NumToStr {
private:
	static constexpr number_t val_abs = _abs(value);
	static constexpr integer_t uint_part = static_cast<integer_t>(val_abs);
	static constexpr number_t decimal_part = val_abs - static_cast<number_t>(uint_part);
	using head = If_t< first_run_flag&& decimal_part!=static_cast<number_t>(0),
		StaticString_PushBack<typename _IntToStr<static_cast<integer_t>(value)>::type, '.'>,
		typename _IntToStr<uint_part>::type
	>;
public:
	using type = StaticString_Concat<head,
		typename _NumToStr< static_cast<number_t>(10) * decimal_part,
		decimal_part != static_cast<number_t>(0), false>::type
	>;
};
template<number_t value>struct _NumToStr<value, false, false> {
	using type = StaticString<>;
};
NAMESPACE_END // impl_static_num_to_str
// 将编译期数值 转化为 编译期字符串
template<number_t value>
using NumToStr = typename impl_static_num_to_str::_NumToStr<value>::type;
NAMESPACE_END // fg_tmp
#endif // !__STATIC_NUM_TO_STR_HPP__
