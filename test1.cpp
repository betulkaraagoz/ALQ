#include <deque>
#include <iostream>
#include "Event.h"





int main()
{
	
	std::deque<Event> list;
	
	list.push_back(Event{1.1});
	list.push_back(Event{1.1});
	list.push_back(Event{1.1});
	list.push_back(Event{1.1});
	list.push_back(Event{1.1});
	list.push_back(Event{1.1});
	list.push_back(Event{1.1});


	auto& it =  list.cbegin();
	++it;
	++it;

	list.insert(it, Event{2.2});


	std::cout << "";

}



