#include "Rung.h"
#include <iostream>
#include <cmath>
#include <algorithm>

Rung::Rung(double rungWidth, double bucketWidth) : buckets(static_cast<int>(std::ceil(rungWidth / bucketWidth))+1)
{
    //TODO initial mSize is given with reserve;
	//int size = (int)(rungWidth / bucketWidth);
	//buckets.resize(size + 1);
	
    mBucketWidth = bucketWidth;
    mBucketCount = mMinBucket = mMaxBucket = 0;
}

void Rung::add(Event &&event)
{
    //std::cout << evt->getTimeStamp() << "\n";
    int k;
    if (mBucketWidth > 0) {
        k = (int) ((event.operator()() - rStart) / mBucketWidth);
	
	

        if (buckets.at(k).empty()) { // TODO buraya bak�lacak
            //TODO is it already allocated?
            //bucket[k] = new std::deque<Event>();
            mBucketCount++;
        }


        buckets.at(k).add(std::move(event));

        if (mBucketCount == 1) {
            mMinBucket = mMaxBucket = k;
        } else {
            mMinBucket = std::min(mMinBucket, k);
            mMaxBucket = std::max(mMaxBucket, k);
        }
    }
    /*else {
        assert(false);
    }*/
}
