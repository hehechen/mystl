#pragma once
#include"Iterator.h"
#include"typetraits.h"

//����
//д�ɶ��������Ϊ����ȡ�����������
template<class RandomAccessIterator>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last)
{
	push_heap_aux(first, last, distance_type(first), value_type(first));
}

template<class RandomAccessIterator,class Distance,class T>
inline void push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*)
{
	_push_heap(first, Distance((last - first) - 1), Distance(0), T(*(last - 1)));
}

template<class RandomAccessIterator,class Distance,class T>
inline void _push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value)
{//�²���ĵ㲻�ϸ����ڵ�Ƚϣ��ȸ��ڵ���򽻻�
	Distance parent = (holeIndex - 1) / 2;	//��ø��ڵ�
	while (holeIndex > topIndex && *(first + holeIndex) > *(first + parent)
	{//��δ�ﵽ���ˣ��Ҹ��ڵ�Ȳ���ĵ�С
		*(first + parent) = value;
		holeIndex = parent;
		parent = (holeIndex - 1) / 2;
	}
	*(first + holeIndex) = value;	//�����������ֵ
}
