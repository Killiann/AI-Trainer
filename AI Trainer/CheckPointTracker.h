#pragma once
#include "SFML/Graphics.hpp"
#include "CheckPoint.h"
#include "CheckPointManager.h"

/// <summary>
/// Checkpoint section structure
/// </summary>
struct Segment {

	Segment(int id, CheckPoint& _c1, CheckPoint& _c2) {
		c1 = _c1;
		c2 = _c2;
		ID = id;
	}

	int ID;
	CheckPoint c1, c2;
	std::vector<float> timings;
	/// <summary>
	/// Get last timing saved for this section
	/// </summary>
	/// <returns>float, last time(ms)</returns>
	float getLast() { return timings[timings.size() - 1]; }
	/// <summary>
	/// Get fastest time on this section
	/// </summary>
	/// <returns>float, fastest time (ms)</returns>
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
	
	unsigned int currentSegmentID = 0;
	unsigned int completedSegments = 0;
	
	bool hasStarted = false;

public:
	CheckPointTracker(){}
	CheckPointTracker(CheckPointManager& checkpointManager);
	void StartTracking();
	void CompleteSegment();

	/// <summary>
	/// Get last lap time
	/// </summary>
	/// <returns>float, last lap time (ms)</returns>
	inline float GetLastLapTime() { return lapTimes.size() > 0 ? lapTimes[lapTimes.size() - 1] : 0.f; }	
	/// <summary>
	/// Get fastest time of certain segment
	/// </summary>
	/// <returns>float, fastest segment time (ms)</returns>
	inline float GetFastestOfSegment() { return segments[currentSegmentID].GetFastest(); }
	/// <summary>
	/// Get current segment time 
	/// </summary>
	/// <returns>float, current segment time (ms)</returns>
	inline float GetCurrentSegmentTime() { return !hasStarted ? 0.f : segmentClock.getElapsedTime().asMilliseconds(); }	
	/// <summary>
	/// Get amount of completed segments
	/// </summary>
	/// <returns>int, completed segments</returns>
	inline int GetCompletedSegments() { return completedSegments; }
	/// <summary>
	/// Check if tracking has started
	/// </summary>
	/// <returns>bool, has started</returns>
	inline bool Started() { return hasStarted; }
	/// <summary>
	/// Get bounds of upcoming checkpoint
	/// </summary>
	/// <returns>sf::FloatRect, bounds</returns>
	inline sf::FloatRect  GetNextCheckpointBounds() {
		return !hasStarted ? segments[0].c1.getGlobalBounds() : segments[currentSegmentID].c2.getGlobalBounds();
	}
	/// <summary>
	/// Get center of upcoming checkpoint
	/// </summary>
	/// <returns>sf::Vector2f, center point</returns>
	inline sf::Vector2f GetNextCheckpointCenter() {
		return !hasStarted ? segments[0].c1.GetCenter() : segments[currentSegmentID].c2.GetCenter();
	}
};
