#pragma once
#include <deque>
#include <memory>
#include "Event.h"
#include "Rung.h"
#include "AlqList.h"

class ALadderQueue
{
public:
    ALadderQueue(bool grouping, bool upGrowing, bool smartSpawn, int threshold = 50, int topThreshold = 50 * 48, int maxRungs = 10);
    ALadderQueue();
	int totalSize();
    ~ALadderQueue();
	void printStatus();
    void enqueue(Event &&event);
    Event dequeue();
    int getSize() const;

private:
    //TOP DATA STRUCTURES
    AlqList top;
    std::list<Rung> mRungs;
    //bottom is used as deque because we may insert into beginning
    AlqList bottom;
    bool mGrouping;
    bool mUpGrowing;
    bool mSmartSpawn;
    int THRES = 50; //threshold to start spawning in Bottom tier
    int THRES_TOP = 48 * THRES; //threshold to indicate max number of events in Top tier
    int MAX_RUNGS = 10;
    int MAX_RUNGS_UP = 3;
    int mSize = 0;
    double mMaxTS = -1;
    double mMinTS = std::numeric_limits<double>::max();
    double mMean = 0;
    double mVariance = 0;
    const double alpha = 1.7;
    double mUpper = 0;
    bool mUpdated = false;
    double topStart = -1.0;
    int rungUsed = 0;
    int rungUpGrowing = 0;
    int topInsert = 0;
    int rungInsert = 0;
    int bottomInsert = 0;
    int upGrowingCount = 0; //number of mUpGrowing performed
    int smartSpawnCount = 0; //number of mSmartSpawn performed

    std::unique_ptr<Rung> createNewRungFromTop();
    std::unique_ptr<Rung> createNewRungFromBottom();
    std::unique_ptr<Rung> createNewRungFromBucket(AlqList &bucket);
    double getMaxTop();
    void transferTopToBottom();
    void smartTransferTopToBottom();
    bool transferTopToRung();
    int recurseRung();
    void checkRungs();
    void smartSpawnStatsTopEnqueue(double ts, int occurrences, int newSize);
    void handleUpGrowing();
};

