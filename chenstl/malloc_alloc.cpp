#include "malloc_alloc.h"

using namespace Chenstl;
void *malloc_alloc::allocate(size_t n)
{
	void *result = malloc(n);
	if (0 == result) result = oom_malloc(n);
	return result;
}

void malloc_alloc::decllocate(void *p)
{
	free(p);
}

void * malloc_alloc::realloc(void *p, size_t new_sz)
{
	void *result = realloc(p, new_sz);
	if (0 == result)	result = oom_realloc(p, new_sz);
	return result;
}

//���ڴ治��ʱ����Ҫ�ͻ�������handler
void malloc_alloc::set_malloc_oom_handler(void(*f)())
{
	malloc_oom_handler = f;
}

void(*malloc_alloc::malloc_oom_handler)() = 0;

void *malloc_alloc::oom_malloc(size_t n)
{//������ͼ����ڴ�
	void *result;
	for (;;)	//��˵������while(1)Ч������
	{
		if (0 == malloc_oom_handler) THROW_OOM;
		(*malloc_oom_handler)();
		result = malloc(n);
		if (result)	return result;
	}
}

void *malloc_alloc::oom_realloc(void *p, size_t n)
{
	void *result;
	for (;;)
	{
		if (0 == malloc_oom_handler) THROW_OOM;
		(*malloc_oom_handler)();
		result = realloc(p, n);
		if (result)	return result;
	}
}


