#include "List.h"
#include <iostream>
#include <stdlib.h>

template<typename listType>
List<listType>::List()
{
	list = static_cast<listType*>(malloc(sizeof(listType)));
	numItems = 0;
}

template<typename listType>
void List<listType>::add(listType item)
{
	numItems++;
	list = static_cast<listType*>(realloc(list, sizeof(listType)*numItems));
	list[numItems - 1] = item;
}

template<typename listType>
void List<listType>::remove(listType item)
{
	int index;
	for (index = 0; index < numItems; index++){
		if (list[index] == item)
			break;
	}
	if (index == numItems){ // Not in the list
		std::cout << "Item cannot be removed because it wasn't found" << std::endl;
	}
	else if (index != (numItems - 1)){	// Item is not the last item in the list
		list[index] = list[numItems - 1]; // copy the last item into the first's place
		numItems--;	// Mark the last item as not needed
		list = static_cast<listType*>(realloc(list, sizeof(listType)*numItems));	// Reallocate and release the last items location
	}
	else {	// The item is the last (we just need to reallocate discarding the last item)
		numItems--;	// Mark the last item as not needed
		list = static_cast<listType*>(realloc(list, sizeof(listType)*numItems));	// Reallocate and release the last items location
	}
	return;
}

template<typename listType>
listType List<listType>::operator[](const int index)
{
	if (index >= numItems){
		std::cout << "Item index is greater than the number of items in this list" << std::endl;
		return 0;
	}
	return list[index];
}

template<typename listType>
inline int List<listType>::count()
{
	return numItems;
}

template<typename listType>
List<listType>::~List()
{
	free(list);
}

	/*
// Testing code
int main(){
	List<int> integerList;
	integerList.add(7);
	std::cout << integerList.count() << std::endl;
	std::cout << integerList[0] << std::endl << std::endl;
	integerList.add(6);
	std::cout << integerList.count() << std::endl;
	std::cout << integerList[0] << std::endl;
	std::cout << integerList[1] << std::endl << std::endl;
	integerList.add(9);
	std::cout << integerList.count() << std::endl;
	std::cout << integerList[0] << std::endl;
	std::cout << integerList[1] << std::endl;
	std::cout << integerList[2] << std::endl << std::endl;
	integerList.remove(6);
	std::cout << integerList.count() << std::endl;
	std::cout << integerList[0] << std::endl;
	std::cout << integerList[1] << std::endl << std::endl;

	// Access an item that doesn't exist
	std::cout << integerList[2] << std::endl;
	// Try to remove an item that isn't in the list
	integerList.remove(23);
}
//	*/