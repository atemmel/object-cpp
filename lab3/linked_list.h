#pragma once
#include <cstddef> //För användande av size_t
#include <iostream>

template<class T>
class linked_list 
{
public:
	// Constructors
	linked_list() = default;	

	linked_list(const linked_list & src)
	{

		//while(!empty()) pop_front();

		if(!src.empty())
		{
			auto current_src = src.head->next;
			auto current = head = new linked_list<T>::node_t(
					src.head->value);

			while(current_src)
			{

				auto next = new linked_list<T>::node_t(
						current_src->value);

				//Koppla ihop det nya elementet med det tidigare
				next->prev = current;

				//Koppla ihop det tidigare elementet med det nya
				current->next = next;

				//Iterera
				current 	= current->next;
				current_src 	= current_src->next;	
			}
			
			tail = current;

		}
	}

	// Destructor
	~linked_list()
	{
		while(!empty()) pop_back();	
	}

	// Tilldelningsoperatorn
	linked_list & operator=(const linked_list & rhs)
	{
		// Check för självtilldelning
		if(*this == rhs) return *this;

		// Töm listan
		while(!empty()) pop_back();

		if(!rhs.empty())
		{
			auto current_src = rhs.head->next;
			auto current = head = new linked_list<T>::node_t(
					rhs.head->value);

			while(current_src)
			{

				auto next = new linked_list<T>::node_t(
						current_src->value);

				//Koppla ihop det nya elementet med det tidigare
				next->prev = current;

				//Koppla ihop det tidigare elementet med det nya
				current->next = next;

				//Iterera
				current 	= current->next;
				current_src 	= current_src->next;	
			}
			
			tail = current;

		}

		return *this;
	}

	// Sammanfoga två listor
	linked_list & operator+=(const linked_list & rhs)
	{
		if(!rhs.empty())
		{
			auto current = rhs.head;
			auto end     = rhs.tail;

			while(current != end)
			{
				push_back(current->value); 
				current = current->next;
			}

			push_back(end->value);
		}

		return *this;
	}

	// Lägg till element i början eller slutet av listan
	void push_front(const T & value)
	{
		auto element = new linked_list<T>::node_t(value, head);

		if(head) head->prev = element;
		if(!tail) tail = element;	

		head = element;
	}

	void push_back(const T & value)
	{
		auto element = new linked_list<T>::node_t(value, 0, tail);

		if(tail) tail->next = element;
		if(!head)  head = element;

		tail = element;
	}

	// Ta bort element i början eller slutet av listan
	T pop_front()
	{
		linked_list<T>::node_t * new_head;

		if(head->next)
		{
			head->next->prev = 0;
			new_head = head->next;
		}
		else new_head = tail = 0;

		auto value = head->value;

		delete head;

		head = new_head;

		return value;
	}

	T pop_back()
	{
		linked_list<T>::node_t * new_tail;

		if(tail->prev)
		{
			tail->prev->next = 0;
			new_tail = tail->prev;
		}
		else new_tail = head = 0;

		auto value = tail->value;

		delete tail;

		tail = new_tail;

		return value;
	}

	// Hämta/modifiera element i listan
	T & front()
	{
		return head->value;
	}

	T & back()
	{
		return tail->value;
	}

	T & operator[](const size_t & index)
	{
		auto element = head;

		for(size_t i = 0; i < index; i++)
		{
			element = element->next;
		}

		return element->value;
	}

	// Ta bort element ur listan
	void remove(const size_t & index)
	{
		auto element = head;

		for(size_t i = 0; i < index; i++)
		{
			element = element->next;
		}

		if(element->next) element->next->prev = element->prev;
		else 
		{
			tail = element->prev;
			tail->next = 0;
		}

		if(element->prev) element->prev->next = element->next;
		else
		{
			head = element->next;
			head->prev = 0;
		}

		delete element;
	}

	// Utility
	size_t size() const
	{
		size_t size  = 0;
		auto current = head;

		while(current)
		{
			current = current->next;
			++size;
		}

		return size;
	}

	bool empty() const
	{
		return head == 0;
	}

	// Utmatning
	void print() const
	{
		std::cout << "{ ";

		auto iter = head;

		while(iter)
		{
			std::cout << iter->value << ' ';
			iter = iter->next;
		}

		std::cout << "}\n";
	}

	void print_reverse() const
	{
		std::cout << "{ ";

		auto iter = tail;

		while(iter)
		{
			std::cout << iter->value << ' ';
			iter = iter->prev;
		}

		std::cout << "}\n";
	}

private:
	
	struct node_t 
	{
		node_t(T value, node_t * next = 0, node_t * prev = 0) 
			: value(value), next(next), prev(prev)
		{};

		T value;
		node_t * next;
		node_t * prev;
	};

	node_t * head = 0;
	node_t * tail = 0;
};
