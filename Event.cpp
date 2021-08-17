#include "Event.h"
#include "Event.h"

//Event::Event()
//{
//}

Event::Event(double ts) : mTimestamp{ts}
{
}

Event::~Event()
{
}

bool Event::operator==(Event &event) const
{
    return this->operator()() - event() < EPSILON;
}

bool Event::operator<(Event &event) const
{
    return this->operator()() < event();
}

double Event::operator()() const
{
    return mTimestamp;
}

bool Event::isComposite() const
{
    return !mCompositeEvents.empty();
}

void Event::add(Event &&event) const
{
    if (event.isComposite()) {
        std::move(event.mCompositeEvents.begin(), event.mCompositeEvents.end(), std::back_inserter(mCompositeEvents));
        event.mCompositeEvents.clear();
    }
    mCompositeEvents.push_back(std::move(event));
}

Event::Event(Event &&o) noexcept: mCompositeEvents{std::move(o.mCompositeEvents)}, mTimestamp{o.mTimestamp}
{}

Event & Event::operator=(Event && o)
{
	this->mTimestamp = o.mTimestamp;
	this->mCompositeEvents = std::move(o.mCompositeEvents);
	return *this;
}

int Event::size()
{
    return 1 + mCompositeEvents.size();
}

//std::ostream &operator<<(std::ostream &out, const Event &c)
//{
//    out << c();
//    return out;
//}

Event Event::popFront()
{
    if (!isComposite()) {
        throw std::runtime_error("popping from non composite event is not possible");
    };
    Event toReturn{std::move(mCompositeEvents.back())};
    mCompositeEvents.pop_back();
    return toReturn;
}
