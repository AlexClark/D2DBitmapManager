#pragma once

template<typename Type> class SLList;

template<typename Type>
class SLLIter
{
	friend class SLList<Type>;
	typename SLList<Type>::Node* m_prev, *m_curr; 
	SLList<Type>* m_list;

public:
	/////////////////////////////////////////////////////////////////////////////
	// Function : Constructor
	// Parameters :	listToIterate - the list to iterate
	/////////////////////////////////////////////////////////////////////////////
	SLLIter(SLList<Type>& listToIterate);

	/////////////////////////////////////////////////////////////////////////////
	// Function : begin
	// Notes : moves the iterator to the head of the list
	/////////////////////////////////////////////////////////////////////////////
	void begin();

	/////////////////////////////////////////////////////////////////////////////
	// Function : end
	// Notes : returns true if we are at the end of the list, false otherwise
	/////////////////////////////////////////////////////////////////////////////
	bool end() const;

	/////////////////////////////////////////////////////////////////////////////
	// Function : operator++
	// Notes : move the iterator forward one node
	/////////////////////////////////////////////////////////////////////////////
	SLLIter<Type>& operator++();

	/////////////////////////////////////////////////////////////////////////////
	// Function : current
	// Notes : returns the item at the current iterator location
	/////////////////////////////////////////////////////////////////////////////
	Type& current() const { return m_curr->data; }

};

template<typename Type>
SLLIter<Type>& SLLIter<Type>::operator++()
{
	if(m_curr == nullptr)
		return *this;
	
	m_prev = m_curr;
	m_curr = m_curr->next;

	return *this;
}

template<typename Type>
bool SLLIter<Type>::end() const
{
	if(m_curr == nullptr)
		return true;
	else
		return false;
}

template<typename Type>
void SLLIter<Type>::begin()
{
	m_prev = nullptr;
	m_curr = m_list->m_head;
}

template<typename Type>
SLLIter<Type>::SLLIter( SLList<Type>& listToIterate ) : m_list(&listToIterate)
{
	m_prev = nullptr;
	m_curr = listToIterate.m_head;
}

template<typename Type>
class SLList
{
	friend class SLLIter<Type>;

	struct Node
	{
		Type data;
		Node* next;
	};

	Node* m_head;
	unsigned int m_size;
public:
	/////////////////////////////////////////////////////////////////////////////
	// Function : Constructor
	// Notes : constructs an empty list
	/////////////////////////////////////////////////////////////////////////////
	SLList();

	/////////////////////////////////////////////////////////////////////////////
	// Function : Destructor
	// Notes : Destroys the list
	/////////////////////////////////////////////////////////////////////////////
	~SLList();

	/////////////////////////////////////////////////////////////////////////////
	// Function : Assignment Operator
	/////////////////////////////////////////////////////////////////////////////
	SLList<Type>& operator=(const SLList<Type>& that);

	/////////////////////////////////////////////////////////////////////////////
	// Function : Copy Constructor
	/////////////////////////////////////////////////////////////////////////////
	SLList(const SLList<Type>& that);

	/////////////////////////////////////////////////////////////////////////////
	// Function : addHead
	// Parameters :	v - the item to add to the list
	/////////////////////////////////////////////////////////////////////////////
	void addHead(const Type& v);

	/////////////////////////////////////////////////////////////////////////////
	// Function : clear
	// Notes : clears the list, freeing any dynamic memory
	/////////////////////////////////////////////////////////////////////////////
	void clear();

	/////////////////////////////////////////////////////////////////////////////
	// Function : insert
	// Parameters :	index - an iterator to the location to insert at
	//				v - the item to insert
	// Notes : do nothing on a bad iterator
	/////////////////////////////////////////////////////////////////////////////
	void insert(SLLIter<Type>& index, const Type& v);

	/////////////////////////////////////////////////////////////////////////////
	// Function : remove
	// Parameters :	index - an iterator to the location to remove from
	// Notes : do nothing on a bad iterator
	/////////////////////////////////////////////////////////////////////////////
	void remove(SLLIter<Type>& index);

	/////////////////////////////////////////////////////////////////////////////
	// Function : size
	// Return : the number of items stored in the linked list.
	/////////////////////////////////////////////////////////////////////////////
	inline unsigned int size() const {return m_size;}

	Type* getHead() {if(m_head == nullptr) return nullptr; else return &(m_head->data);}

};

template<typename Type>
void SLList<Type>::remove( SLLIter<Type>& index )
{
	if(index.m_curr == nullptr || index.m_list->m_head != m_head)
		return;
	else if(index.m_curr == m_head)
	{
		m_head = m_head->next;
		delete index.m_curr;
		index.m_curr = m_head;
	}
	else
	{
		index.m_prev->next = index.m_curr->next;
		delete index.m_curr;
		index.m_curr = index.m_prev->next;
	}

	m_size--;
}

template<typename Type>
void SLList<Type>::insert( SLLIter<Type>& index, const Type& v )
{
	if(index.m_curr == nullptr || index.m_list->m_head != m_head)
		return;
	else if(index.m_curr == m_head)
	{
		addHead(v);
		index.m_curr = m_head;
	}
	else
	{
		index.m_prev->next = new Node;
		index.m_prev->next->data = v;
		index.m_prev->next->next = index.m_curr;
		index.m_curr = index.m_prev->next;
		m_size++;
	}
}

template<typename Type>
void SLList<Type>::clear()
{
	Node* tmp;
	while(m_head != nullptr)
	{
		tmp = m_head;
		m_head = m_head->next;
		delete tmp;
	}
	m_size = 0;
}

template<typename Type>
void SLList<Type>::addHead( const Type& v )
{
	Node* tmp = new Node;
	tmp->data = v;
	tmp->next = m_head;
	m_head = tmp;
	m_size++;
}

template<typename Type>
SLList<Type>::SLList( const SLList<Type>& that )
{
	if(that.size() == 0)
	{
		m_head = nullptr;
		m_size = 0;
		return;
	}

	Node* thatCurr = that.m_head, *thisCurr = new Node;

	// Initial setup for the head node
	thisCurr->data = thatCurr->data;
	m_head = thisCurr;
	thatCurr = thatCurr->next;

	while(thatCurr != nullptr)
	{
		thisCurr = thisCurr->next = new Node;
		thisCurr->data = thatCurr->data;
		thatCurr = thatCurr->next;
	}
	thisCurr->next = nullptr;
	m_size = that.m_size;
}

template<typename Type>
SLList<Type>::~SLList()
{
	clear();
}

template<typename Type>
SLList<Type>::SLList()
{
	m_size = 0;
	m_head = nullptr;
}


template<typename Type>
SLList<Type>& SLList<Type>::operator=( const SLList<Type>& that )
{
	if(this == &that)
		return *this;

	clear();

	if(that.m_size == 0)
		return *this;

	Node* thatCurr = that.m_head, *thisCurr = new Node;

	// Initial setup for the head node
	thisCurr->data = thatCurr->data;
	m_head = thisCurr;
	thatCurr = thatCurr->next;

	while(thatCurr != nullptr)
	{
		thisCurr->next = new Node;
		thisCurr = thisCurr->next;
		thisCurr->data = thatCurr->data;
		thatCurr = thatCurr->next;
	}

	thisCurr->next = nullptr;
	m_size = that.m_size;

	return *this;
}
