#include "RBtree.h"
#pragma once
using namespace Chenstl;
namespace Chenstl {
	void rbtree_base_iterator::increment()
	{
		if (node->right != nullptr)
		{//��������ӽڵ㣬�����ߣ�Ȼ�������ߵ���
			node = node->right;
			while (node->left != nullptr)
				node = node->left;
		}
		else
		{//û�����ӽڵ㣬�ұ��Լ���ĸ��ڵ�
			rbtree_base_node* p_tmp = node->parent;
			while (node == p_tmp->right)
			{//������ڵĽ�����ҽ�㣬��һֱ�����ң�ֱ�������ҽ��
				node = p_tmp;
				p_tmp = p_tmp->parent;
			}
			if (node->right != p_tmp)//��ȵ������û���ҽ��ĸ��ڵ�
				node = p_tmp;
		}

	}
	void rbtree_base_iterator::decrement()
	{
		if (node->color == rb_tree_red && node->parent->parent == node)
			node = node->right;	//��nodeΪhead��㣬��end()ʱ
		else if (node->left != nullptr)
		{//����������һֱ��
			node = node->left;
			while (node->right != nullptr)
				node = node->right;
		}
		else
		{//û�����㣬�ұ��Լ�С�ĸ��ڵ�
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
		else  //������
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
	//xΪ�������
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
					{//����
						rotate_right(parent->parent, root);
						parent->color = rb_tree_black;
						parent->parent->color = rb_tree_red;
					}
					if (x == parent->right)
					{//����������
						rbtree_base_node* grand = parent->parent;
						rotate_left(parent, root);
						rotate_right(grand, root);
						x->color = rb_tree_black;
						grand->color = rb_tree_red;
					}
				}
			}
			else
			{//���ڵ����ҽ��
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
					{//����
						rotate_left(parent->parent, root);
						parent->color = rb_tree_black;
						parent->parent->color = rb_tree_red;
					}
					if (x == parent->right)
					{//����������
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