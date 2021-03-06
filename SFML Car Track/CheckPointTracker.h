#pragma once
#include "SFML/Graphics.hpp"
#include "CheckPoint.h"
#include "CheckPointManager.h"

struct Segment {
	Segment(int id, CheckPoint* _c1, CheckPoint* _c2) {
		c1 = _c1;
		c2 = _c2;
		ID = id;
	}
	int ID;
	CheckPoint* c1, * c2;
	std::vector<float> timings;
	float getLast() { return timings[timings.size() - 1]; }
	float GetFastest() {
		return (std::min_element(timings.begin(), timings.end()) != timings.end()) ?
			*std::min_element(timings.begin(), timings.end()) : 0;
	}
};

class CheckPointTracker {

	std::vector<CheckPoint> checkpoints;
	std::vector<Segment> segments;
	std::vector<float> lapTimes;
	
	sf::Clock lapClock;
	sf::Clock segmentClock;
	int currentSegmentID = 0;
	
	bool hasStarted = false;

public:
	CheckPointTracker(){}
	CheckPointTracker(CheckPointManager* checkpointManager);
	void StartTracking();
	void CompleteSegment();
	inline float getLastLapTime() { return lapTimes.size() > 0 ? lapTimes[lapTimes.size() - 1] : 0.0; }
	inline float getFastestTime() { return segments[currentSegmentID].GetFastest(); }
	inline sf::FloatRect  GetNextCheckpointBounds() {
		return !hasStarted ? segments[0].c1->getGlobalBounds() : segments[currentSegmentID].c2->getGlobalBounds();
	}
	inline float getCurrentSegmentTime() { return !hasStarted ? 0.0 : segmentClock.getElapsedTime().asMilliseconds(); }
};
