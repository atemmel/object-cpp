#include "linked_list.h"

template<class T>
linked_list<T>::linked_list()
{

}

template<class T>
linked_list<T>::linked_list(const linked_list & src)
{

}

template<class T>
linked_list<T>::~linked_list()
{

}

template<class T>
linked_list<T> & linked_list<T>::operator=(const linked_list & rhs)
{

}

template<class T>
linked_list<T> & linked_list<T>::operator+=(const linked_list & rhs)
{

}

template<class T>
void linked_list<T>::push_front(const T & value)
{

}

template<class T>
void linked_list<T>::push_back(const T & value)
{

}

template<class T>
T linked_list<T>::pop_front()
{

}

template<class T>
T linked_list<T>::pop_back()
{

}


template<class T>
T & linked_list<T>::front()
{

}

template<class T>
T & linked_list<T>::back()
{

}

template<class T>
T & linked_list<T>::at(const size_t & index)
{

}

template<class T>
T & linked_list<T>::operator[](const size_t & index)
{

}

template<class T>
void linked_list<T>::remove(const size_t & index)
{

}

template<class T>
size_t linked_list<T>::size() const
{

}

template<class T>
bool linked_list<T>::empty() const
{

}

template<class T>
void linked_list<T>::print() const
{

}

template<class T>
void linked_list<T>::print_reverse() const
{

}

template<class T>
linked_list<T>::node_t::node_t(T value, node_t * next, node_t * prev) :
	value(value), next(next), prev(prev)
{
}
