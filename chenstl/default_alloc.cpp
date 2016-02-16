#include "default_alloc.h"
#include "malloc_alloc.h"
using namespace Chenstl;

default_alloc::obj *default_alloc::free_list[NFREELISTS]
= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
char *default_alloc::start_free = 0;	//内存池的起始位置
char *default_alloc::end_free = 0;		//内存池的终止位置
size_t default_alloc::heap_size = 0;

void *default_alloc::allocate(size_t n)
{
	obj *result = 0;
	obj **my_free_list = 0;
	if (n > MAX_BYTES)
		return malloc_alloc::allocate(n);
	//寻找free lists中合适的一个				
	my_free_list = free_list + FREELIST_INDEX(n);
	result = *my_free_list;
	if(0 == result)
	{//没有找到可用的freelist，从内存池里取出空间
		return refill(ROUND_UP(n));
	}
	//调整freelist
	*my_free_list = result->next;
	return result;
}

void default_alloc::deallocate(void *p, size_t n)
{
	if (n > 128)
	{
		malloc_alloc::decllocate(p);
		return;
	}
	obj *q = (obj *)p;
	obj **my_free_list = free_list + FREELIST_INDEX(n);
	q->next = *my_free_list;
	*my_free_list = q;
}
//返回一个大小为n的对象，并可能加入大小为n的其他区块到freelist
//在ANSI c中，void *不允许进行加减操作，所以chunk用char *
void *default_alloc::refill(size_t n)
{
	int objs = 20;
	char *chunk = chunk_alloc(n, objs);
	
	obj *next = nullptr, *current = nullptr;
	obj *result = nullptr;
	obj **my_free_list = nullptr;
	if (1 == objs)	//只取出一个区块
		return chunk;
	my_free_list = free_list + FREELIST_INDEX(n);
	result = (obj *)chunk;	//这一块返回给客户端
	//将freellist指向分配的区域
	*my_free_list = next = (obj *)(chunk + n);
	for (int i = 1;; i++)
	{
		current = next;
		next = (obj *)((char *)next + n);	//这里注意不能直接用next+n
		if (i == objs - 1)
		{
			current->next = nullptr;
			break;
		}
		else
			current->next = next;
	}
	return result;	
}

char *default_alloc::chunk_alloc(size_t size, int &nobjs)
{
	char *result = 0;
	size_t total_bytes = size*nobjs;
	size_t bytes_left = end_free - start_free;	//内存池剩余空间
	if (bytes_left >= total_bytes)
	{//内存池足够提供所需内存
		result = start_free;
		start_free += total_bytes;
		return result;
	}
	else if (bytes_left >= size)
	{//内存池足够供应一个以上的区块
		nobjs = bytes_left / size;
		total_bytes = nobjs * size;
		result = start_free;
		start_free += total_bytes;
		return result;
	}
	else
	{//内存池一块区块也供应不了
		size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);;
		if (bytes_left>0)
		{//将内存池的零头分配给合适的freelist
			obj **my_free_list = free_list + FREELIST_INDEX(bytes_left);
			((obj *)start_free)->next = *my_free_list;
			*my_free_list = (obj *)start_free;
		}
		start_free = (char *)malloc(bytes_to_get);
		if (!start_free) 
		{//系统堆内存不足，寻找还未使用的freelist
			obj *p = 0;
			obj **my_free_list = 0;
			for (int i = size; i < MAX_BYTES; ++i)
			{
				my_free_list = free_list + FREELIST_INDEX(i);
				p = *my_free_list;
				if (0 != p)
				{//还有未使用的freelist
					start_free = (char *)p;
					*my_free_list = p->next;
					end_free = start_free + i ;
					//递归调用，修正nobjs
					return chunk_alloc(size, nobjs);
				}
			}
			//没内存可用，寄希望于第一级的new-handler或抛出异常
			end_free = 0;
			start_free = (char *)malloc_alloc::allocate(bytes_to_get);
		}
		heap_size += bytes_to_get;
		end_free = start_free + bytes_to_get;
		return chunk_alloc(size, nobjs);//递归调用，修正nobjs
	}
}