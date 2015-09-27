#pragma once
#include <iostream>
#include <stdlib.h>

template<class ListType> class List
{
	ListType * list;
	int numItems;
public:
	List(){
		list = static_cast<ListType*>(malloc(sizeof(ListType)));
		numItems = 0;
	};

	void add(ListType item){
		numItems++;
		list = static_cast<ListType*>(realloc(list, sizeof(ListType)*numItems));
		list[numItems - 1] = item;
	};

	void add_multiple(ListType * firstNew, int numNew){
		numItems += numNew;
		list = static_cast<ListType*>(realloc(list, sizeof(ListType)*numItems));
		memcpy((list + (numItems - numNew)), firstNew, sizeof(ListType) * numNew);
	};

	ListType * start(){
		return list;
	};

	void remove(ListType item){
		int index;
		for (index = 0; index < numItems; index++){
			if (list + index == &item)
				break;
		}
		if (index == numItems){ // Not in the list
			std::cout << "Item cannot be removed because it wasn't found" << std::endl;
		}
		else if (index != (numItems - 1)){	// Item is not the last item in the list
			list[index] = list[numItems - 1]; // copy the last item into the first's place
			numItems--;	// Mark the last item as not needed
			list = static_cast<ListType*>(realloc(list, sizeof(ListType)*numItems));	// Reallocate and release the last items location
		}
		else {	// The item is the last (we just need to reallocate discarding the last item)
			numItems--;	// Mark the last item as not needed
			list = static_cast<ListType*>(realloc(list, sizeof(ListType)*numItems));	// Reallocate and release the last items location
		}
		return;
	};
	
	ListType operator[](const int index){
		if (index >= numItems){
			std::cout << "Item index is greater than the number of items in this list" << std::endl;
			return ListType();
		}
		return list[index];
	};
	
	inline int count(){
		return numItems;
	};

	virtual ~List(){
		free(list);
	};
};