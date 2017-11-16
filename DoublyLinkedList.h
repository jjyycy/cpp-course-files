
// File: DoublyLinkedList.h
// Author(s): Jingyi Guo

#include <iostream>
using namespace std;
#ifndef _DBL_LINK_LIST_
#define _DBL_LINK_LIST_
//Class definition
class DoublyLinkedList {
public:
    DoublyLinkedList();        // default constructor
    DoublyLinkedList(int);     // construct a one-node list
    DoublyLinkedList(int,int); // construct a two-node list
    void put_rev(ostream &) const;   // display the list in reverse
                               // to some ostream
    struct Data;
	friend ostream& operator <<(ostream&, const DoublyLinkedList&);
	~DoublyLinkedList();
	void push_back(int);
	void pop_back();
	void push_front(int);
	void pop_front();
	int front() const;
	int back() const;
	DoublyLinkedList(const DoublyLinkedList&);
	DoublyLinkedList& operator=(const DoublyLinkedList&);
	int size() const;
	int min() const;
	int max() const;
	int sum() const;
	double mean() const;
private:
    Data *d_ptr;               // a pointer to some Data
};
#endif