#pragma once

#include "SLList.h"

template<typename Type>
class HTable
{
	SLList<Type>* table;
	unsigned int size;
	unsigned int (*hashFunc)(const Type& v);
public:
	/////////////////////////////////////////////////////////////////////////////
	// Function : Constructor 
	// Parameters : numOfBuckets - the number of buckets
	//              hFunction - pointer to the hash function for this table
	// Notes : constructs an empty hash table
	/////////////////////////////////////////////////////////////////////////////
	HTable(unsigned int numOfBuckets, unsigned int (*hFunction) (const Type &v));

	/////////////////////////////////////////////////////////////////////////////
	// Function : Destructor
	// Notes : destroys a hash table
	/////////////////////////////////////////////////////////////////////////////
	~HTable();

	/////////////////////////////////////////////////////////////////////////////
	// Function : Assignment Operator
	/////////////////////////////////////////////////////////////////////////////
	HTable<Type>& operator=(const HTable<Type>& that);

	/////////////////////////////////////////////////////////////////////////////
	// Function : Copy Constructor
	/////////////////////////////////////////////////////////////////////////////
	HTable(const HTable<Type>& that);

	/////////////////////////////////////////////////////////////////////////////
	// Function : insert
	// Parameters : v - the item to insert into the hash table
	/////////////////////////////////////////////////////////////////////////////
	Type* insert(const Type& v);

	/////////////////////////////////////////////////////////////////////////////
	// Function : findAndRemove
	// Parameters : v - the item to remove(if it is found)
	/////////////////////////////////////////////////////////////////////////////
	bool findAndRemove(const Type& v);

	/////////////////////////////////////////////////////////////////////////////
	// Function : clear
	// Notes : clears the hash table
	/////////////////////////////////////////////////////////////////////////////
	void clear();

	/////////////////////////////////////////////////////////////////////////////
	// Function : find
	// Parameters : v - the item to look for
	// Return : the bucket we found the item in or -1 if we didn’t find the item.
	/////////////////////////////////////////////////////////////////////////////
	int find(const Type& v) const;

	SLList<Type>& getList(const unsigned int index) const { return table[index]; }
	Type* findAndGet(const Type& v);

};

template<typename Type>
HTable<Type>::HTable(unsigned int numOfBuckets, unsigned int (*hFunction) (const Type &v))
{
	hashFunc = hFunction;
	size = numOfBuckets;
	table = new SLList<Type>[size];
}

template<typename Type>
HTable<Type>::~HTable()
{
	delete[] table;
}


template<typename Type>
HTable<Type>& HTable<Type>::operator=(const HTable<Type>& that)
{
	if(this == &that)
		return *this;

	delete[] table;
	hashFunc = that.hashFunc;
	size = that.size;
	table = new SLList<Type>[size];
	for(unsigned int i = 0u; i < size; table[i] = that.table[i], i++);
	return *this;
}


template<typename Type>
HTable<Type>::HTable(const HTable<Type>& that)
{
	table = nullptr;
	*this = that;
}

template<typename Type>
Type* HTable<Type>::insert(const Type& v)
{
	int index = hashFunc(v);
	table[index].addHead(v);
	return table[index].getHead();
}

template<typename Type>
bool HTable<Type>::findAndRemove(const Type& v)
{
	int index = hashFunc(v);
	SLLIter<Type> iter(table[index]);
	iter.begin();
	while(!iter.end())
	{
		if(iter.current() == v)
		{
			table[index].remove(iter);
			return true;
		}
		else
		{
			++iter;
		}
	}
	return false;
}


template<typename Type>
void HTable<Type>::clear()
{
	for(unsigned int i = 0; i < size; table[i].clear(), i++);
}

template<typename Type>
int HTable<Type>::find(const Type& v) const
{
	int index = hashFunc(v);
	SLLIter<Type> iter(table[index]);
	iter.begin();
	while(!iter.end())
	{
		if(iter.current() == v)
		{
			return index;
		}
		else
		{
			++iter;
		}
	}
	return -1;
}

template<typename Type>
Type* HTable<Type>::findAndGet(const Type& v)
{
	int index = hashFunc(v);
	SLLIter<Type> iter(table[index]);
	iter.begin();
	while(!iter.end())
	{
		if(iter.current() == v)
		{
			return &iter.current();
		}
		else
		{
			++iter;
		}
	}
	return nullptr;
}