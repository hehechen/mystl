#include<stdio.h>
#include<iostream>
#include "vector.h"
#include"list.h"
#include "heap.hpp"
using namespace Chenstl;
using namespace std;


int main()
{
	list<int> a;
	a.push_back(5);
	a.push_back(2);
	a.push_back(3);
	a.push_back(1);
	a.push_back(7);
	list<int> b;
	b.push_back(9);
	b.push_back(10);
	list<int> c;
	c.push_back(2);
	c.push_back(8);
	b.merge(c);
	a.sort();
	for (auto it : a)
	{
		cout << it << endl;
	}
	return 0;
}