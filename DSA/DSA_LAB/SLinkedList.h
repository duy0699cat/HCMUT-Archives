#pragma once
#include <iostream>
template <class T>
class SLinkedList {
public:
    class Node; // Forward declaration
protected:
    Node* head;
    Node* tail;
    int count;
public:
   SLinkedList();
   ~SLinkedList();
    void    add(T e);
    void    add(int index, T e);
    int     size();
    bool    empty();

    void    clear();
    T       get(int index);
    void    set(int index, T e);
    int     indexOf(T item);
    bool    contains(T item);
    T       removeAt(int index);
    bool    removeItem(T item);
    void    Lprint();
public:
    class Node {
    private:
        T data;
        Node* next;
        friend class SLinkedList<T>;
    public:
        Node() {
            next = 0;
        }
        Node(Node* next) {
            this->next = next;
        }
        Node(T data, Node* next = NULL) {
            this->data = data;
            this->next = next;
        }
    };
};
/////////////////////////////////////////////
template <class T>
void SLinkedList<T>::add(T e) {
	// allocating space
	T data = e;
	Node* newNode = new Node();

	// inserting the required data
	newNode->data = data;
	newNode->next = NULL;

	// If the Linked List is empty, then make the new node as head 
	if (head == NULL)
	{
		/////////////////////////////////
		head = newNode;
		tail = newNode;
	}
	else {
		tail->next = newNode;
		tail = newNode;
	}
	count++;
}
//////////////////////////////////////////////////////////////////////////////////////////
template <class T>
void SLinkedList<T>::add(int index,T e) {

	int pos = index; //?//
	Node** current = &head;
	T data = e;

	if (pos > count) pos = count;
	if (pos < 0) pos = 0;

	// allocating space
	Node* newNode = new Node();

	// inserting the required data
	newNode->data = data;
	newNode->next = NULL;

	// allocating space
	Node* temp = new Node();

	// inserting the required data
	temp->data = data;
	temp->next = NULL;


	// If the Linked List is empty, then make the new node as head 
	if (head == NULL)
	{
		/////////////////////////////////
		head = newNode;
		tail = newNode;
	}

	if (pos == 0)
	{
		newNode->next = head; //link to old head
		head = newNode; //then become new head
	}
	else if (pos == count) {
		add(data);
		count--;
	}

	else {

		// Keep looping until the pos is zero
		for (int i = 0; i < pos; i++) {
			current = &(*current)->next;
		}
		temp->next = *current;
		*current = temp;
	}

	count++;
}

template<class T>
int SLinkedList<T>::size() {
	if (head == NULL)  count = 0;

	return count;
}
////////////////////////////////////////////////
template<class T>
T SLinkedList<T>::get(int index) {
    /* Give the data of the element at given index in the list. */

    Node* tmp = new Node;
    tmp = head;
    if (index<0 || index>(count - 1)) {
        throw std::out_of_range("F");
    }
    for (int i = 0; i < index; i++)    tmp = tmp->next;
    return tmp->data;

}




template <class T>
void SLinkedList<T>::set(int index, T e) {
    /* Assign new value for element at given index in the list */
    Node* tmp = new Node;
    tmp = head;
    if (index<0 || index>(count - 1)) {
        throw std::out_of_range("F");
    }
    for (int i = 0; i < index; i++) {
        tmp = tmp->next;
    }
    tmp->data = e;
}




template<class T>
bool SLinkedList<T>::empty() {
    /* Check if the list is empty or not. */
    if (head == NULL)return true;
    else return false;
}



template<class T>
int SLinkedList<T>::indexOf(T item) {
    /* Return the first index wheter item appears in list, otherwise return -1 */
    Node* temp = head;
    int count = 0;
    while (temp != NULL)
    {
        if (temp->data == item)  return count;
        ++count;
        temp = temp->next;
    }
    //return -1 if no match 
    return -1;
}



template<class T>
bool SLinkedList<T>::contains(T item) {
    Node* temp = head;
    while (temp != NULL)
    {
        if (temp->data == item)  return 1;
        temp = temp->next;
    }
    return 0;
}
//////////////////////////////////////////////////////////
template <class T>
T SLinkedList<T>::removeAt(int index)
{
    T tdata;
    // If linked list is empty
    if (head == NULL) throw"nothing to delete";

    // Store head node
    Node* temp = head;

    // If head needs to be removed
    if (index == 0)
    {

        // Change head
        head = temp->next;

        // Free old head
        T tdata = temp->data;
        delete(temp);
        --count;
        return tdata;
    }

    // Find previous node of the node to be deleted
    for (int i = 0; temp != NULL && i < index - 1; i++)
        temp = temp->next;

    // If index is more than number of nodes
    if (temp == NULL || temp->next == NULL) throw"out of range";

    // Node temp->next is the node to be deleted
    // Store pointer to the next of node to be deleted
    Node* next = temp->next->next;

    // Unlink the node from linked list
    tdata = temp->next->data;
    delete(temp->next); // Free memory

    // Unlink the deleted node from list
    temp->next = next;
    --count;
    return tdata;
}

template <class T>
bool SLinkedList<T>::removeItem(T item)
{
    int i;
    /* Remove the first apperance of item in list and return true, otherwise return false */
    i = SLinkedList::indexOf(item);
    if (i == -1) return false;
    else
    {
        removeAt(i);
        return true;
    }

}

template<class T>
void SLinkedList<T>::Lprint()
{
    Node* temp = head;
    int idx = 0;
    while (temp!=NULL)
    {
        std::cout << temp->data << " ";
        temp = temp->next;
        ++idx;
    }

}

template<class T>
void SLinkedList<T>::clear() {
    /* Remove all elements in list */
    for (int i = 0; i < count; i++) {
        removeAt(i);
    }
    count = 0;
}
template<class T>
SLinkedList<T>::SLinkedList() {
    //std::cout << "Constructor called \n";
}
template<class T>
SLinkedList<T>::~SLinkedList() {
    //std::cout << "Destructor called \n";
}