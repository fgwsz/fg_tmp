#ifndef    __NAMESPACE_MACRO_H__
#define    __NAMESPACE_MACRO_H__
/* 宏函数 NAMESPACE_BEGIN(__NAMESPACE_NAME__)
参数一:__NAMESPACE_NAME__:命名空间 的名称
作用：命名空间 __NAMESPACE_NAME__ 开始 */
#define NAMESPACE_BEGIN(__NAMESPACE_NAME__) namespace __NAMESPACE_NAME__{
/* 宏函数 NAMESPACE_END
作用：最近邻的 一个 命名空间 结束 */
#define NAMESPACE_END }
#endif // !__NAMESPACE_MACRO_H__