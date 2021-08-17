#pragma once
#include <deque>
#include "Event.h"
#include "AlqList.h"
#include <deque>
#include <memory>

class Rung {

public:
	double mBucketWidth;
	double rStart;
	double rCurrent;
	int mBucketCount = 0;
	std::vector<AlqList> buckets;
	int mMinBucket = 0;
	int mMaxBucket = 0;

	Rung(double rungWidth, double bucketWidth);
	void add(Event &&event);

	//TODO toString() method not written
private:
};

