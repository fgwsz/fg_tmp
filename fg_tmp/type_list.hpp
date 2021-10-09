#ifndef    __TYPE_LIST_HPP__
#define    __TYPE_LIST_HPP__
#include "namespace_macro.h"
#include "static_if.hpp"
NAMESPACE_BEGIN(fg_tmp)
// 编译期类型列表
template<typename...Types>struct TypeList {
	static constexpr auto size = sizeof...(Types);
};
NAMESPACE_BEGIN(impl_type_list)
template<typename Type>struct _Is_TypeList {
	static constexpr auto value = false;
};
template<typename...Types>struct _Is_TypeList<TypeList<Types...>> {
	static constexpr auto value = true;
};
template<typename...type_list_package>struct _TypeList_Concat;
template<typename...Types, typename...type_list_package>
struct _TypeList_Concat<TypeList<Types...>, type_list_package...> {
	using type = typename _TypeList_Concat<
		TypeList<Types...>
		, If_t<sizeof...(type_list_package)
		, typename _TypeList_Concat<type_list_package...>::type
		, TypeList<>
		>
	>::type;
};
template<typename...Types1, typename...Types2>
struct _TypeList_Concat<TypeList<Types1...>, TypeList<Types2...>> {
	using type = TypeList<Types1..., Types2...>;
};
template<>struct _TypeList_Concat<> { using type = TypeList<>; };
template<typename type_list>struct _TypeList_Front;
template<typename Type, typename...Types>
struct _TypeList_Front<TypeList<Type, Types...>> {
	using type = Type;
};
template<> struct _TypeList_Front<TypeList<>> {
	using type = void;
};
template<typename type_list>struct _TypeList_Back;
template<typename Type, typename...Types>
struct _TypeList_Back<TypeList<Type, Types...>> {
	using type = If_t < sizeof...(Types)
		, typename _TypeList_Back <TypeList<Types...>>::type
		, Type
	>;
};
template<> struct _TypeList_Back<TypeList<>> {
	using type = void;
};
template<typename type_list, typename Type>struct _TypeList_PushFront;
template<typename...Types, typename Type>
struct _TypeList_PushFront <TypeList<Types...>, Type> {
	using type = TypeList<Type, Types...>;
};
template<typename type_list, typename Type>struct _TypeList_PushBack;
template<typename...Types, typename Type>
struct _TypeList_PushBack <TypeList<Types...>, Type> {
	using type = TypeList<Types..., Type>;
};
template<typename type_list, size_t index>struct _TypeList_Get;
template<typename Type, typename...Types, size_t index>
struct _TypeList_Get<TypeList<Type, Types...>, index> {
private:
	static constexpr auto is_idx_useful = index + 1 <= TypeList<Type, Types...>::size;
public:
	using type = If_t<is_idx_useful
		, If_t< index != 0
		, typename _TypeList_Get<TypeList<Types...>, index != 0 ? (index - 1) : 0 >::type
		, Type
		>
		, void
	>;
};
template<size_t index>struct _TypeList_Get<TypeList<>, index> { using type = void; };
template<typename type_list, size_t index, typename Type>struct _TypeList_Set;
template<typename Type, typename...Types, size_t index, typename NewType>
struct _TypeList_Set<TypeList<Type, Types...>, index, NewType> {
private:
	static constexpr auto is_idx_useful = index + 1 <= TypeList<Type, Types...>::size;
public:
	using type = If_t < is_idx_useful
		, If_t< index != 0
		, typename _TypeList_Concat< TypeList<Type>
		, typename _TypeList_Set<TypeList<Types...>, index != 0 ? (index - 1) : 0, NewType>::type
		>::type
		, TypeList<NewType, Types...>
		>
		, TypeList<Type, Types...>
	>;
};
template<size_t index, typename Type>
struct _TypeList_Set<TypeList<>, index, Type> { using type = TypeList<>; };
NAMESPACE_END // impl_type_list
// 判断 是否为 编译期类型列表
template<typename Type>
static constexpr auto  is_type_list = impl_type_list::_Is_TypeList<Type>::value;
// 首尾相连多个编译期类型列表为一个编译期类型列表
template<typename...type_list_package>
using TypeList_Concat = typename impl_type_list::_TypeList_Concat<type_list_package...>::type;
// 编译期类型列表：得到列表中第一个类型
template<typename type_list>
using TypeList_Front = typename impl_type_list::_TypeList_Front<type_list>::type;
// 编译期类型列表：得到列表中最后一个类型
template<typename type_list>
using TypeList_Back = typename impl_type_list::_TypeList_Back<type_list>::type;
// 编译期类型列表：头部插入一个类型
template<typename type_list, typename Type>
using TypeList_PushFront = typename impl_type_list::_TypeList_PushFront<type_list, Type>::type;
// 编译期类型列表：尾部插入一个类型
template<typename type_list, typename Type>
using TypeList_PushBack = typename impl_type_list::_TypeList_PushBack<type_list, Type>::type;
// 编译期类型列表：得到下标对应的类型
template<typename type_list, size_t index>
using TypeList_Get = typename impl_type_list::_TypeList_Get<type_list, index>::type;
// 编译期类型列表: 设置下标对应的类型
template<typename type_list, size_t index, typename Type>
using TypeList_Set = typename impl_type_list::_TypeList_Set<type_list, index, Type>::type;
NAMESPACE_END // fg_tmp
#endif // !__TYPE_LIST_HPP__
