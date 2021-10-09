#ifndef    __STATIC_IF_HPP__
#define    __STATIC_IF_HPP__
#include "namespace_macro.h"
NAMESPACE_BEGIN(fg_tmp)
NAMESPACE_BEGIN(impl_static_if)
template<bool bool_expr, typename TrueRet, typename FalseRet>struct _If_RetType {
	using type = TrueRet;
};
template<typename TrueRet, typename FalseRet>struct _If_RetType<false, TrueRet, FalseRet> {
	using type = FalseRet;
};
template<bool bool_expr, auto true_ret, auto false_ret>struct _If_RetValue {
private:
	struct TrueType { static constexpr auto value = true_ret; };
	struct FalseType { static constexpr auto value = false_ret; };
	using RetType = typename _If_RetType<bool_expr, TrueType, FalseType>::type;
public:
	static constexpr auto value = RetType::value;
};
NAMESPACE_END // impl_static_if
// 编译期 if 输出为两个数据类型中的一个
template<bool bool_expr, typename TrueRet, typename FalseRet>
using If_t = typename impl_static_if::_If_RetType<bool_expr, TrueRet, FalseRet>::type;
// 编译期 if 输出为两个编译期值中的一个
template<bool bool_expr, auto true_ret, auto false_ret>
static constexpr auto If_v = impl_static_if::_If_RetValue<bool_expr, true_ret, false_ret>::value;
NAMESPACE_END // fg_tmp
#endif // !__STATIC_IF_HPP__