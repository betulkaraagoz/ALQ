#include "AtomicEvent.h"

AtomicEvent::AtomicEvent(double time) : Event( time )
{

}


bool AtomicEvent::isComposite()
{
	return false;
}

AtomicEvent::~AtomicEvent()
{
}
