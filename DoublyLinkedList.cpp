
// File: DoublyLinkedList.cpp
// Author(s): Jingyi Guo

#include <iostream>
using namespace std;
#include "DoublyLinkedList.h"

// define what each node looks like
// (Data is a type member of the DoublyLinkedList class)
struct DoublyLinkedList::Data {
    int value;
    DoublyLinkedList::Data *next;
    DoublyLinkedList::Data *prev;
};

// define a type with a more familiar name, "dll_node"
typedef DoublyLinkedList::Data dll_node;

// Now we can pretend we have:
// struct dll_node {
//     int value;
//     dll_node *next;
//     dll_node *prev;
// };

// default constructor: empty list
DoublyLinkedList::DoublyLinkedList()
: d_ptr(0)  // sets d_ptr to point nowhere
{ }

// constructor for a one-node (one value) list
DoublyLinkedList::DoublyLinkedList(int val)
: d_ptr(new dll_node)  // allocate one new node
{
    d_ptr->value = val;
    d_ptr->next = d_ptr;
    d_ptr->prev = d_ptr;
}

// constructor for a two-node (two value) list
DoublyLinkedList::DoublyLinkedList(int val1, int val2)
: d_ptr(new dll_node)  // allocate first node
{
    d_ptr->value = val1;
    d_ptr->next = new dll_node;  // allocate second node
    d_ptr->prev = d_ptr->next;   // which is both next AND
                                 // prev of first node

    d_ptr->next->value = val2;   // second node's value
    d_ptr->next->next = d_ptr;   // first node is both next
    d_ptr->next->prev = d_ptr;   // AND prev of second node
}

// display the list in reverse
void DoublyLinkedList::put_rev(ostream& os) const
{
    if (!d_ptr) return;  // empty list: do nothing

    // display the rest of the nodes in reverse, until we
    // have circled back to the first node again
    for (dll_node *p(d_ptr->prev); p != d_ptr; p = p->prev)
        os << p->value << " ";
    // finally, display the first node's value
    os << d_ptr->value;
}

DoublyLinkedList::~DoublyLinkedList()//destructor
{
	if (!d_ptr) return;
	for (dll_node *last = d_ptr->prev; last != d_ptr;)
	{
		dll_node *temp = last->prev;
		delete last;
		last = temp;
	}
	delete d_ptr;
	d_ptr = 0;
}

void DoublyLinkedList::push_back(int val)
{
	if (!d_ptr)
	{
		d_ptr = new dll_node{ val,0,0 };
		d_ptr->next = d_ptr;
		d_ptr->prev = d_ptr;
		return;
	}
	dll_node *nowback = d_ptr->prev;
	dll_node *newback = new dll_node{val,0,0};
	newback->prev = nowback;
	newback->next = d_ptr;
	nowback->next = newback;
	d_ptr->prev = newback;
}

void DoublyLinkedList::pop_back()
{
	if (!d_ptr)
		return;
	if (d_ptr == d_ptr->next)
	{
		delete d_ptr;
		d_ptr = 0;
		return;
	}
	dll_node *last = d_ptr->prev;
	dll_node *second_to_last = d_ptr->prev->prev;
	d_ptr->prev = second_to_last;
	second_to_last->next = d_ptr;
	delete last;
}

void DoublyLinkedList::push_front(int val)
{
	if (!d_ptr)
	{
		d_ptr = new dll_node{ val,0,0};
		d_ptr->next = d_ptr;
		d_ptr->prev = d_ptr;
		return;
	}
	dll_node *nowfirst = d_ptr;
	dll_node *newfirst = new dll_node{val,0,0};
	newfirst->prev = nowfirst->prev;
	newfirst->next = nowfirst;
	d_ptr = newfirst;
	nowfirst->prev->next = newfirst;
	nowfirst->prev = newfirst;
}

void DoublyLinkedList::pop_front()
{
	if (!d_ptr)
		return;
	if (d_ptr == d_ptr->next)
	{
		delete d_ptr;
		d_ptr = 0;
		return;
	}
	dll_node *first = d_ptr;
	dll_node *second = d_ptr->next;
	first->prev->next = second;
	second->prev = first->prev;
	d_ptr = second;
	delete first;
}

int DoublyLinkedList::front() const
{
	if (!d_ptr)
		return 0;
	return d_ptr->value;
}

int DoublyLinkedList::back() const
{
	if (!d_ptr)
		return 0;
	return d_ptr->prev->value;
}

DoublyLinkedList::DoublyLinkedList(const DoublyLinkedList & other)
	: d_ptr(0)
{
	if (other.d_ptr == 0)
		return;
	for (dll_node *p = other.d_ptr; p->next!=other.d_ptr;p=p->next)
		push_back(p->value);
	push_back(other.d_ptr->prev->value);
}

DoublyLinkedList & DoublyLinkedList::operator=(const DoublyLinkedList & other)
{
	if (this == &other)
		return *this;
	while (d_ptr)
		pop_back();
	if (other.d_ptr == 0)
		return *this;
	else
	{
		for (dll_node *p = other.d_ptr; p->next != other.d_ptr; p = p->next)
			push_back(p->value);
		push_back(other.d_ptr->prev->value);
		return *this;
	}
}

int DoublyLinkedList::size() const
{
	if (!d_ptr)
		return 0;
	else
	{
		int size = 1;
		for (dll_node *p = d_ptr; p->next != d_ptr; p = p->next)
				size++;
		return size;
	}
}

int DoublyLinkedList::min() const
{
	if (!d_ptr)
		return 0;
	else
	{
		int min = d_ptr->value;
		for (dll_node *p = d_ptr; p->next != d_ptr; p = p->next)
			min = p->value > min ? min : p->value;//haven't compared the last one
		min = d_ptr->prev->value > min ? min : d_ptr->prev->value;
		return min;
	}
}

int DoublyLinkedList::max() const
{
	if (!d_ptr)
		return 0;
	else
	{
		int max = d_ptr->value;
		for (dll_node *p = d_ptr; p->next != d_ptr; p = p->next)
			max = p->value > max ? p->value : max;//haven't compared the last one
		max= d_ptr->prev->value > max ? d_ptr->prev->value : max;
		return max;
	}
}

int DoublyLinkedList::sum() const
{
	if (!d_ptr)
		return 0;
	else
	{
		int sum = 0;
		for (dll_node *p = d_ptr; p->next != d_ptr; p = p->next)
			sum+=p->value;
		sum += d_ptr->prev->value;
		return sum;
	}
}

double DoublyLinkedList::mean() const
{
	return DoublyLinkedList::sum()/ (double)DoublyLinkedList::size();
}

ostream & operator<<(ostream & os, const DoublyLinkedList & list)
{
	if (!list.d_ptr) return os;
	dll_node *p = list.d_ptr;
	for (p; p!= list.d_ptr->prev; p = p->next)
		os << p->value << " ";
	os << p->value;
	return os;
}
