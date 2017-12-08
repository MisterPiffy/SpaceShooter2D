#ifndef __COMMON_H__
#define __COMMON_H__

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "math.h"

#include <cstdlib>

#include <SDL.h>

// #include "Sprite.h"

//upper limits
#define MAX_BULLETS 1000
#define MAX_ENEMIES 50
#define MAX_EXPLOSIONS 50

//player fire
enum {
	PLAYER_BEAM_NORMAL = 0,
	PLAYER_BEAM_CHARGE
};

//flight types
enum {
	FLIGHT_NORMAL = 0,
	FLIGHT_SINE,
	FLIGH_HOMING
};

//gamestates
enum {
	GS_TITLESCREEN = 0,
	GS_MAIN,
	GS_PAUSE,
	GS_DONE
};

/* Sprite Resources */
enum {
	SR_SHIP_1 = 0,
	SR_BEAM_1,
	SR_BEAM_2,
	SR_BACKGROUND_1,
	SR_BACKGROUND_2,
	SR_BACKGROUND_3,
	SR_ENEMY_1,
	SR_EXPLOSION_1
};


#endif