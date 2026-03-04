#pragma once
#include "Functions.h"
#include "List.h"

template <typename T>
class List;

template <typename T>
class List<T>::Iterator
{
private:
	Node<T>* current;
public:
	friend class List<T>;
	Iterator(Node<T>* current) : current(current){}
	Node<T> operator*()
	{
		return *current;
	}
	Iterator& operator++()
	{
		this->current = this->current->next;
		return *this;
	}
	Iterator operator++(int)
	{
		Iterator temp = *this;
		this->current = this->current->next;
		return temp;
	}
	Iterator& operator--()
	{
		this->current = this->current->prev;
		return *this;
	}
	Iterator operator--(int)
	{
		Iterator temp = this;
		this->current = this->current->prev;
		return temp;
	}
	bool operator==(const Iterator& it)
	{
		return this->current == it.current;
	}
	bool operator!=(const Iterator& it)
	{
		return !(this->current == it.current);
	}
	Iterator operator+=(int value)
	{
		for (int i = 0; i < value; ++i)
		{
			this->current = this->current->next;
		}
		return *this;
	}
};