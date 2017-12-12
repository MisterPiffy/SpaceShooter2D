#include "InputManager.h"

//set to null by default
InputManager* InputManager::sInstance = NULL;

//create new instance
InputManager* InputManager::Instance() {

	if (sInstance == NULL) {
		sInstance = new InputManager();
	}

	return sInstance;
}

//free
void InputManager::Release() {

	delete sInstance;
	sInstance = NULL;
}

//constructor
InputManager::InputManager() {

	mKeyboardState = SDL_GetKeyboardState(&mKeyLength);
	mPrevKeyboardState = new Uint8[mKeyLength];
	memcpy(mPrevKeyboardState, mKeyboardState, mKeyLength);
}

//destructor
InputManager::~InputManager() {

	delete[] mPrevKeyboardState;
	mPrevKeyboardState = NULL;
}

//key down
bool InputManager::KeyDown(SDL_Scancode scanCode) {

	return mKeyboardState[scanCode];
}

//key pressed
bool InputManager::KeyPressed(SDL_Scancode scanCode) {

	return !mPrevKeyboardState[scanCode] && mKeyboardState[scanCode];
}

//key released
bool InputManager::KeyReleased(SDL_Scancode scanCode) {

	return mPrevKeyboardState[scanCode] && !mKeyboardState[scanCode];
}

//mouse position
Vector2 InputManager::MousePos() {

	return Vector2((float)mMouseXPos, (float)mMouseYPos);
}

//mouse down
bool InputManager::MouseButtonDown(MOUSE_BUTTON button) {

	Uint32 mask = 0;

	switch (button) {

	case left:

		mask = SDL_BUTTON_LMASK;
		break;

	case right:

		mask = SDL_BUTTON_RMASK;
		break;

	case middle:

		mask = SDL_BUTTON_MMASK;
		break;

	case back:

		mask = SDL_BUTTON_X1MASK;
		break;

	case forward:

		mask = SDL_BUTTON_X2MASK;
		break;
	}

	return (mMouseState & mask);
}

//mouse pressed
bool InputManager::MouseButtonPressed(MOUSE_BUTTON button) {

	Uint32 mask = 0;

	switch (button) {

	case left:

		mask = SDL_BUTTON_LMASK;
		break;

	case right:

		mask = SDL_BUTTON_RMASK;
		break;

	case middle:

		mask = SDL_BUTTON_MMASK;
		break;

	case back:

		mask = SDL_BUTTON_X1MASK;
		break;

	case forward:

		mask = SDL_BUTTON_X2MASK;
		break;
	}

	return !(mPreviousMouseState & mask) && (mMouseState & mask);
}

//mouse released
bool InputManager::MouseButtonReleased(MOUSE_BUTTON button) {

	Uint32 mask = 0;

	switch (button) {

	case left:

		mask = SDL_BUTTON_LMASK;
		break;

	case right:

		mask = SDL_BUTTON_RMASK;
		break;

	case middle:

		mask = SDL_BUTTON_MMASK;
		break;

	case back:

		mask = SDL_BUTTON_X1MASK;
		break;

	case forward:

		mask = SDL_BUTTON_X2MASK;
		break;
	}

	return (mPreviousMouseState & mask) && !(mMouseState & mask);
}

//update mouse
void InputManager::Update() {

	mMouseState = SDL_GetMouseState(&mMouseXPos, &mMouseYPos);
}

//update input
void InputManager::UpdatePrevInput() {

	memcpy(mPrevKeyboardState, mKeyboardState, mKeyLength);
	mPreviousMouseState = mMouseState;
}