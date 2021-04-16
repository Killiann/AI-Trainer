#include "CheckPointTracker.h"

/// <summary>
/// Initialise CheckpointTracker 
/// </summary>
/// <param name="manager">CheckPointManager reference</param>
CheckPointTracker::CheckPointTracker(CheckPointManager& manager) {
	checkpoints = manager.GetCheckpoints();

	//setup segments
	for (unsigned int i = 0; i < checkpoints.size() - 1; ++i) {
		segments.emplace_back(i, checkpoints[i], checkpoints[i + 1]);
	}
	segments.emplace_back(checkpoints.size() - 1, checkpoints[checkpoints.size() - 1], checkpoints[0]);
}

/// <summary>
/// Start Tracking time / passed checkpoints
/// </summary>
void CheckPointTracker::StartTracking() {
	currentSegmentID = 0;
	segmentClock.restart();
	lapClock.restart();
}

/// <summary>
/// Complete track segment
/// </summary>
void CheckPointTracker::CompleteSegment() {
	if (!hasStarted) {
		hasStarted = true;
		StartTracking();
	}
	else {
		segments[currentSegmentID].timings.push_back((float)segmentClock.restart().asMilliseconds());
		if (currentSegmentID < segments.size() - 1)++currentSegmentID;
		else {
			lapTimes.push_back((float)lapClock.restart().asMilliseconds());
			currentSegmentID = 0;
		}
	}
	completedSegments++;
}
