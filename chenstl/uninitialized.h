#pragma once
#include "construct.h"
#include "typetraits.h"
#include "alog.h"
/******uninitialized_fill_n*******/
template<class ForwardIterator,class Size,class T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& value)
{
	return _uninitialized_fill_n(first, n, value, value_type(first));
}

template<class ForwardIterator,class Size,class T,class VT>
inline ForwardIterator _uninitialized_fill_n(ForwardIterator first, Size n, const T& value, VT*)
{
	typedef typename type_traits<VT>::is_POD_type is_POD_type;
	return _uninitialized_aux_fill_n(first, n, value, is_POD_type());
}

template<class ForwardIterator,class Size, class T>
inline ForwardIterator _uninitialized_aux_fill_n(ForwardIterator firt, Size n, const T& value, false_type)
{
	ForwardIterator cur = first;
	for (; n > 0; --n, ++cur)
	{
		construct(&*cur, value);
	}
	return cur;
}

template<class ForwardIterator, class Size, class T>
inline ForwardIterator _uninitialized_aux_fill_n(ForwardIterator first, Size n, const T& value, true_type)
{
	//TODO:fill_n
	for (; n > 0; --n, ++first)
		*first = value;
	return first;
}
/******uninitialized_fill_n*******/

/********uninitialized_copy*********/
//将从first到last的数据复制到result
template<class ForwardIterator,class InputIterator>
inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
{
	return _uninitialized_copy(first, last, result, value_type(first));
}

template<class ForwardIterator, class InputIterator,class T>
inline ForwardIterator _uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result,T*)
{
	typedef typename type_traits<T>::is_POD_type is_POD_type;
	return _uninitialized_copy_aux(first, last, result, is_POD_type());
}

template<class ForwardIterator, class InputIterator>
inline ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result,
												false_type)
{
	ForwardIterator cur = result;
	for (; first < last; ++first,++cur)
		construct(&*cur, *first);
	return cur;
}
template<class ForwardIterator, class InputIterator>
inline ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result,
												true_type)
{
	return copy(first, last, result);
}