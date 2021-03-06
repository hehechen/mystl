#ifndef _DEFAULT_ALLOC_H
#define _DEFAULT_ALLOC_H

namespace Chenstl{	
	template<class T, class Alloc>
	class simple_alloc {

	public:
		static T *allocate(size_t n)
		{
			return 0 == n ? 0 : (T*)Alloc::allocate(n * sizeof(T));
		}
		static T *allocate(void)
		{
			return (T*)Alloc::allocate(sizeof(T));
		}
		static void deallocate(T *p, size_t n)
		{
			if (0 != n) Alloc::deallocate(p, n * sizeof(T));
		}
		static void deallocate(T *p)
		{
			Alloc::deallocate(p, sizeof(T));
		}
	};

	//使用内存池以减少碎片
	class default_alloc {
	private:
		enum { ALIGN = 8};
		enum { MAX_BYTES = 128 };
		enum { NFREELISTS = 16 };
		//static const int ALIGN = 8;
		//static const int MAX_BYTES = 128;
		//static const int NFREELISTS = 16;	//MAX_BYTES/ALIGN
		union obj {			//free-lists的节点构造 
			union obj *next;
			char client[1];
		};
		//freelist
		static obj *free_list[NFREELISTS];
		static char *start_free;	//内存池的起始位置
		static char *end_free;		//内存池的终止位置
		static size_t heap_size;

	private:
		//将bytes上调至8的倍数 
		static size_t ROUND_UP(size_t bytes) {
			return ((bytes +ALIGN - 1) & ~(ALIGN - 1));
		}
		//获取合适的区块在freelist中的位置
		static  size_t FREELIST_INDEX(size_t __bytes) {
			return (((__bytes)+(size_t)ALIGN - 1) / (size_t)ALIGN - 1);
		}
		//返回一个大小为n的对象，并可能加入大小为n的其他区块到free-list
		static void *refill(size_t n);
		//配置一大块空间，可容纳nobjs个大小为size的区块
		//如果配置nobjs个区块有所不便，nobjs可能会降低
		static char *chunk_alloc(size_t size, int &nobjs);

	public:
		static void *allocate(size_t n);
		static void deallocate(void *p, size_t n);
		static void * reallocat(void * p, size_t old_sz, size_t new_sz);
	};
}

#endif
