#pragma once

#include "LinkedEventList.h"
#include "Event.h"
#include <cassert>
#include <vector>
#include <memory>
using namespace std;

//TODO composite event second constructor + add remove methods for list
class CompositeEvent: public Event{
	
	private:
		//std::vector<std::unique_ptr<Event>> list;
		LinkedEventList list;
		double timestamp;


	public:
		CompositeEvent(double ts);
		CompositeEvent(std::unique_ptr<Event>&& left, std::unique_ptr<Event>&& right);
		CompositeEvent& asComposite();
		bool isComposite();
		int size();
		void add(std::unique_ptr<Event>&& event);
		void addFirst(std::unique_ptr<Event>&& event);
		std::unique_ptr<Event>&& getFirst();
		std::unique_ptr<Event>&& remove();

		~CompositeEvent();
};

