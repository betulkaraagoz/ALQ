#include "ALadderQueue.h"
#include <cmath>
#include <memory>
#include <algorithm>

ALadderQueue::ALadderQueue(bool grouping, bool upGrowing, bool smartSpawn, int threshold, int topThreshold, int maxRungs)
    : mGrouping(grouping),
    mUpGrowing(upGrowing),
    mSmartSpawn(smartSpawn),
    THRES(threshold),
    THRES_TOP(topThreshold),
    MAX_RUNGS(maxRungs),
    top(grouping),
    bottom(grouping)
{
}

ALadderQueue::ALadderQueue() : ALadderQueue(true, true, true)
{
}


int ALadderQueue::totalSize() {

	int total = 0;
	auto it = mRungs.begin();
	for (auto it = mRungs.begin(); it != mRungs.end(); ++it)
	{
		Rung& rung = *it;
		for (size_t j = 0; j < rung.buckets.size(); j++)
		{
			AlqList& bucket = rung.buckets[j];
			total += bucket.totalSize();
		}
	}
	total += top.totalSize();
	total += bottom.totalSize();
	return total;
}

void ALadderQueue::printStatus() 
{
	std::cout << "Size:" << mSize << " - Top:" << top.size() << " - Bottom:" << bottom.size() << " - Total:" << totalSize() << "\n";

	int i = 0;

	auto it = mRungs.begin();
	for (auto it = mRungs.begin(); it != mRungs.end(); ++it)
	{
		Rung& rung = *it;
		for (size_t j = 0; j < rung.buckets.size(); j++)
		{
			AlqList& bucket = rung.buckets[j];
			if (bucket.size() > 0) {
				std::cout << "Rung " << (i + 1) << ", Bucket " << (j + 1) << " is not empty\n";
			}
		}
		++i;
	}

}



std::unique_ptr<Rung> ALadderQueue::createNewRungFromBottom()
{
    if (mRungs.size() >= MAX_RUNGS) {
        return nullptr;
    }
    double bw;
    double w;
    double rStart;

    auto &lastRung = mRungs.back();
    rStart = bottom.front()();
    w = lastRung.rCurrent - rStart;
    bw = w / THRES;

    if (bw == 0) {
        return nullptr;
    }


    auto rung = std::make_unique<Rung>(w, bw);
    rung->rCurrent = rung->rStart = rStart;
    return rung;
}

std::unique_ptr<Rung> ALadderQueue::createNewRungFromTop()
{
    if (mRungs.size() >= MAX_RUNGS) {
        return nullptr;
    }

    double bw;
    double w;
    double rStart;

    int rungSize = mRungs.size();
	 // TODO @BK
    // assert(rungSize == 0 || mUpGrowing);
    double min = rungSize > 0 ? topStart : mMinTS;

    w = getMaxTop() - min; // TODO @BK +1 in Java implementation
    double nc = top.size();
    bw = w / nc;
 /*   if (std::fmod(w, THRES) != 0) {
        bw++;
    }*/
    if (bw == 0) {
        return nullptr;
    }
    rStart = min;
    auto rung = std::make_unique<Rung>(w, bw);
    rung->rCurrent = rung->rStart = rStart;
    return rung;
}

std::unique_ptr<Rung> ALadderQueue::createNewRungFromBucket(AlqList &bucket)
{
    if (mRungs.size() >= MAX_RUNGS) {
        return nullptr;
    }

    if (bottomInsert == 0 && mRungs.empty() && top.size() == 1) {
        return nullptr;
    }

    double bw;
    double w;
    double rStart;

    auto &lastRung = mRungs.back();
    w = lastRung.mBucketWidth;
    bw = w / THRES;

    if (bw == 0) {
        return nullptr;
    }

    rStart = lastRung.rCurrent;

    auto rung = std::make_unique<Rung>(w, bw);
    rung->rCurrent = rung->rStart = rStart;
    return rung;
}

