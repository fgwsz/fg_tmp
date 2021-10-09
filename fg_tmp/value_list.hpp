#ifndef    __VALUE_LIST_HPP__
#define    __VALUE_LIST_HPP__
#include "namespace_macro.h"
NAMESPACE_BEGIN(fg_tmp)
// 编译期值列表
template<auto...values>struct ValueList 
{ static constexpr auto size = sizeof...(values); };
NAMESPACE_BEGIN(impl_value_list)
template<typename Type>struct _Is_ValueType {
	static constexpr auto value = false;
};
template<auto...values>struct _Is_ValueType<ValueList<values...>> {
	static constexpr auto value = true;
};
NAMESPACE_END
// 判断 是否为 编译期值列表
template<typename Type>
static constexpr auto is_value_list = impl_value_list::_Is_ValueType<Type>::value;
NAMESPACE_END // fg_tmp
#endif // !__VALUE_LIST_HPP__