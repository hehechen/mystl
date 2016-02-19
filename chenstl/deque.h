#pragma once
#include "typetraits.h"
using namespace Chenstl;
namespace Chenstl {
	template<class T>
	class deque_iterator :public iterator<bidirectional_iterator_tag, T>
	{
	public:
		T* cur;
		T* first;
		T* last;
		T** node;

		reference operator*() {}
	};
}
