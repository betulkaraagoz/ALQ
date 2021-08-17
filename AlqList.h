#pragma once

#include <list>
#include <deque>
#include "Event.h"

class AlqList
{

public:
    AlqList();
    explicit AlqList(bool mGrouping);
    AlqList &operator=(AlqList &&other) noexcept;
	AlqList(const AlqList& a) = delete;

    ~AlqList();
    int size();
	int totalSize();
    void moveFrom(AlqList &from);
    bool empty();
    void sort();
    Event popFront();
    Event &front();
    Event popBack();
    Event &back();
    void add(Event &&event);
    void addOrdered(Event &&event);
    void addFront(Event &&event);
private:
    bool mGrouping;
    std::list<Event> mList;
};


