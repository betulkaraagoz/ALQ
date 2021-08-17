#pragma once
#include "Event.h"
#include "CompositeEvent.h"

class AtomicEvent : public Event{
	public:
		AtomicEvent(double time);
		bool isComposite();
		~AtomicEvent();

};

