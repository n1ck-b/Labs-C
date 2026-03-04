#pragma once
#include "Functions.h"

template <typename T>
class Node
{
public:
	int id;
	T car;
	Node* next;
	Node* prev;
};