#ifndef    __INDEX_SEQ_HPP__
#define    __INDEX_SEQ_HPP__
#include "namespace_macro.h"
NAMESPACE_BEGIN(fg_tmp)
// 大小类型(无符号整数)
using size_t = unsigned long long;
// 编译期下标序列
template<size_t...values>struct IndexSeq{static constexpr auto size = sizeof...(values);};
NAMESPACE_BEGIN(impl_index_seq)
template<typename index_seq, size_t value>struct _IndexSeq_PushBack;
template<size_t...values, size_t value>struct _IndexSeq_PushBack<IndexSeq<values...>,value> 
{ using type = IndexSeq<values..., value>; };
template<size_t size>struct _IndexSeq_Make 
{ using type = typename _IndexSeq_PushBack<typename _IndexSeq_Make<size -1>::type,size-1>::type;};
template<>struct _IndexSeq_Make<1> { using type = IndexSeq<0>; };
template<>struct _IndexSeq_Make<0> { using type = IndexSeq<>; };
NAMESPACE_END // impl_index_seq
// 制造编译期下标序列
// 举例： 输入:IndexSeq_Make<10> 输出:IndexSeq<0,1,2,3,4,5,6,7,8,9>
template<size_t size>using IndexSeq_Make = typename impl_index_seq::_IndexSeq_Make<size>::type;
NAMESPACE_END // fg_tmp
#endif // !__INDEX_SEQ_HPP__