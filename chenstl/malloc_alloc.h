#ifndef _MALLOC_ALLOC_H_ 
#define _MALLOC_ALLOC_H_ 

//定义内存不足又没有定义相关处理函数时抛出的异常
#ifndef THROW_OOM
#    include <stdio.h>
#    include <stdlib.h>
#    define THROW_OOM fprintf(stderr, "out of memory\n"); exit(1)
#endif

#include<stdlib.h>

namespace Chenstl{

//第一级空间配置器，直接用mallloc分配内存
//当需要分配的空间大于MAX_BYTES时使用
	class malloc_alloc{
	private:
		static void *oom_malloc(size_t);	//声明时可以只写类型啊。。现在才知道
		static void *oom_realloc(void *,size_t);
		static void (* malloc_oom_handler)();	//处理malloc时内存不足时的函数指针
	public:
		static void *allocate(size_t n);
		static void decllocate(void *p);

		static void *realloc(void *p, size_t new_sz);
		//当内存不足时，需要客户端设置handler
		static void set_malloc_oom_handler(void(*f)());
	};
}

#endif