double ALadderQueue::getMaxTop()
{
    //TODO @BK check this it is ok
    if (top.size() == 1) {
        return mMaxTS;
    }

    if (mSmartSpawn) {
        if (mUpdated) {
            return mUpper;
        }
        mUpper = mMean + alpha * sqrt(mVariance) / (top.size() - 1);
        if (mUpper > mMaxTS) {
            mUpper = mMaxTS;
        }
        if (mUpper < mMinTS) {
            mUpper = mMinTS;
        }
        mUpdated = true;
        return mUpper;
    }
    return mMaxTS;
}

void ALadderQueue::transferTopToBottom()
{
    topStart = mMaxTS;
    bottom.moveFrom(top);
    // TODO @BK bottom may need sorting
    mMinTS = -1;
    mMaxTS = -1;
    if (mSmartSpawn) {
        mMean = 0;
        mVariance = 0;
    }
}

void ALadderQueue::smartTransferTopToBottom()
{
    //TODO	if (bottom == nullptr)
//    if (bottom.empty())
//        bottom = std::move(std::deque<std::unique_ptr<Event>>());

    double upper = getMaxTop();
    mMean = 0;
    mVariance = 0;
    double max = std::numeric_limits<double>::max();
    double min = std::numeric_limits<double>::min();
    double maxMoved = -1.0;

    AlqList newTop{mGrouping};

    while (!top.empty()) {
        Event eventPoppedFromTop = top.popFront();

        double currTs = eventPoppedFromTop();
        if (currTs <= upper) {
            if (currTs > maxMoved) {
                maxMoved = currTs;
            }
            bottomInsert++;
            bottom.addOrdered(std::move(eventPoppedFromTop));
            // TODO @BK bottom.add(eventPoppedFromTop, mGrouping);
        } else {
            max = std::max(currTs, max);
            min = std::min(currTs, min);
            int size = eventPoppedFromTop.size();
            newTop.add(std::move(eventPoppedFromTop));
            // TODO @BK newTop.add(eventPoppedFromTop, mGrouping);
            smartSpawnStatsTopEnqueue(currTs, size, newTop.size());
        }
    }
    top = std::move(newTop);
    if (top.empty()) {
        mMinTS = -1L;
        mMaxTS = -1L;
    } else {
        mMinTS = min;
        mMaxTS = max;
    }
    topStart = maxMoved;
}

bool ALadderQueue::transferTopToRung()
{
    std::unique_ptr<Rung> rung = createNewRungFromTop();
    if (rung == nullptr) {
        return false;
    }
    double upper = getMaxTop();
    if (mSmartSpawn) {
        mMean = 0;
        mVariance = 0;
        double max = std::numeric_limits<double>::max();
        double min = std::numeric_limits<double>::min();
        double maxRung = -1;
        AlqList newTop{mGrouping};

        while (!top.empty()) {
            Event event = top.popFront();
            double currTs = event();
            if (currTs <= upper) {
                if (currTs > maxRung) {
                    maxRung = currTs;
                }
                rung->add(std::move(event));
            } else {
                max = std::max(currTs, max);
                min = std::min(currTs, min);

                //TODO @BK
                //newTop.add(evt, mGrouping);
                int size = event.size();
                newTop.add(std::move(event));
                smartSpawnStatsTopEnqueue(currTs, size, newTop.size());
            }
        }

        if (newTop.size() > 0) {
            smartSpawnCount++;
        }

        top = std::move(newTop);

        if (top.size() == 0) {
            mMinTS = std::numeric_limits<double>::min();
            mMaxTS = std::numeric_limits<double>::max();
        } else {
            mMinTS = min;
            mMaxTS = max;
        }
        topStart = maxRung;
    } else {
        topStart = mMaxTS;
        while (!top.empty()) {
            rung->add(std::move(top.popFront()));
        }
        mMinTS = std::numeric_limits<double>::min();
        mMaxTS = std::numeric_limits<double>::max();
    }
    mRungs.push_front(std::move(*rung));
    if (mRungs.size() > rungUsed) {
        rungUsed = mRungs.size();
    }
    return true;
}

