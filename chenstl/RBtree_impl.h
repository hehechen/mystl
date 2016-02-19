#include "RBtree.h"
#pragma once
using namespace Chenstl;
namespace Chenstl {
	void rbtree_base_iterator::increment()
	{
		if (node->right != nullptr)
		{//如果有右子节点，向右走，然后往左走到底
			node = node->right;
			while (node->left != nullptr)
				node = node->left;
		}
		else
		{//没有右子节点，找比自己大的父节点
			rbtree_base_node* p_tmp = node->parent;
			while (node == p_tmp->right)
			{//如果现在的结点是右结点，就一直往上找，直到不是右结点
				node = p_tmp;
				p_tmp = p_tmp->parent;
			}
			if (node->right != p_tmp)//相等的情况是没有右结点的根节点
				node = p_tmp;
		}

	}
	void rbtree_base_iterator::decrement()
	{
		if (node->color == rb_tree_red && node->parent->parent == node)
			node = node->right;	//当node为head结点，即end()时
		else if (node->left != nullptr)
		{//向左再向右一直走
			node = node->left;
			while (node->right != nullptr)
				node = node->right;
		}
		else
		{//没有左结点，找比自己小的父节点
			rbtree_base_node* p_tmp = node->parent;
			while (p_tmp->left == node)
			{
				node = p_tmp;
				p_tmp = p_tmp->parent;
			}
			node = p_tmp;
		}
	}
	inline void rotate_left(rbtree_base_node* k, rbtree_base_node* root)
	{
		rbtree_base_node* c_tmp = k->right;
		k->right = c_tmp->left;
		if (c_tmp->left != nullptr)
			c_tmp->left->parent = k;
		c_tmp->parent = k->parent;
		if (k == root)
			root = c_tmp;
		else if (k == k->parent->left)
			k->parent->left = c_tmp;
		else  //右子树
			k->parent->right = c_tmp;
		c_tmp->left = k;
		k->parent = c_tmp;
	}

	inline void rotate_right(rbtree_base_node* k, rbtree_base_node* root)
	{
		rbtree_base_node* c_tmp = k->left;
		c_tmp->parent = k->parent;
		k->left = c_tmp->right;
		if (c_tmp->right != nullptr)
			c_tmp->right->parent = k;
		c_tmp->parent = k->parent;
		if (k == root)
			root = c_tmp;
		else if (k == k->parent->left)
			k->parent->left = c_tmp;
		else
			k->parent->right = c_tmp;
		c_tmp->right = k;
		k->right = c_tmp;
	}
	//x为新增结点
	inline void rebalance(rbtree_base_node* x, rbtree_base_node* root)
	{
		x->color = rb_tree_red;
		while (x != root && x->parent->color == rb_tree_red)
		{
			rbtree_base_node* parent = x->parent;
			if (parent->parent->left == parent)
			{
				rbtree_base_node* uncle = parent->parent->right;
				if (uncle && uncle->color == rb_tree_red)
				{
					parent->color = rb_tree_black;
					uncle->color = rb_tree_black;
					parent->parent->color = rb_tree_red;
					x = parent->parent;
				}
				else
				{
					if (x == parent->left)
					{//右旋
						rotate_right(parent->parent, root);
						parent->color = rb_tree_black;
						parent->parent->color = rb_tree_red;
					}
					if (x == parent->right)
					{//左旋再右旋
						rbtree_base_node* grand = parent->parent;
						rotate_left(parent, root);
						rotate_right(grand, root);
						x->color = rb_tree_black;
						grand->color = rb_tree_red;
					}
				}
			}
			else
			{//父节点是右结点
				rbtree_base_node* uncle = parent->parent->left;
				if (uncle && uncle->color == rb_tree_red)
				{
					parent->color = rb_tree_black;
					uncle->color = rb_tree_black;
					parent->parent->color = rb_tree_red;
					x = parent->parent;
				}
				else
				{
					if (x == parent->right)
					{//左旋
						rotate_left(parent->parent, root);
						parent->color = rb_tree_black;
						parent->parent->color = rb_tree_red;
					}
					if (x == parent->right)
					{//右旋再左旋
						rbtree_base_node* grand = parent->parent;
						rotate_right(parent, root);
						rotate_left(grand, root);
						x->color = rb_tree_black;
						grand->color = rb_tree_red;
					}
				}
			}
		}
		root->color = rb_tree_black;
	}
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename RBtree<Key, Value, KeyOfValue, Compare, default_alloc>::iterator 
	RBtree<Key, Value, KeyOfValue, Compare, default_alloc>::
	insert(rbtree_base_node * it_new, rbtree_base_node * it_parent, const Value& value)
	{
		rbtree_node* x = (rbtree_node*)it_new;
		rbtree_node* y = (rbtree_node*)it_parent;
		x = create_node(value);
		x->parent = y;
		x->left = nullptr;
		x->right = nullptr;
		++node_num;
		return iterator(x);
	}
	template <class Key, class Value, class KeyOfValue, class Compare, class Alloc = default_alloc>
	pair<typename RBtree<Key, Value, KeyOfValue, Compare, default_alloc>::iterator,bool>
	RBtree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const Value& value)
	{
		rbtree_node* x = root();
		rbtree_node*y = head;
		while (x != nullptr)
		{
			if (x->value_field == value)
				return pair<iterator, bool>(x, false);
			y = x;
			x = compare(key(value), key(x)) ? x->left : x->right;
		}
		return insert(x, y, value);
	}
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename RBtree<Key, Value, KeyOfValue, Compare, default_alloc>::iterator
	RBtree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const Value & value)
	{
		rbtree_node* x = root();
		rbtree_node*y = head;
		while (x != nullptr)
		{
			y = x;
			x = compare(key(value), key(x)) ? x->left : x->right;
		}
		return insert(x, y, value);
	}
}