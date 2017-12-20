#include "BezierPath.h"

//constructor
BezierPath::BezierPath() {

}

//destructor
BezierPath::~BezierPath() {

}

//add curve
void BezierPath::AddCurve(BezierCurve curve, int samples) {

	mCurves.push_back(curve);
	mSamples.push_back(samples);
}

//loop through curves
void BezierPath::Sample(std::vector<Vector2>* sampledPath) {

	for (int i = 0; i < mCurves.size(); i++) {

		for (float t = 0; t <= 1.0f; t += (1.0f / mSamples[i])) {

			sampledPath->push_back(mCurves[i].CalculateCurvePoint(t));
		}
	}
}