int ALadderQueue::recurseRung()
{
    if (mRungs.empty()) {
        throw std::runtime_error("no rungs");
    }

    auto &lastRung = mRungs.back();
    int k = lastRung.mMinBucket;

    //TODO k <= lastRung->mMaxBucket && (lastRung->bucket[k] == null || lastRung->bucket[k].nodeCount() == 0)
    while (k <= lastRung.mMaxBucket && lastRung.buckets.at(k).size() == 0) {
        ++k;
    }

    lastRung.rCurrent = lastRung.rStart + (lastRung.mBucketWidth * (k));

    //NEDEN?
    //assert(k > lastRung->mMaxBucket);

    AlqList &bucket_k = lastRung.buckets.at(k);

	//TODO: BW NEDEN 1'DEN BÜYÜK OLMALI
    //TODO      if (bucket_k.size() > THRES && lastRung.mBucketWidth > 1) {
    if (bucket_k.size() > THRES ) {
        std::unique_ptr<Rung> newRung = createNewRungFromBucket(bucket_k);
	
        if (newRung == nullptr) {
            lastRung.mMinBucket = k;
            return k; // It was not possible to create a new rung
        }

        if (newRung->mBucketWidth == 0) {
            lastRung.mMinBucket = k;
            return k;// It was not possible to create a new rung
        }

	
	
        while (bucket_k.size() > 0) {
            newRung->add(std::move(bucket_k.popFront()));
        }


        lastRung.mBucketCount--;
        lastRung.mMinBucket = k + 1;

        lastRung.rCurrent = lastRung.rStart + lastRung.mMinBucket * lastRung.mBucketWidth;

        mRungs.push_back(std::move(*newRung));

        if (mRungs.size() > rungUsed) {
            rungUsed = mRungs.size();
        }
	
        return recurseRung();
    } else {
        lastRung.mMinBucket = k;
        return k;
    }
}

void ALadderQueue::checkRungs()
{
    // TODO d�ng� gezerken silme yokk
    while (!mRungs.empty()) {
        auto &lastRung = mRungs.back();

        /*while (lastRung->mMinBucket <= lastRung->mMaxBucket && (lastRung->bucket[lastRung->mMinBucket] == null
            || lastRung->bucket[lastRung->mMinBucket].mSize() == 0))
            ++lastRung->mMinBucket;*/

        while (lastRung.mMinBucket <= lastRung.mMaxBucket &&
               lastRung.buckets.at(lastRung.mMinBucket).size() == 0)
            ++lastRung.mMinBucket;

        if (lastRung.mMinBucket > lastRung.mMaxBucket) {
            mRungs.pop_back();
        } else {
            break;
        }
    }

}

void ALadderQueue::smartSpawnStatsTopEnqueue(double ts, int occurrences, int newSize)
{
    double d = ts - mMean;
    mMean = mMean + occurrences * (d / newSize);
    mVariance = mVariance + occurrences * d * (ts - mMean);
    mUpdated = false;
}

void ALadderQueue::handleUpGrowing()
{
    if (top.size() >= THRES_TOP && rungUpGrowing < MAX_RUNGS_UP) {
        if (transferTopToRung()) {
            rungUpGrowing++;
            upGrowingCount++;
        }
    }
}

