#pragma once
using namespace std;
#include "Event.h"
#include <memory>

class EventList{
	public:
		virtual void add(std::unique_ptr<Event>& e) = 0;
		virtual void addInOrder(std::unique_ptr<Event>& e) = 0;
		//CHECK: FINAL KEYWORD CONVERTED TO CONST
		virtual void add(std::unique_ptr<Event>& e, const bool group) = 0;
		virtual void addInOrder(std::unique_ptr<Event>& e, const bool group) = 0;
		virtual void addFirst(std::unique_ptr<Event>& e) = 0;
		virtual std::unique_ptr<Event>& getFirstAtomic() = 0;
		virtual std::unique_ptr<Event>& getFirst() = 0;
		virtual std::unique_ptr<Event>& getLast() = 0;
		virtual std::unique_ptr<Event>& removeFirstAtomic() = 0;
		virtual std::unique_ptr<Event>& removeFirst() = 0;
		//long getMinTS();
		//long getMaxTS();
		virtual int eventCount() = 0;
		virtual int nodeCount() = 0;
		virtual void sort(bool group) = 0;
		virtual bool isEmpty() = 0;
		virtual ~EventList();
};

