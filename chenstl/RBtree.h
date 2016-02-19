#pragma once
#include "construct.h"
#include "typetraits.h"
#include "default_alloc.h"
#include "pair.h"

using namespace Chenstl;
namespace Chenstl {
	typedef bool rbtree_color_type;
	const rbtree_color_type rb_tree_red = true;
	const rbtree_color_type rb_tree_black = false;

	struct rbtree_base_node
	{
		rbtree_color_type color;
		rbtree_base_node* parent;
		rbtree_base_node* left;
		rbtree_base_node* right;

		static rbtree_base_node* minimum(rbtree_base_node* base)
		{
			while (base->left != nullptr)	base = base->left;
			return base;
		}
		static rbtree_base_node* maximum(rbtree_base_node* base)
		{
			while (base->right != nullptr)	base = base->right;
			return base;
		}
	};
	//分成两个是为了作参数时不用写template
	template<class Value>
	struct rbtree_node :public rbtree_base_node
	{
		Value value_field;
	};

	struct rbtree_base_iterator
	{		
		rbtree_base_node* node;
		//如果最小值(最大值)是根节点，对其进行加减还是根节点
		//如果不是，则为end()	
		void increment();
		void decrement();
	};
	//设置多个模板参数便于生成const型和非const型的iterator
	template<class T, class Ref, class Pointer>
	struct rbtree_iterator
	{
		typedef bidirectional_iterator_tag iterator_category;
		typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef Pointer pointer;
		typedef Ref reference;

		typedef rbtree_iterator<T, Ref, Pointer> self;
		self& operator++()	//前缀
		{
			increment();
			return *this;
		}
		self operator++(int)
		{
			self tmp = *this;
			increment();
			return tmp;
		}
		self& operator--()
		{
			decrement();
			return *this;
		}
		self operator--(int)
		{
			self tmp = *this;
			decrement();
			return tmp;
		}
		reference operator*() const { return node->value_field; }
		pointer operator->() const { return &(operator*()); }
	};

	inline void rotate_left(rbtree_base_node* k, rbtree_base_node* root);
	inline void rotate_right(rbtree_base_node* k, rbtree_base_node* root);
	//x为新增结点
	inline void rebalance(rbtree_base_node* x, rbtree_base_node* root);

	template <class Key, class Value, class KeyOfValue, class Compare,
	class Alloc = default_alloc>
	class RBtree
	{
	public:
		typedef rbtree_iterator<Value, Value&, Value*> iterator;
		pair<iterator, bool> insert_unique(const Value& value);
		iterator insert_equal(const Value& value);
	private:
		typedef simple_alloc<rbtree_node, Alloc> rbtree_node_allocator;
		size_t node_num;
		rbtree_node* head;
		Compare compare;

		rbtree_node* get_node() { return rbtree_node_allocator::allocate(); }
		void put_node(rbtree_node* p) { rbtree_node_allocator::deallocate(p); }
		rbtree_node* create_node(const Value value)
		{
			rbtree_node* tmp = get_node();
			construct(tmp, value);
			return tmp;
		}
		rbtree_node* clone_node(rbtree_node* x);
		void destroy_node(rbtree_node* p)
		{
			destroy(&p->value_field);
			put_node(p);
		}

		rbtree_node* root() { return head->parent; }
		rbtree_node* left_most{ return head->left; }
		rbtree_node* right_most{ return head->right; }
		
		void init()
		{
			head = get_node();
			head->color = rb_tree_red;
			root() = 0;
			left_most = head;
			right_most = head;
		}
		Key key(const Value& value) { return KeyOfValue()(value); }
		iterator insert(rbtree_base_node * it_new, rbtree_base_node * it_parent, const Value& value);
	};

}


#include "RBtree_impl.h"

