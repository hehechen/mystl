#ifndef _VECTOR_H
#define _VECTOR_H
#include "Iterator.h"
#include "default_alloc.h"
#include "uninitialized.h"
namespace Chenstl {

	//倒序复制 将从first到last的数据复制到以result结尾的区间，左闭右开
	template<class BidirectionalIterator>
	inline BidirectionalIterator copy_backward(BidirectionalIterator first, BidirectionalIterator last,
		BidirectionalIterator result)
	{
		while (first != last)	--result = --last;
		return result;
	}


	template<class T,class Alloc = default_alloc>
	class vector
	{
	public:
		typedef T value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type* iterator;
		typedef const value_type* const_iterator;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		iterator begin() { return start; }
		iterator end() { return finish; }
		size_type size() { return size_type(end() - begin()); }

		typedef simple_alloc<value_type, Alloc>	data_allocator;
		vector(size_t n, const T& value) { fill_initialize(n, value); }	//个数及初值
		~vector() 
		{ 
			destroy(start, finish); 
			data_allocator::deallocate(start, end_of_storage - start); 
		}
		void push_back(const T& value)
		{
			if (finish != end_of_storage)	//尚有裕量
			{
				construct(finish, value);
				++finish;
			}
			else
				insert_aux(end(), value);
		}
		void pop_back()
		{
			--finish;
			destroy(finish);
		}

		iterator erase(iterator first, iterator last)
		{
			 iterator i = copy(last, finish, first);
			 destroy(i, finish);		
			 finish = i;
			 return first;
		}

		void insert(iterator position, size_type n, const T& value)
		{
			if (n != 0)
			{
				if (size_type(end_of_storage - finish) >= n)
				{//备用空间足够
					iterator old_finish = finish;
					finish = uninitialized_copy(position, finish, position + n);
					uninitialized_fill_n(position, n, value);
				}
				else
				{
					size_type old_size = size();
					size_type len = old_size + max(old_size, n);
					//新创建vector并填充数据
					iterator new_start = data_allocator::allocate(len);
					iterator new_finish = new_start;
					new_finish = uninitialized_copy(start, position, new_start);
					new_finish = uninitialized_fill_n(new_finish, n, value);
					new_finish = uninitialized_copy(position, finish, new_finish);
					//销毁原来的vector
					destroy(start, finish);
					//调整标记
					start = new_start;
					finish = new_finish;
					end_of_storage = new_start + len;
				}
			}
		}

	private:
		iterator start,finish;
		iterator end_of_storage;	//storage跟实际的大小不一样
		void fill_initialize(size_t n, const T& value)	//填充并初始化
		{
			start = allocate_and_fill(n, value);
			finish = start + n;
			end_of_storage = finish;
		}
		iterator allocate_and_fill(size_t n, const T& value)
		{
			iterator result = data_allocator::allocate(n);
			uninitialized_fill_n(result, n, value);
			return result;
		}
		//如果空间足够就直接插入，不够就重新配置两倍的空间
		void insert_aux(iterator position, const T& value)
		{
			if (finish != end_of_storage)
			{
				construct(finish, *(finish - 1));
				T x = value;
				++finish;
				copy_backward(position, finish - 2, finish - 1);
				*position = x;
			}
			else
			{
				const size_t old_size = size();
				size_t n = old_size != 0 ? 2 * old_size : 1;
				iterator new_start = data_allocator::allocate(n);
				iterator new_finish = uninitialized_copy(start, position, new_start);
				*new_finish = value;			//插入
				++new_finish;
				uninitialized_copy(position, finish, new_finish);
				start = new_start;
				finish = new_finish;
			}
		}
	};
}

#endif // !_VECTOR_H

