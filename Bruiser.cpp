#include "Bruiser.h"
#include "BoxCollider.h"

std::vector<std::vector<Vector2>> Bruiser::sDivePaths;

void Bruiser::CreateDivePaths() {

	int currentPath = 0;
	BezierPath* path = new BezierPath();

	path->AddCurve({ Vector2(0.0f, 0.0f), Vector2(0.0f, -45.0f), Vector2(-60.0f, -45.0f), Vector2(-60.0f, 0.0f) }, 15);
	path->AddCurve({ Vector2(-60.0f, 0.0f), Vector2(-60.0f, 80.0f), Vector2(0.0f, 150.0f), Vector2(100.0f, 150.0f) }, 15);
	path->AddCurve({ Vector2(100.0f, 150.0f), Vector2(250.0f, 150.0f), Vector2(350.0f, 200.0f), Vector2(350.0f, 350.0f) }, 15);
	path->AddCurve({ Vector2(350.0f, 350.0f), Vector2(350.0f, 575.0f), Vector2(100.0f, 575.0f), Vector2(100.0f, 350.0f) }, 15);

	sDivePaths.push_back(std::vector<Vector2>());
	path->Sample(&sDivePaths[currentPath]);
	delete path;

	currentPath = 1;
	path = new BezierPath();

	path->AddCurve({ Vector2(0.0f, 0.0f), Vector2(0.0f, -45.0f), Vector2(60.0f, -45.0f), Vector2(60.0f, 0.0f) }, 15);
	path->AddCurve({ Vector2(60.0f, 0.0f), Vector2(60.0f, 80.0f), Vector2(0.0f, 150.0f), Vector2(-100.0f, 150.0f) }, 15);
	path->AddCurve({ Vector2(-100.0f, 150.0f), Vector2(-250.0f, 150.0f), Vector2(-350.0f, 200.0f), Vector2(-350.0f, 350.0f) }, 15);
	path->AddCurve({ Vector2(-350.0f, 350.0f), Vector2(-350.0f, 575.0f), Vector2(-100.0f, 575.0f), Vector2(-100.0f, 350.0f) }, 15);

	sDivePaths.push_back(std::vector<Vector2>());
	path->Sample(&sDivePaths[currentPath]);

	delete path;
}

//constructor
Bruiser::Bruiser(int index, int path, bool challengeStage, bool diver)
	: Enemy(index, path, challengeStage)
{
	mAudio = AudioManager::Instance();

	mDiver = diver;

	mTexture = new Texture("ships/Bruiser.png");
	mTexture->Parent(this);
	mTexture->Pos(VEC2_ZERO);

	mDeathAnimation = new AnimatedTexture("ships/Explosion.png", 0, 0, 64, 64, 6, 1.0f, AnimatedTexture::horizontal);
	mDeathAnimation->Parent(this);
	mDeathAnimation->Pos(VEC2_ZERO);
	mDeathAnimation->WrapMode(AnimatedTexture::once);

	mType = bruiser;

	AddCollider(new BoxCollider(mTexture->ScaledDimensions()));
}

Bruiser::~Bruiser() {

	mAudio = NULL;

	delete mDeathAnimation;
	mDeathAnimation = NULL;
}

void Bruiser::Hit(PhysEntity* other) {

	mDeathAnimation->ResetAnimation();
	mAudio->PlaySFX("audio/Explosion.wav");
	mCurrentState = dead;
}

void Bruiser::PathComplete() {

		Enemy::PathComplete();
}

void Bruiser::FlyInComplete() {

	if (mDiver) {

		mCurrentState = dead;
	}
	else {

		JoinFormation();
	}
}

Vector2 Bruiser::LocalFormationPosition() {

	Vector2 retVal;

	retVal.x = (sFormation->GridSize().x + sFormation->GridSize().x * 2 * (mIndex / 4)) * pow(-1, mIndex % 2 + 1);
	retVal.y = sFormation->GridSize().y * 2 + sFormation->GridSize().y * ((mIndex % 4) / 2);

	return retVal;
}

void Bruiser::HandleDiveState() {

	int currentPath = mIndex % 2;

	if (mCurrentWaypoint < sDivePaths[currentPath].size()) {

		Vector2 waypointPos = mDiveStartPosition + sDivePaths[currentPath][mCurrentWaypoint];

		Vector2 dist = waypointPos - Pos();
		Translate(dist.Normalized() * mTimer->DeltaTime() * mSpeed, world);

		Rotation(atan2(dist.y, dist.x) * RAD_TO_DEG + 90.0f);

		if ((waypointPos - Pos()).MagnitudeSqr() < EPSILON) {
			mCurrentWaypoint++;
		}
	}
	else {

		Vector2 dist = WorldFormationPosition() - Pos();
		Translate(dist.Normalized() * mTimer->DeltaTime() * mSpeed, world);
		Rotation(atan2(dist.y, dist.x) * RAD_TO_DEG + 90.0f);

		if (dist.MagnitudeSqr() < EPSILON) {
			JoinFormation();
		}

	}
}

void Bruiser::HandleDeadState() {

	this->Active(false);
}

void Bruiser::RenderDiveState() {

	mTexture->Render();

	int currentPath = mIndex % 2;

	Vector2 finalPos = WorldFormationPosition();
	Vector2 pathEndPos = mDiveStartPosition + sDivePaths[currentPath][sDivePaths[currentPath].size() - 1];
}

void Bruiser::RenderDeadState() {

	mDeathAnimation->Render();
}