#pragma once
#include<new.h>
#include "typetraits.h"
#include "Iterator.h"

template<class T1,class T2>
inline void construct(T1 *p,const T2& value)
{
	new(p) T1(value);  //placement new ��p��ִ��T1::T1(value)
}

template<class T>
inline void destroy(T *p)
{
	p->~T();
}

template<class ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last)
{
	_destroy(first, last, value_type(first));
}

template<class ForwardIterator,class T>
inline void _destroy(ForwardIterator first, ForwardIterator last, T*)
{
	typedef typename type_traits<T>::has_trivial_destructor trivial_destructor;//����value_type�ж��Ƿ�POD
	_destroy_aux(first, last, trivial_destructor());	//��ʱ�����ڱ����ڸ������ؾ���ѡ����ִ��
}
//false_type��ִ����������
template<class ForwardIterator>
inline void _destroy_aux(ForwardIterator first, ForwardIterator last, false_type)
{
	for (; first < last; ++first)
	{
		destroy(&*first);
	}
}

//true_type
template<class ForwardIterator>
inline void _destroy_aux(ForwardIterator first, ForwardIterator last, true_type)
{
}
