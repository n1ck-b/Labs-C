#pragma once
#include "Functions.h"
#include "Node.h"
#include <type_traits>

template <typename T>
class List
{
private:
	Node<T>* first = nullptr;
	Node<T>* last = nullptr;
	int sizeOfList = 0;
public:
	class Iterator;
	Iterator begin()
	{
		return Iterator(first);
	}
	Iterator end()
	{
		return Iterator(last->next);
	}
	~List()
	{
		while (sizeOfList != 0)
		{
			popBack();
		}
	}
	int size() const
	{
		return sizeOfList;
	}
	bool isEmpty() const
	{
		return first == nullptr;
	}
	void pushBack(T& newCar, int id)
	{
		Node<T>* temp = new Node<T>;
		temp->next = nullptr;
		temp->prev = nullptr;
		if (isEmpty())
		{
			first = temp;
			last = temp;
		}
		else
		{
			last->next = temp;
			temp->prev = last;
			last = temp;
		}
		temp->car.setYear(newCar.getYear());
		temp->car.setMileage(newCar.getMileage());
		temp->car.setPrice(newCar.getPrice());
		temp->car.setBrand(newCar.getBrand());
		temp->car.setModel(newCar.getModel());
		temp->id = id;
		if constexpr (std::is_same_v<T, CombustionEngineCar> || std::is_same_v<T, HybridEngineCar>)
			temp->car.setFuelTankCapacity(newCar.getFuelTankCapacity());
		if constexpr (std::is_same_v<T, ElectricEngineCar> || std::is_same_v<T, HybridEngineCar>)
			temp->car.setBatteryCapacity(newCar.getBatteryCapacity());
		if constexpr (std::is_same_v<T, HybridEngineCar>)
			temp->car.setHybridType(newCar.getHybridType());
		sizeOfList++;
	}
	void popBack()
	{
		if (first->next == nullptr)
		{
			delete first;
			first = last = nullptr;
		}
		else
		{
			Node<T>* temp = last;
			last = last->prev;
			delete temp;
			last->next = nullptr;
		}
		sizeOfList--;
	}
	void insert(Iterator it, T& Car, int id)
	{
		if (size() > 1 && it != first)
		{
			Node<T>* newNode = new Node<T>;
			newNode->next = nullptr;
			newNode->prev = nullptr;
			Node<T>* prevNode = it.current->prev;
			it.current->prev = newNode;
			newNode->next = it.current;
			prevNode->next = newNode;
			newNode->prev = prevNode;
			newNode->car.setYear(Car.getYear());
			newNode->car.setMileage(Car.getMileage());
			newNode->car.setPrice(Car.getPrice());
			newNode->car.setBrand(Car.getBrand());
			newNode->car.setModel(Car.getModel());
			newNode->id = id;
			if constexpr (std::is_same_v<T, CombustionEngineCar> || std::is_same_v<T, HybridEngineCar>)
				newNode->car.setFuelTankCapacity(Car.getFuelTankCapacity());
			if constexpr (std::is_same_v<T, ElectricEngineCar> || std::is_same_v<T, HybridEngineCar>)
				newNode->car.setBatteryCapacity(Car.getBatteryCapacity());
			if constexpr (std::is_same_v<T, HybridEngineCar>)
				newNode->car.setHybridType(Car.getHybridType());
			sizeOfList++;
		}
		else
			pushBack(Car, id);
	}
	void clear()
	{
		while (sizeOfList != 0)
		{
			popBack();
		}
	}
};