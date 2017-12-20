#ifndef _BEZIERPATH_H
#define _BEZIERPATH_H
#include "MathHelper.h"
#include <vector>

class BezierPath {

private:

	std::vector<BezierCurve> mCurves;
	std::vector<int> mSamples;

public:

	BezierPath();
	~BezierPath();

	void AddCurve(BezierCurve curve, int samples);

	void Sample(std::vector<Vector2>* sampledPath);

};

#endif // !_BEZIERPATH_H