#include "CompositeEvent.h"

CompositeEvent::CompositeEvent(double ts) : Event(ts)
{
	
}

CompositeEvent::CompositeEvent(std::unique_ptr<Event>&& left, std::unique_ptr<Event>&& right)
{
	assert(left->getTimeStamp() == right->getTimeStamp());
	timestamp = left->getTimeStamp();
	left.list.enqueueAndEmpties(right.list);
	list = left->list;
}


CompositeEvent & CompositeEvent::asComposite()
{
	return *this;
}

bool CompositeEvent::isComposite()
{
	return true;
}

int CompositeEvent::size()
{
	return list.size();
}

CompositeEvent::~CompositeEvent()
{
}
