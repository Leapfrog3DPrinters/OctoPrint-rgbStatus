#pragma once

#ifndef RGBLIGHTHANDLER_H
#define RGBLIGHTHANDLER_H 1

#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <algorithm>

#include "PwmDriver.h"
#include "RgbLightPattern.h"
#include "RgbLightConstant.h"

using namespace std;

class RgbLightHandler
{
private:
	RgbLightPattern * patternLeft;
	RgbLightPattern * patternRight;

	PwmDriver * pwmDriver;

	thread workerThread;

	atomic<bool> isRunning;
	atomic<bool> patternsChanged;

	bool transitionsEnabled = true;
	unsigned int transitionRefreshInterval = 20; //ms
	unsigned int transitionTime = 200; //ms

	mutex patternMutex;

	void worker();
	void transitionBoth(const float leftFrom[NUM_COLORS], const float rightFrom[NUM_COLORS]);
public:
	RgbLightHandler(const float defaultColor[NUM_COLORS] );
	RgbLightHandler(const float defaultColor[NUM_COLORS] , unsigned int transitionRefreshInterval, unsigned int transitionTime);
	~RgbLightHandler();

	void start();
	void stop();
	void setPatternLeft(RgbLightPattern * pattern);
	void setPatternRight(RgbLightPattern * pattern);
	void setPatterns(RgbLightPattern * pattern);
};

#endif
