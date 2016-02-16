#include "default_alloc.h"
#include "malloc_alloc.h"
using namespace Chenstl;

default_alloc::obj *default_alloc::free_list[NFREELISTS]
= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
char *default_alloc::start_free = 0;	//�ڴ�ص���ʼλ��
char *default_alloc::end_free = 0;		//�ڴ�ص���ֹλ��
size_t default_alloc::heap_size = 0;

void *default_alloc::allocate(size_t n)
{
	obj *result = 0;
	obj **my_free_list = 0;
	if (n > MAX_BYTES)
		return malloc_alloc::allocate(n);
	//Ѱ��free lists�к��ʵ�һ��				
	my_free_list = free_list + FREELIST_INDEX(n);
	result = *my_free_list;
	if(0 == result)
	{//û���ҵ����õ�freelist�����ڴ����ȡ���ռ�
		return refill(ROUND_UP(n));
	}
	//����freelist
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
//����һ����СΪn�Ķ��󣬲����ܼ����СΪn���������鵽freelist
//��ANSI c�У�void *��������мӼ�����������chunk��char *
void *default_alloc::refill(size_t n)
{
	int objs = 20;
	char *chunk = chunk_alloc(n, objs);
	
	obj *next = nullptr, *current = nullptr;
	obj *result = nullptr;
	obj **my_free_list = nullptr;
	if (1 == objs)	//ֻȡ��һ������
		return chunk;
	my_free_list = free_list + FREELIST_INDEX(n);
	result = (obj *)chunk;	//��һ�鷵�ظ��ͻ���
	//��freellistָ����������
	*my_free_list = next = (obj *)(chunk + n);
	for (int i = 1;; i++)
	{
		current = next;
		next = (obj *)((char *)next + n);	//����ע�ⲻ��ֱ����next+n
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
	size_t bytes_left = end_free - start_free;	//�ڴ��ʣ��ռ�
	if (bytes_left >= total_bytes)
	{//�ڴ���㹻�ṩ�����ڴ�
		result = start_free;
		start_free += total_bytes;
		return result;
	}
	else if (bytes_left >= size)
	{//�ڴ���㹻��Ӧһ�����ϵ�����
		nobjs = bytes_left / size;
		total_bytes = nobjs * size;
		result = start_free;
		start_free += total_bytes;
		return result;
	}
	else
	{//�ڴ��һ������Ҳ��Ӧ����
		size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);;
		if (bytes_left>0)
		{//���ڴ�ص���ͷ��������ʵ�freelist
			obj **my_free_list = free_list + FREELIST_INDEX(bytes_left);
			((obj *)start_free)->next = *my_free_list;
			*my_free_list = (obj *)start_free;
		}
		start_free = (char *)malloc(bytes_to_get);
		if (!start_free) 
		{//ϵͳ���ڴ治�㣬Ѱ�һ�δʹ�õ�freelist
			obj *p = 0;
			obj **my_free_list = 0;
			for (int i = size; i < MAX_BYTES; ++i)
			{
				my_free_list = free_list + FREELIST_INDEX(i);
				p = *my_free_list;
				if (0 != p)
				{//����δʹ�õ�freelist
					start_free = (char *)p;
					*my_free_list = p->next;
					end_free = start_free + i ;
					//�ݹ���ã�����nobjs
					return chunk_alloc(size, nobjs);
				}
			}
			//û�ڴ���ã���ϣ���ڵ�һ����new-handler���׳��쳣
			end_free = 0;
			start_free = (char *)malloc_alloc::allocate(bytes_to_get);
		}
		heap_size += bytes_to_get;
		end_free = start_free + bytes_to_get;
		return chunk_alloc(size, nobjs);//�ݹ���ã�����nobjs
	}
}