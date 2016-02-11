#pragma once
#include "Iterator.h"
#include "typetraits.h"
#include"default_alloc.h"
#include"construct.h"
#include"alog.h"
using namespace Chenstl;

namespace Chenstl {
	template <class T>
	struct __list_node {
		typedef void* void_pointer;
		void_pointer next;
		void_pointer prev;
		T data;
	};

	template<class T, class Ref, class Ptr>
	struct __list_iterator {
		typedef __list_iterator<T, T&, T*>             iterator;
		typedef __list_iterator<T, const T&, const T*> const_iterator;
		typedef __list_iterator<T, Ref, Ptr>           self;

		typedef bidirectional_iterator_tag iterator_category;
		typedef T value_type;
		typedef Ptr pointer;
		typedef Ref reference;
		typedef __list_node<T>* link_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		link_type node;

		__list_iterator(link_type x) : node(x) {}
		__list_iterator() {}
		__list_iterator(const iterator& x) : node(x.node) {}

		bool operator==(const self& x) const { return node == x.node; }
		bool operator!=(const self& x) const { return node != x.node; }
		reference operator*() const { return (*node).data; }

		pointer operator->() const { return &(operator*()); }

		self& operator++() {
			node = (link_type)((*node).next);
			return *this;
		}
		self operator++(int) {		//后缀
			self tmp = *this;
			++*this;
			return tmp;
		}
		self& operator--() {
			node = (link_type)((*node).prev);
			return *this;
		}
		self operator--(int) {
			self tmp = *this;
			--*this;
			return tmp;
		}
	};
	//双向循环链表
	template<class T, class Alloc = default_alloc>
	class list {
	private:
		typedef T value_type;
		typedef value_type& reference;
		typedef size_t size_type;
		typedef __list_node<T> list_node;
		typedef list_node* link_type;
		typedef simple_alloc<list_node, Alloc> list_node_allocator;
		link_type node;	//头结点，也是end()，next为begin()
	public:
		typedef __list_iterator<T, T&, T*> iterator;
	public:
		list() { empty_initialize(); }
		iterator begin() { return (link_type)((*node).next); }
		iterator end() { return node; }
		bool empty() { return node->next == node; }
		size_type size() { return distance(begin(), end()); }
		reference front() { return *begin(); }	//头结点的元素值
		reference back() { return *end(); }
		void push_back(const T& val) { insert(end(), val); }
		void push_front(const T& val) { insert(begin(), val); }
		//在position前插入
		iterator insert(iterator position, const T& val)
		{
			link_type tmp = create_node(val);
			tmp->next = position.node;
			tmp->prev = position.node->prev;
			(link_type(position.node->prev))->next = tmp;
			position.node->prev = tmp;
			return tmp;
		}
		//擦除position所指结点，返回下一个结点的迭代器
		iterator erase(iterator position)
		{
			link_type next_node = (link_type(position.node->next));
			link_type prev_node = (link_type(position.node->prev));
			prev_node->next = next_node;
			next_node->prev = prev_node;
			destroy_node(position.node);
			return next_node;
		}
		void pop_front() { erase(begin()); }
		void pop_back() { erase(--end()); }
		//清楚所有节点
		void clear()
		{
			link_type cur = begin();
			while (cur != node)
			{
				link_type tmp = cur;
				cur = cur->next;
				destroy_node(tmp);
			}
			node->next = node;
			node->prev = node;
		}
		//将所有值为val的元素移除
		void remove(const T& val)
		{
			iterator cur = begin();
			while (cur != end())
			{
				iterator next = cur;
				++next;
				if (*cur == val)	erase(cur);
				cur = next;
			}
		}
		//移除所有数值相同的连续元素
		void unique()
		{//用快慢指针
			iterator first = begin();
			iterator last = first + 1;
			if (first == last)	return;//空链表直接返回
			while (last != node)
			{
				if (*first == *last)
				{
					erase(first);
					++last;
				}
				else { ++first; ++last; }
			}
		}
		void splice(iterator position, list& x)
		{
			if (!x.empty())	transfer(position, x.begin(), x.end());
		}
		//将i所指元素结合到position前
		void splice(iterator position, list& x, iterator i)
		{
			iterator j = i;
			++j;
			if (position == i || position == j)	return;
			transfer(position, i, j);
		}
		//将[first,last)结合到position前
		void splice(iterator position, iterator first, iterator last)
		{
			if (first != last)
				transfer(position, first, last);
		}
		//归并，两个list的内容必须是递增排序
		void merge(list<T, Alloc>& x)
		{
			iterator first1 = begin();
			iterator last1 = end();
			iterator first2 = x.begin();
			iterator last2 = x.end();
			while (first1 != last1 && first2 != last2)
			{
				if (*first1 > *first2)
				{
					iterator next = first2;
					transfer(first1, first2, ++next);
					first2 = next;
				}
				else
					++first1;
			}
			if (first2 != last2)	//此时*first2>*(last1-1)
				transfer(last1, first2, last2);
		}
		//归并排序
		void sort()
		{
			//仅有零个或一个元素则不进行操作
			if (node->next == node || link_type(node->next)->next == node)	return;
			list<T, Alloc> carry;
			list<T, Alloc>counter[64];
			int fill = 0;
			while (!empty()) {
				carry.splice(carry.begin(), *this, begin());
				int i = 0;
				while (i < fill && !counter[i].empty()) {
					counter[i].merge(carry);
					carry.swap(counter[i++]);
				}
				carry.swap(counter[i]);
				if (i == fill) ++fill;
			}

			for (int i = 1; i < fill; ++i) counter[i].merge(counter[i - 1]);
			swap(counter[fill - 1]);
		}
	private:
		//配置结点所需的内存
		link_type get_node() { return list_node_allocator::allocate(); }
		//释放节点
		void put_node(link_type p) { list_node_allocator::deallocate(p); }
		//配置并构造节点
		link_type create_node(const T& val)
		{
			link_type p = get_node();
			construct(&(p->data), val);
			return p;
		}
		//析构并释放结点
		void destroy_node(link_type p)
		{
			destroy(&(p->data));
			put_node(p);
		}
		//产生一个空链表
		void empty_initialize()
		{
			node = get_node();
			node->next = node;
			node->prev = node;
		}
		//将[first,last)内的所有元素移动到position之前
		void transfer(iterator position, iterator first, iterator last)
		{
			(link_type(first.node->prev))->next = last.node;
			link_type tmp = link_type(position.node->prev);
			link_type(last.node->prev)->next = position.node;
			position.node->prev = last.node->prev;
			last.node->prev = first.node->prev;
			tmp->next = first.node;
			first.node->prev = tmp;
		}
		void swap(list<T, Alloc>& x) { _swap(node, x.node); }
	};
}

