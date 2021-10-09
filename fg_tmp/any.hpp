#ifndef __ANY_HPP__
#define __ANY_HPP__
#include "namespace_macro.h"
#include "type_info.hpp"
#include <cassert>
#include <type_traits>
#include "static_reflection.hpp"
NAMESPACE_BEGIN(fg_tmp)
// 万能数据类型
struct Any {
private:
	enum class Tag   { VOID,VAL, PTR, REF };
	enum class Alloc { VOID,NEW,MALLOC };
	Tag      tag_;
	size_t   offset_;
	void*    data_;
	Alloc    alloc_;
	string_t name_;
	void init() { 
		tag_ = Tag::VOID;
		offset_ = 0;
		data_ = nullptr; 
		alloc_ = Alloc::VOID;
		name_ = {};
	}
public:
	// 销毁存储的数据
	void clear() { 
		if (tag_ == Tag::VAL && data_) { 
			if (alloc_ == Alloc::NEW) { delete(data_); }
			else if (alloc_ == Alloc::MALLOC) { free(data_); }
		} 
		init(); 
	}
	~Any() { clear(); }
private:
	template<typename T>void setVal(T val){
		using ValType = ::std::decay_t<T>;
		clear();
		tag_ = Tag::VAL;
		offset_ = sizeof(ValType);
		name_ = TypeInfo<ValType>::name();
		if constexpr (::std::is_copy_constructible_v<ValType>) {
			data_ = (void*)new ValType(val);
			alloc_ = Alloc::NEW;
			return;
		}
		data_ = malloc(offset_);
        assert(data_); // "::fg_tmp::Any开辟堆区内存失败"
        memcpy(data_, &val, offset_);
		alloc_ = Alloc::MALLOC;
	}
	template<typename T>void setPtr(T ptr){
		clear();
		tag_ = Tag::PTR;
		offset_ = sizeof(ptr);
		data_ = (void*)(ptr);
		name_ = TYPE_INFO(ptr)::name();
	}
	template<typename T>void setRef(T ref){
		clear();
		tag_ = Tag::REF;
		offset_ = sizeof(ref);
		data_ = (void*)(&ref);
		name_ = TYPE_INFO(ref)::name();
	}
	void setAny(Any const & any) {
		if (this == &any) { return; }
		clear();
		tag_ = any.tag_;
		offset_ = any.offset_;
		name_ = any.name_;
		if (any.tag_ == Tag::VAL && any.data_) {
			data_ = malloc(offset_);
			assert(data_); // "::fg_tmp::Any开辟堆区内存失败"
			memcpy(data_, any.data_, offset_);
			alloc_ = Alloc::MALLOC;
		}
		else { data_ = any.data_; }
	}
public:
	// 无参构造
	Any() { init(); }
private:
	template<typename T>struct _IsAny { static constexpr auto value = false; };
	template<>struct _IsAny<Any> { static constexpr auto value = true; };
	template<typename T>struct _IsAny<T const> { 
		static constexpr auto value = _IsAny<T>::value; 
	};
	template<typename T>struct _IsAny<T volatile> { 
		static constexpr auto value = _IsAny<T>::value;
	};
	template<typename T>struct _IsAny<T const volatile> { 
		static constexpr auto value = _IsAny<T>::value;
	};
	template<typename T>struct _IsAny<T&> {
		static constexpr auto value = _IsAny<T>::value;
	};
	template<typename T>struct _IsAny<T&&> {
		static constexpr auto value = _IsAny<T>::value;
	};
	template<typename T>static constexpr auto is_any = _IsAny<T>::value;
	template<typename T>struct _RemoveRef { using type = T; };
	template<typename T>struct _RemoveRef<T&> { using type = T; };
	template<typename T>using RemoveRef = typename _RemoveRef<T>::type;
	template<typename T>struct _IsPtr { static constexpr auto value = false; };
	template<typename T>struct _IsPtr <T*> { static constexpr auto value = true; };
	template<typename T>struct _IsPtr <T const> { 
		static constexpr auto value = _IsPtr<T>::value; 
	};
	template<typename T>struct _IsPtr <T volatile> { 
		static constexpr auto value = _IsPtr<T>::value;
	};
	template<typename T>struct _IsPtr <T const volatile> { 
		static constexpr auto value = _IsPtr<T>::value;
	};
	template<typename T>struct _IsPtr <T&> { 
		static constexpr auto value = _IsPtr<T>::value;
	};
	template<typename T>struct _IsPtr <T&&> {
		static constexpr auto value = _IsPtr<T>::value;
	};
	template<typename T>static constexpr auto is_ptr = _IsPtr<T>::value;
public:
	// 赋值数据// 可赋值类型 如下
	// 1. 普通值类型
	// 2. 普通指针类型
	// 3. 数组引用类型
	// 4. 函数指针类型
	// 5. Any 类型
	Any& operator=(Any const& any) { setAny(any); return *this; }
	template<typename T>Any& operator=(T&& data) { 
		if constexpr (is_any<T>) {
			setAny(data);
		}
		if constexpr (!is_any<T> && is_ptr<T>) {
			setPtr(data);
		}
		if constexpr (!is_any<T> && !is_ptr<T>) {
			setVal(data);
		}
		return *this; 
	}
	template<typename T, size_t N>
	Any& operator=(T(&ref)[N]) { setRef<T(&)[N]>(ref); return *this;   }
	template<typename T, typename ...Ts>
	Any& operator=(T(*ptr)(Ts...)) { setPtr<T(*)(Ts...)>(ptr); return *this;}
	template<typename T, typename ...Ts>
	Any& operator=(T(*ptr)(Ts..., ...)) { setPtr<T(*)(Ts..., ...)>(ptr); return *this;}
	// 采用数据构造
	Any(Any const& any) { init(); setAny(any); }
	template<typename T>Any(T&& data) { init(); this->operator=(data); }
	template<typename T, size_t N>Any(T(&ref)[N]) { init(); this->operator=(ref); }
	template<typename T, typename ...Ts>Any(T(*ptr)(Ts...)) { init(); this->operator=(ptr); }
	template<typename T, typename ...Ts>Any(T(*ptr)(Ts..., ...)) { init(); this->operator=(ptr); }
	// 得到存储数据的真实类型名(字符串)
	string_t const& name()const { return name_; }
	// 得到存储的数据,可获取类型(模板参数)如下
	// 1. 普通值类型
	// 2. 普通指针类型
	// 3. 数组引用类型
	// 4. 函数指针类型
	template<typename T>T data() const {
		if constexpr (is_ptr<T>) {
			if(tag_ == Tag::PTR) { return reinterpret_cast<T>(data_);   }
		}
		switch (tag_) {
		case Tag::VAL: 
		case Tag::REF: { return *reinterpret_cast<RemoveRef<T>*>(data_); }
		}
		return *static_cast<RemoveRef<T>*>(nullptr);
	}
	// 得到存储数据的内存大小
	size_t const& size()const { return offset_; }
	// 判断是否含有存储数据
	bool has_data() { return !name_.empty(); }
private:
	template<typename T>void _swap(T& left, T& right) {
		auto temp = left;
		left = right;
		right = temp;
	}
public:
	// 交换存储的数据
	Any& swap(Any& any) {
		if (this == &any) { return *this; }
		_swap(tag_, any.tag_);
		_swap(offset_, any.offset_);
		_swap(data_, any.data_);
		_swap(alloc_, any.alloc_);
		_swap(name_, any.name_);
		return *this;
	}
};
REGIST_TYPE("Any",Any)
NAMESPACE_END // fg_tmp
#endif //!__ANY_HPP__