void ALadderQueue::enqueue(Event &&event)
{
    if (event.isComposite()) {
        throw std::runtime_error(std::string{"enqueued event can not be composite"});
    }
    ++mSize;
    const double timestamp = event();

    //insert into top
    if (timestamp > topStart) {
        topInsert++;
        if (top.empty()) {
            //only element is timestamp
            mMinTS = mMaxTS = timestamp;
        } else {
            //update variables
            mMaxTS = std::max(mMaxTS, timestamp);
            mMinTS = std::min(mMinTS, timestamp);
        }
		//std::cout << "insert into top: "<< event.operator()() << "\n";

        top.add(std::move(event));
        if (mSmartSpawn) {
            smartSpawnStatsTopEnqueue(timestamp, 1, top.size());
        }
        if (mUpGrowing) {
            handleUpGrowing();
        }
        return;
    }
    if (!bottom.empty() && timestamp < bottom.front()()) {
		//std::cout << "insert into bottom: " << event.operator()() << "\n";
        bottom.addFront(std::move(event));
        bottomInsert++;
        return;
    }
    for (auto &rung : mRungs) {
        if (timestamp > rung.rCurrent) {
            //insert into the rung
			//std::cout << "insert into rung: " << event.operator()() << "\n";
            rung.add(std::move(event));
            rungInsert++;
            return;
        }
    }

    if (!bottom.empty() && bottom.size() > THRES && !mRungs.empty() && bottom.front()() != bottom.back()()) {
        std::unique_ptr<Rung> rung = createNewRungFromBottom();

        if (rung != nullptr) {
            // assert(rung == mRungs.back()); TODO @BK check this

            while (bottom.size() != 0) {
                rung->add(std::move(bottom.popFront()));
            }
            rung->add(std::move(event));
            mRungs.push_back(std::move(*rung));
            rungUsed = std::max(rungUsed, static_cast<int>(mRungs.size()));
            rungInsert++;
            return;
        }
        //not able to crate a rung
    }

	//std::cout << "insert into bottom: " << event.operator()() << "\n";
    bottomInsert++;
    bottom.addOrdered(std::move(event));
}

Event ALadderQueue::dequeue()
{
	//std::cout << "bottom: " << bottom.size() << "\n";
	--mSize;
    if (bottom.empty()) {
		//std::cout << "here" << "\n";
        if (mRungs.empty()) {
            double upper = getMaxTop();
            if ((upper - mMinTS) / THRES_TOP == 0) {
                if (upper == mMaxTS) {
                    // transfers entire top
                    transferTopToBottom();
                } else {
                    smartTransferTopToBottom();
                }
            } else {
                if (!transferTopToRung()) {
                    if (upper == mMaxTS) {
                        // transfers entire top
                        transferTopToBottom();
                    } else {
                        smartTransferTopToBottom();
                    }
                }
            }
        }


        if (!mRungs.empty()) {
            int k = recurseRung();
			
            auto &lastRung = mRungs.back();
            auto &bucketData = lastRung.buckets.at(k);
            bottom.moveFrom(bucketData);

            lastRung.mBucketCount--;
            lastRung.mMinBucket++;

            if (k == lastRung.mMaxBucket) {
                mRungs.pop_back();
                rungUsed = mRungs.size();
                if (rungUsed < MAX_RUNGS_UP && rungUpGrowing > rungUsed) {
                    rungUpGrowing = rungUsed;
                }
            } else {
                lastRung.rCurrent = lastRung.rStart + lastRung.mMinBucket * lastRung.mBucketWidth;
            }
            checkRungs();
        }
	
        bottom.sort();
    }
	//std::cout << "Size:" << mSize << " - Top:" << top.size() << " - Rung:" << mRungs.size() << " - Bottom:" << bottom.size() << "\n";

    if (!bottom.empty()) {
	
        if(bottom.front().isComposite()){
            return bottom.front().popFront();
        }
        return bottom.popFront();
	}
	else {
		
	}

    throw std::runtime_error("dequeuing from empty queue");

    //TODO @BK SORT FUNC NEED TO BE IMPLEMENTED
    //bottom.sort(mGrouping);
    /*	std::sort(std::begin(bottom), std::end(bottom), [&](const auto& l, const auto& r) {
            return l->getTimeStamp() < r->getTimeStamp();
        });*/
}

int ALadderQueue::getSize() const
{
    return mSize;
}

ALadderQueue::~ALadderQueue() = default;


