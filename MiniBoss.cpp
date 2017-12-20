#include "MiniBoss.h"
#include "BoxCollider.h"

std::vector<std::vector<Vector2>> MiniBoss::sDivePaths;

void MiniBoss::CreateDivePaths() {

	int currentPath = 0;
	BezierPath* path = new BezierPath();

	path->AddCurve({ Vector2(0.0f, 0.0f), Vector2(0.0f, -60.0f), Vector2(-90.0f, -60.0f), Vector2(-90.0f, 0.0f) }, 15);
	path->AddCurve({ Vector2(-90.0f, 0.0f), Vector2(-90.0f, 60.0f), Vector2(-100.0f, 275.0f), Vector2(-15.0f, 275.0f) }, 15);
	path->AddCurve({ Vector2(-15.0f, 275.0f), Vector2(85.0f, 275.0f), Vector2(85.0f, 125.0f), Vector2(-15.0f, 125.0f) }, 15);
	path->AddCurve({ Vector2(-15.0f, 125.0f), Vector2(-175.0f, 125.0f), Vector2(0.0f, 450.0f), Vector2(120.0f, 450.0f) }, 25);
	path->AddCurve({ Vector2(120.0f, 450.0f), Vector2(160.0f, 450.0f), Vector2(200.0f, 500.0f), Vector2(200.0f, 550.0f) }, 15);
	path->AddCurve({ Vector2(200.0f, 550.0f), Vector2(200.0f, 540.0f), Vector2(200.0f, 810.0f), Vector2(200.0f, 800.0f) }, 1);

	sDivePaths.push_back(std::vector<Vector2>());
	path->Sample(&sDivePaths[currentPath]);
	delete path;

	currentPath = 1;
	path = new BezierPath();

	path->AddCurve({ Vector2(0.0f, 0.0f), Vector2(0.0f, -60.0f), Vector2(90.0f, -60.0f), Vector2(90.0f, 0.0f) }, 15);
	path->AddCurve({ Vector2(90.0f, 0.0f), Vector2(90.0f, 60.0f), Vector2(100.0f, 275.0f), Vector2(15.0f, 275.0f) }, 15);
	path->AddCurve({ Vector2(15.0f, 275.0f), Vector2(-85.0f, 275.0f), Vector2(-85.0f, 125.0f), Vector2(15.0f, 125.0f) }, 15);
	path->AddCurve({ Vector2(15.0f, 125.0f), Vector2(175.0f, 125.0f), Vector2(0.0f, 450.0f), Vector2(-120.0f, 450.0f) }, 25);
	path->AddCurve({ Vector2(-120.0f, 450.0f), Vector2(-160.0f, 450.0f), Vector2(-200.0f, 500.0f), Vector2(-200.0f, 550.0f) }, 15);
	path->AddCurve({ Vector2(-200.0f, 550.0f), Vector2(-200.0f, 540.0f), Vector2(-200.0f, 810.0f), Vector2(-200.0f, 800.0f) }, 1);

	sDivePaths.push_back(std::vector<Vector2>());
	path->Sample(&sDivePaths[currentPath]);
	delete path;

	currentPath = 2;
	path = new BezierPath();

	path->AddCurve({ Vector2(0.0f, 0.0f), Vector2(0.0f, -60.0f), Vector2(-90.0f, -60.0f), Vector2(-90.0f, 0.0f) }, 15);
	path->AddCurve({ Vector2(-90.0f, 0.0f), Vector2(-90.0f, 60.0f), Vector2(100.0f, 340.0f), Vector2(100.0f, 400.0f) }, 15);

	sDivePaths.push_back(std::vector<Vector2>());
	path->Sample(&sDivePaths[currentPath]);
	delete path;

	currentPath = 3;
	path = new BezierPath();

	path->AddCurve({ Vector2(0.0f, 0.0f), Vector2(0.0f, -60.0f), Vector2(90.0f, -60.0f), Vector2(90.0f, 0.0f) }, 15);
	path->AddCurve({ Vector2(90.0f, 0.0f), Vector2(90.0f, 60.0f), Vector2(-100.0f, 340.0f), Vector2(-100.0f, 400.0f) }, 15);

	sDivePaths.push_back(std::vector<Vector2>());
	path->Sample(&sDivePaths[currentPath]);
	delete path;
}

