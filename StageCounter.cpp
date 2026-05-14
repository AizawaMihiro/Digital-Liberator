#include "StageCounter.h"

namespace 
{
	int count_;
}

void StageCounter::StageCounter()
{
	count_ = 0;
}


void StageCounter::CountUp()
{
	count_++;
}

void StageCounter::CountDown()
{
	count_--;
}

void StageCounter::Reset()
{
	count_ = 0;
}

int StageCounter::GetCount()
{
	return count_;
}
