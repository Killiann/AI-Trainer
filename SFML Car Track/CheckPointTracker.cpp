#include "CheckPointTracker.h"

CheckPointTracker::CheckPointTracker(CheckPointManager& manager) {
	checkpoints = manager.GetCheckpoints();

	//setup segments
	for (int i = 0; i < checkpoints.size() - 1; ++i) {
		segments.emplace_back(i, checkpoints[i], checkpoints[i + 1]);
	}
	segments.emplace_back(checkpoints.size() - 1, checkpoints[checkpoints.size() - 1], checkpoints[0]);
}

void CheckPointTracker::StartTracking() {
	currentSegmentID = 0;
	segmentClock.restart();
	lapClock.restart();
}

void CheckPointTracker::CompleteSegment() {
	if (!hasStarted) {
		hasStarted = true;
		StartTracking();
	}
	else {
		segments[currentSegmentID].timings.emplace_back(segmentClock.restart().asMilliseconds());
		if (currentSegmentID < segments.size() - 1)++currentSegmentID;
		else {
			lapTimes.emplace_back(lapClock.restart().asMilliseconds());
			currentSegmentID = 0;
		}
	}
	completedSegments++;
}
