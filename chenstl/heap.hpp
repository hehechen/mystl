#pragma once
#include"Iterator.h"
#include"typetraits.h"
#include <stdio.h>
using namespace Chenstl;
//����
//д�ɶ��������Ϊ����ȡ�����������
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
	{//�²���ĵ㲻�ϸ����ڵ�Ƚϣ��ȸ��ڵ���򽻻�
		Distance parent = (holeIndex - 1) / 2;	//��ø��ڵ�
		while (holeIndex > topIndex && *(first + holeIndex) > *(first + parent))
		{//��δ�ﵽ���ˣ��Ҹ��ڵ�Ȳ���ĵ�С
			*(first + parent) = value;
			holeIndex = parent;
			parent = (holeIndex - 1) / 2;
		}
		*(first + holeIndex) = value;	//�����������ֵ
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
	//��holeIndex��ʼ����������Ϊlen(��0��len-1)��i�ӽڵ�Ϊ2*i+1,2*i+2,valueΪԭ����βֵ��pop��ŵ�holeIndex
	inline void adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value)
	{
		Distance nextIndex = 2 * holeIndex + 1;
		while (nextIndex < len)
		{
			if (nextIndex < len-1 && *(first + nextIndex) < *(first + (nextIndex + 1)))
				++nextIndex;//ȡ�ϴ���ӽڵ�
			if (*(first + nextIndex) < *(first + holeIndex))
				break;	//����Ѵ�holeIndex֮����϶��壬���ص���
			*(first + holeIndex) = *(first + nextIndex);
			holeIndex = nextIndex;
			nextIndex = 2 * holeIndex + 1;//�ϴ���ӽڵ�
			*(first + holeIndex) = value;
		}
	}

	//��С��������
	template<class RandomAccessIterator>
	inline void sort_heap(RandomAccessIterator first, RandomAccessIterator last)
	{//����ִ��pop�������ֵ����last
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
		{//Ҷ�ӽڵ��ǺϷ��ģ��ҳ���һ�������ŵ�������ͷ��
			adjust_heap(first, holeIndex, len, T(*(first + holeIndex)));
		}
	}
}