//constructor
MiniBoss::MiniBoss(int index, int path, bool challengeStage)
	: Enemy(index, path, challengeStage)
{
	mAudio = AudioManager::Instance();

	mTexture = new Texture("ships/Defender.png");
	mTexture->Parent(this);
	mTexture->Pos(VEC2_ZERO);

	mDeathAnimation = new AnimatedTexture("ships/Explosion.png", 0, 0, 64, 64, 6, 1.0f, AnimatedTexture::horizontal);
	mDeathAnimation->Parent(this);
	mDeathAnimation->Pos(VEC2_ZERO);
	mDeathAnimation->WrapMode(AnimatedTexture::once);

	mType = miniboss;

	mCaptureDive = false;
	mCurrentPath = 1;
	mCapturing = false;

	mCaptureBeam = new CaptureBeam();
	mCaptureBeam->Parent(this);
	mCaptureBeam->Pos(Vector2(0.0f, -100.0f));
	mCaptureBeam->Rotation(180.0f);

	AddCollider(new BoxCollider(mTexture->ScaledDimensions()));
}

//destructor
MiniBoss::~MiniBoss() {

	mAudio = NULL;

	delete mCaptureBeam;
	mCaptureBeam = NULL;

	delete mDeathAnimation;
	mDeathAnimation = NULL;
}

void MiniBoss::Hit(PhysEntity * other) {

	mDeathAnimation->ResetAnimation();
	mAudio->PlaySFX("audio/Explosion.wav");
	mCurrentState = dead;
}

Vector2 MiniBoss::LocalFormationPosition() {

	Vector2 retVal;

	retVal.x = (sFormation->GridSize().x + sFormation->GridSize().x * 2 * (mIndex / 2)) * pow(-1, mIndex % 2 + 1);
	retVal.y = -sFormation->GridSize().y;

	return retVal;
}

void MiniBoss::HandleCaptureBeam() {

	mCaptureBeam->Update();

	if (!mCaptureBeam->IsAnimating()) {

		Translate(VEC2_UP * mSpeed * mTimer->DeltaTime(), world);
		if (Pos().y >= 910.0f) {

			Pos(Vector2(WorldFormationPosition().x, -20.0f));
			mCapturing = false;
		}
	}
}

void MiniBoss::HandleDiveState() {

	if (mCurrentWaypoint < sDivePaths[mCurrentPath].size()) {

		Vector2 waypointPos = mDiveStartPosition + sDivePaths[mCurrentPath][mCurrentWaypoint];

		Vector2 dist = waypointPos - Pos();
		Translate(dist.Normalized() * mTimer->DeltaTime() * mSpeed, world);

		Rotation(atan2(dist.y, dist.x) * RAD_TO_DEG + 90.0f);

		if ((waypointPos - Pos()).MagnitudeSqr() < EPSILON) {
			mCurrentWaypoint++;
		}

		if (mCurrentWaypoint == sDivePaths[mCurrentPath].size()) {
			if (mCaptureDive) {

				mCapturing = true;
				Rotation(180);
			}
			else {

				Pos(Vector2(WorldFormationPosition().x, -20.0f));
			}
		}
	}
	else {

		if (!mCaptureDive || !mCapturing) {

			Vector2 dist = WorldFormationPosition() - Pos();
			Translate(dist.Normalized() * mTimer->DeltaTime() * mSpeed, world);

			Rotation(atan2(dist.y, dist.x) * RAD_TO_DEG + 90.0f);

			if (dist.MagnitudeSqr() < EPSILON) {

				JoinFormation();
			}
		}
		else {

			HandleCaptureBeam();
		}
	}
}

void MiniBoss::HandleDeadState() {

	this->Active(false);
}

void MiniBoss::RenderDiveState() {

	mTexture->Render();

	if (mCapturing && mCaptureBeam->IsAnimating()) {
		mCaptureBeam->Render();
	}
}

void MiniBoss::RenderDeadState() {

	mDeathAnimation->Render();
}

void MiniBoss::Dive(int type) {

	mCaptureDive = type != 0;

	Enemy::Dive();

	if (mCaptureDive) {

		mCapturing = false;
		mCurrentPath = 2 + rand() % 2;
		mCaptureBeam->ResetAnimation();
	}
	else {

		mCurrentPath = mIndex % 2;
	}
}