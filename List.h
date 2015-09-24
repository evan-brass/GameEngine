#pragma once

template<typename listType> class List
{
protected:
	listType * list;
	int numItems;
public:
	List();

	void add(listType item);
	void remove(listType item);

	listType operator[](const int index);

	inline int count();

	virtual ~List();
};