#include "Bullet.h"
#include "BoxCollider.h"
#include "PhysicsManager.h"

//constructor
Bullet::Bullet(bool friendly) {

	mTimer = Timer::Instance();

	mSpeed = 1000.0f;

	mTexture = new Texture("ships/RedFire.png");
	mTexture->Parent(this);
	mTexture->Pos(VEC2_ZERO);

	Reload();

	AddCollider(new BoxCollider(mTexture->ScaledDimensions()));

	if (friendly) {

		PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::FriendlyProjectiles);
	}
	else {

		PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::HostileProjectiles);
	}
}

//destructor
Bullet::~Bullet() {

	mTimer = NULL;

	delete mTexture;
	mTexture = NULL;
}

//fire bullet in a direction pos
void Bullet::Fire(Vector2 pos) {

	Pos(pos);
	Active(true);
}

//get ready to re use bullet
void Bullet::Reload() {

	Active(false);
}

void Bullet::Hit(PhysEntity * other) {

	Reload();
}

//update
void Bullet::Update() {

	if (Active()) {

		Translate(-VEC2_UP*mSpeed*mTimer->DeltaTime(), local);

		Vector2 pos = Pos();

		if (pos.y < -OFFSCREEN_BUFFER || pos.y > Graphics::Instance()->SCREEN_HEIGHT + OFFSCREEN_BUFFER) {

			Reload();
		}
	}
}

//render
void Bullet::Render() {

	if (Active()) {

		mTexture->Render();
		//render collider
		//PhysEntity::Render();
	}
}

bool Bullet::IgnoreCollisions() {

	return !Active();
}
