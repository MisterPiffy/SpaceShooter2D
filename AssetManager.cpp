#include "AssetManager.h"

AssetManager* AssetManager::sInstance = NULL;

//create new manager
AssetManager* AssetManager::Instance() {

	if (sInstance == NULL) {
		sInstance = new AssetManager();
	}

	return sInstance;
}

//free resources
void AssetManager::Release() {

	delete sInstance;
	sInstance = NULL;
}

//constructor
AssetManager::AssetManager() {

}

//destructor
AssetManager::~AssetManager() {

	for (auto tex : mTextures) {

		if (tex.second != NULL) {

			SDL_DestroyTexture(tex.second);
		}
	}

	mTextures.clear();

	for (auto text : mText) {

		if (text.second != NULL) {

			SDL_DestroyTexture(text.second);
		}
	}

	mText.clear();

	for (auto font : mFonts) {

		if (font.second != NULL) {

			TTF_CloseFont(font.second);
		}
	}

	mFonts.clear();

	for (auto music : mMusic) {

		if (music.second != NULL) {

			Mix_FreeMusic(music.second);
		}
	}

	mMusic.clear();

	for (auto sfx : mSFX) {

		if (sfx.second != NULL) {

			Mix_FreeChunk(sfx.second);
		}
	}
	
	mSFX.clear();
}

//get texture
SDL_Texture* AssetManager::GetTexture(std::string filename) {

	std::string fullPath = SDL_GetBasePath();
	fullPath.append("Assets/" + filename);

	if (mTextures[fullPath] == nullptr) {
		mTextures[fullPath] = Graphics::Instance()->LoadTexture(fullPath);
	}

	return mTextures[fullPath];
}

//get font
TTF_Font* AssetManager::GetFont(std::string fileName, int size) {

	std::string fullPath = SDL_GetBasePath();
	fullPath.append("Assets/" + fileName);
	std::string key = fullPath + (char)size;

	if (mFonts[key] == nullptr) {

		mFonts[key] = TTF_OpenFont(fullPath.c_str(), size);
		if (mFonts[key] == nullptr) {

			printf("Font Loading Error: Font-%s Error-%s", fileName.c_str(), TTF_GetError());
		}
	}

	return mFonts[key];
}

//get text
SDL_Texture* AssetManager::GetText(std::string text, std::string fileName, int size, SDL_Color color) {

	TTF_Font* font = GetFont(fileName, size);

	std::string key = text + fileName + (char)size + (char)color.r + (char)color.g + (char)color.b;

	if (mText[key] == nullptr) {
		mText[key] = Graphics::Instance()->CreateTextTexture(font, text, color);
	}

	return mText[key];
}

//get music
Mix_Music* AssetManager::GetMusic(std::string fileName) {

	std::string fullPath = SDL_GetBasePath();
	fullPath.append("Assets/" + fileName);

	if (mMusic[fullPath] == nullptr) {

		mMusic[fullPath] = Mix_LoadMUS(fullPath.c_str());
		if (mMusic[fullPath] == NULL) {

			printf("Music Loading Error: File-%s Error-%s", fileName.c_str(), Mix_GetError());
		}
	}

	return mMusic[fullPath];
}

//get sound fx
Mix_Chunk* AssetManager::GetSFX(std::string fileName) {

	std::string fullPath = SDL_GetBasePath();
	fullPath.append("Assets/" + fileName);

	if (mSFX[fullPath] == nullptr) {

		mSFX[fullPath] = Mix_LoadWAV(fullPath.c_str());
		if (mSFX[fullPath] == NULL) {

			printf("SFX Loading Error: File-%s Error-%s", fileName.c_str(), Mix_GetError());
		}
	}

	return mSFX[fullPath];
}