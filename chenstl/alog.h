#pragma once
#include "typetraits.h"

/***********copy**********************/
template<class InputIterator,class OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
{
	return _copy(first, last, result, value_type(first));
}
template<class InputIterator, class OutputIterator,class T>
inline OutputIterator _copy(InputIterator first, InputIterator last, OutputIterator result, T*)
{
	typedef typename type_traits<T>::is_POD_type is_POD;
	return _copy_aux(first, last, result, is_POD());
}

template<class InputIterator, class OutputIterator>
inline OutputIterator _copy_aux(InputIterator first, InputIterator last, OutputIterator result, false_type)
{
	OutputIterator cur = result;
	for (; first < last; ++first, ++cur)
		*cur = first;
	return cur;

}
template<class InputIterator, class OutputIterator>
inline OutputIterator _copy_aux(InputIterator first, InputIterator last, OutputIterator result, true_type)
{
	OutputIterator cur = result;
	for (; first < last; ++first, ++cur)
		*cur = *first;
	return cur;
}
/***********copy**********************/

/*****max********/
inline size_t max(const size_t a, const size_t b)
{
	return a > b ? a : b;
}

template <class T>
inline void _swap(T& a, T& b) {
	T tmp = a;
	a = b;
	b = tmp;
}
