#pragma once

#define PM_MAX							7
#define PM_INCREASE						0.008f
#define PM_DECREASE						0.0016f

#define MARIO_WALK_SPEED				0.27f
#define MARIO_RUN_SPEED					0.48f
#define MARIO_FLY_SPEED_X				0.27f
#define MARIO_FLOAT_SPEED_X				0.20f

#define MARIO_RUN_ACCELERATION			0.0005613f
#define MARIO_WALK_ACCELERATION			0.000476f
#define MARIO_SKID_ACCELERATION			0.001104f

#define MARIO_HEIGHT_DIE_BOUNCE			90			

#define MARIO_JUMP_FALL_POINT			80
#define MARIO_BEGIN_HIGH_JUMP_HEIGHT	97
#define MARIO_HIGH_JUMP_FALL_POINT		216
#define MARIO_FLY_FALL_POINT			300
#define MARIO_FLY_MAX_POINT				1500


#define MARIO_WALK_FRICTION				0.0014306f
#define MARIO_RUN_FRICTION				0.0016366f
#define MARIO_CROUCH_FRICTION 			0.0008766f

#define MARIO_GRAVITY				0.002f

#define MARIO_JUMP_PUSH				0.432f
#define MARIO_FLY_PUSH				0.632f
#define MARIO_JUMP_DEFLECT_SPEED	0.3f
#define MARIO_ULTRA_JUMP_DEFLECT	1.8f
#define MARIO_DIE_DEFLECT_SPEED		0.5f

#define MARIO_STATE_IDLE			10
#define MARIO_STATE_WALK			100
#define MARIO_STATE_RUN				200
#define MARIO_STATE_RUN_HIGH_SPEED	300

#define MARIO_STATE_DIE				400
#define MARIO_STATE_CROUCH			500

#define MARIO_STATE_JUMP			600
#define MARIO_STATE_JUMP_IDLE		700
#define MARIO_STATE_JUMP_HIGH		800
#define MARIO_STATE_JUMP_FALL		900
#define MARIO_STATE_GROW			3000
#define MARIO_STATE_SHRINK			3001

#define MARIO_STATE_FLY				2000
#define MARIO_STATE_FLOAT			2001

#define MARIO_STATE_ATTACK			1001
#define MARIO_STATE_REALLY_DIE		1002

#define	MARIO_LEVEL_SMALL			1
#define	MARIO_LEVEL_BIG				2

#define MARIO_WIDTH					45
#define MARIO_HEIGHT				81

#define MARIO_CROUCH_SUBSTRACT		26


#define MARIO_SMALL_BBOX_WIDTH		13
#define MARIO_SMALL_BBOX_HEIGHT		15

#define MARIO_UNTOUCHABLE_TIME		1500
#define MARIO_ATTACK_TIME			350
#define MARIO_ATTACK_TIME_FIRE		200
#define MARIO_FIRE_TIME				100

#define MARIO_FLY_TIME				500
#define MARIO_FLOAT_TIME			500

#define FIREBALL_SPEED              0.48f
#define VFIREBALL_SPEED              0.18f
#define FIREBALL_BOUNCE_PUSH        0.4f

#define FIREBALL_SIZE               24
#define TAIL_SIZE					15

#define FIREBALL_START_Y            35

#define COIN_PRIZE			11
#define GMUSH_PRIZE			12
#define RMUSH_PRIZE			13
#define LEAF_PRIZE			14
#define PSWITCH_PRIZE		15
#define SECRET_MUSIC_PRIZE	16