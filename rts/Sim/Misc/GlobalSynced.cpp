/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#include "GlobalSynced.h"

#include <assert.h>
#include <cstring>

#include "ExternalAI/SkirmishAIHandler.h"
#include "Game/GameSetup.h"
#include "Sim/Misc/TeamHandler.h"
#include "Sim/Misc/GlobalConstants.h"
#include "System/Util.h"
#include "System/Log/FramePrefixer.h"


/**
 * @brief global synced
 *
 * Global instance of CGlobalSynced
 */
CGlobalSynced* gs = NULL;



CR_BIND(CGlobalSynced, )

CR_REG_METADATA(CGlobalSynced, (
	CR_MEMBER(frameNum),
	CR_MEMBER(tempNum),

	CR_MEMBER(speedFactor),
	CR_MEMBER(wantedSpeedFactor),

	CR_MEMBER(paused),
	CR_MEMBER(godMode),
	CR_MEMBER(globalLOS),
	CR_MEMBER(cheatEnabled),
	CR_MEMBER(noHelperAIs),
	CR_MEMBER(editDefsEnabled),
	CR_MEMBER(useLuaGaia),

	CR_MEMBER(randSeed),
	CR_MEMBER(initRandSeed)
))


/**
 * Initializes variables in CGlobalSynced
 */
CGlobalSynced::CGlobalSynced()
{
	randSeed = 18655;
	initRandSeed = randSeed;

	frameNum = 0;
	tempNum = 2;

	speedFactor = 1;
	wantedSpeedFactor = 1;
	paused = false;
	godMode = false;
	cheatEnabled = false;
	noHelperAIs = false;
	editDefsEnabled = false;

	useLuaGaia = true;

	memset(globalLOS, 0, sizeof(globalLOS));
	log_framePrefixer_setFrameNumReference(&frameNum);

	teamHandler = new CTeamHandler();
}


CGlobalSynced::~CGlobalSynced()
{
	SafeDelete(teamHandler);

	log_framePrefixer_setFrameNumReference(NULL);
}


void CGlobalSynced::LoadFromSetup(const CGameSetup* setup)
{
	noHelperAIs = setup->noHelperAIs;
	useLuaGaia  = setup->useLuaGaia;

	skirmishAIHandler.LoadFromSetup(*setup);
	teamHandler->LoadFromSetup(setup);
}

/**
 * @return synced random integer
 *
 * returns a synced random integer
 */
int CGlobalSynced::randInt()
{
	randSeed = (randSeed * 214013L + 2531011L);
	return (randSeed >> 16) & RANDINT_MAX;
}

/**
 * @return synced random float
 *
 * returns a synced random float
 */
float CGlobalSynced::randFloat()
{
	randSeed = (randSeed * 214013L + 2531011L);
	return float((randSeed >> 16) & RANDINT_MAX)/RANDINT_MAX;
}

/**
 * @return synced random vector
 *
 * returns a synced random vector
 */
float3 CGlobalSynced::randVector()
{
	float3 ret;
	do {
		ret.x = randFloat()*2-1;
		ret.y = randFloat()*2-1;
		ret.z = randFloat()*2-1;
	} while(ret.SqLength()>1);

	return ret;
}
