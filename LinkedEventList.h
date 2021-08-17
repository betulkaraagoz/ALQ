#pragma once
#include <list>
#include "Event.h"
#include "CompositeEvent.h"


class LinkedEventList : public std::list<std::unique_ptr<Event>>
{
private:
	
	std::list<std::unique_ptr<Event>> list;
	int eventCount;

public:
	LinkedEventList();
	void add(std::unique_ptr<Event>&& event) ;
	void add(std::unique_ptr<Event>&& event, bool grouping ) ;
	void addOrdered(std::unique_ptr<Event>&& event, bool grouping);
	void addFirst(std::unique_ptr<Event>&& event);

	std::unique_ptr<Event> getFirst();
	std::unique_ptr<Event> getLast();
	std::unique_ptr<Event> getFirstAtomic();
	
	std::unique_ptr<Event> removeFirstAtomic();
	std::unique_ptr<Event> removeFirst();
	//std::unique_ptr<Event> removeNode(); //same as remove first without composite part

	void enqueueAndEmpties(LinkedEventList& other);

	int getEventCount();
	void sort(bool group);
	bool isEmpty();

};

