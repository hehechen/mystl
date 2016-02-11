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

//当内存不足时，需要客户端设置handler
void malloc_alloc::set_malloc_oom_handler(void(*f)())
{
	malloc_oom_handler = f;
}

void(*malloc_alloc::malloc_oom_handler)() = 0;

void *malloc_alloc::oom_malloc(size_t n)
{//不断试图获得内存
	void *result;
	for (;;)	//据说这样比while(1)效果更优
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


