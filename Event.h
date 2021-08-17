#pragma once
#include <vector>
#include <iostream>

constexpr double EPSILON{0.000001};

class Event {
public:
    Event() = delete;
    Event(Event&& o) noexcept;
	Event& operator=(Event&& o);
    explicit Event(double ts);
	Event(const Event& other) = delete;
	Event& operator=(const Event& other) = delete;

    ~Event();
    Event popFront();

    void add(Event&& event) const;
    double operator()() const;
    bool isComposite() const;
    int size();
    bool operator==(Event &event) const;
    bool operator<(Event &event) const;
    //friend std::ostream & operator << (std::ostream &out, const Event &c);
private:
    double mTimestamp;
	mutable std::vector<Event> mCompositeEvents;
};

