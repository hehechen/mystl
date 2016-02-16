#pragma once
#include"Iterator.h"
#include"typetraits.h"
#include <stdio.h>
using namespace Chenstl;
//最大堆
//写成多个函数是为了萃取出所需的类型
namespace Chenstl {
	template<class RandomAccessIterator>
	inline void push_heap(RandomAccessIterator first, RandomAccessIterator last)
	{
		push_heap_aux(first, last, distance_type(first), value_type(first));
	}

	template<class RandomAccessIterator, class Distance, class T>
	inline void push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*)
	{
		_push_heap(first, Distance((last - first) - 1), Distance(0), T(*(last - 1)));
	}

	template<class RandomAccessIterator, class Distance, class T>
	inline void _push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value)
	{//新插入的点不断跟父节点比较，比父节点大则交换
		Distance parent = (holeIndex - 1) / 2;	//求得父节点
		while (holeIndex > topIndex && *(first + holeIndex) > *(first + parent))
		{//尚未达到顶端，且父节点比插入的点小
			*(first + parent) = value;
			holeIndex = parent;
			parent = (holeIndex - 1) / 2;
		}
		*(first + holeIndex) = value;	//最后再填入新值
	}

	template<class RandomAccessIterator>
	inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last)
	{
		pop_heap_aux(first, last, last - 1, distance_type(first), value_type(first));
	}

	template<class RandomAccessIterator, class Distance, class T>
	inline void pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result,
		Distance*, T*)
	{
		swap(*first, *result);
		adjust_heap(first, Distance(0), Distance(last - first-1), T(*first));
	}

	template<class RandomAccessIterator, class Distance, class T>
	//从holeIndex开始调整，长度为len(即0到len-1)，i子节点为2*i+1,2*i+2,value为原来的尾值，pop后放到holeIndex
	inline void adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value)
	{
		Distance nextIndex = 2 * holeIndex + 1;
		while (nextIndex < len)
		{
			if (nextIndex < len-1 && *(first + nextIndex) < *(first + (nextIndex + 1)))
				++nextIndex;//取较大的子节点
			if (*(first + nextIndex) < *(first + holeIndex))
				break;	//这个堆从holeIndex之后符合定义，不必调整
			*(first + holeIndex) = *(first + nextIndex);
			holeIndex = nextIndex;
			nextIndex = 2 * holeIndex + 1;//较大的子节点
			*(first + holeIndex) = value;
		}
	}

	//从小到大排序
	template<class RandomAccessIterator>
	inline void sort_heap(RandomAccessIterator first, RandomAccessIterator last)
	{//不断执行pop，将最大值置于last
		while (last-1 > first)
		{
			pop_heap(first, last--);
		}
	}

	template<class RandomAccessIterator>
	inline void make_heap(RandomAccessIterator first, RandomAccessIterator last)
	{
		_make_heap(first, last, distance_type(first), value_type(first));
	}

	template<class RandomAccessIterator, class Distance, class T>
	inline void _make_heap(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*)
	{
		Distance len = last - first;
		for (Distance holeIndex = len / 2 - 1; holeIndex >= 0; --holeIndex)
		{//叶子节点是合法的，找出第一个需重排的子树的头部
			adjust_heap(first, holeIndex, len, T(*(first + holeIndex)));
		}
	}
}


