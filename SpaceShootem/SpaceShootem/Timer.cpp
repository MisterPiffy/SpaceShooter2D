#include "Timer.h"

Timer* Timer::sInstance = NULL;

//create timer instance
Timer* Timer::Instance() {

	if (sInstance == NULL)
		sInstance = new Timer();

	return sInstance;
}

//free resources
void Timer::Release() {
	delete sInstance;
	sInstance = NULL;
}

//constructor
Timer::Timer() {

	Reset();
	mTimeScale = 1.0f;
}

//destructor
Timer::~Timer() {

}

//reset timer
void Timer::Reset() {

	mStartTicks = SDL_GetTicks();
	mElapsedTicks = 0;
	mDeltaTime = 0.0f;
}

//get deltatime
float Timer::DeltaTime() {
	
	return mDeltaTime;
}

//set timescale
void Timer::TimeScale(float t) {

	mTimeScale = t;
}

//get timescale
float Timer::TimeScale() {
	
	return mTimeScale;
}

//update timer
void Timer::Update() {

	mElapsedTicks = SDL_GetTicks() - mStartTicks;
	mDeltaTime = mElapsedTicks * 0.001f;
}