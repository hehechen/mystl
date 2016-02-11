#pragma once
#include"Iterator.h"
#include"typetraits.h"

//最大堆
//写成多个函数是为了萃取出所需的类型
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
{//新插入的点不断跟父节点比较，比父节点大则交换
	Distance parent = (holeIndex - 1) / 2;	//求得父节点
	while (holeIndex > topIndex && *(first + holeIndex) > *(first + parent)
	{//尚未达到顶端，且父节点比插入的点小
		*(first + parent) = value;
		holeIndex = parent;
		parent = (holeIndex - 1) / 2;
	}
	*(first + holeIndex) = value;	//最后再填入新值
}
