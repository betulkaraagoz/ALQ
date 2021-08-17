#include "AlqList.h"
#include <functional>
#include <algorithm>

AlqList::AlqList() : AlqList(false)
{}

AlqList::AlqList(bool mGrouping) : mGrouping(mGrouping)
{}

void AlqList::moveFrom(AlqList &from)
{
    std::move(from.mList.begin(), from.mList.end(), back_inserter(mList));
    from.mList.clear();
}

bool AlqList::empty()
{
    return mList.empty();
}

Event AlqList::popFront()
{
    Event toReturn{std::move(mList.front())};
    mList.pop_front();
    return toReturn;
}

Event AlqList::popBack()
{
    Event toReturn{std::move(mList.back())};
    mList.pop_back();
    return toReturn;
}

int AlqList::size()
{
    return mList.size();
}

int AlqList::totalSize()
{
	int toReturn = 0;
	for (auto it = mList.begin(); it != mList.end(); ++it)
	{
		Event& e = *it;
		toReturn += e.size();
	}	
	return toReturn;
}

void AlqList::add(Event &&event)
{
    mList.push_back(std::move(event));
}

void AlqList::addOrdered(Event &&event)
{
    for (auto& it = mList.begin(); it != mList.end(); it++) {
        Event &listEvent = *it;
        if (event() < listEvent()) {
            mList.insert(it, std::move(event));
            break;
        } else if (event() == listEvent()) {
            if (mGrouping) {
                listEvent.add(std::move(event));
            } else {
                mList.insert(it, std::move(event));
            }
            break;
        }
    }
	mList.push_back(std::move(event));
}

Event &AlqList::front()
{
    return mList.front();
}

Event &AlqList::back()
{
    return mList.back();
}

void AlqList::addFront(Event &&event)
{
    mList.push_front(std::move(event));
}

AlqList &AlqList::operator=(AlqList &&other) noexcept
{
    mGrouping = other.mGrouping;
    mList = std::move(other.mList);
    return *this;
}

void AlqList::sort()
{
	static std::function<bool(const Event&, const Event&)> predicate = [](const  Event& left, const Event& right) {
		return left() < right();
	};

	//std::sort(mList.begin(), mList.end(), predicate);

    mList.sort(predicate);
    // TODO @BK std::list is not sortable
//    std::sort(mList.begin(), mList.end(),);

}

AlqList::~AlqList() = default;
