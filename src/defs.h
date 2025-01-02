/*
Copyright (C) 2003 Parallel Realities
Copyright (C) 2011, 2012 Guus Sliepen
Copyright (C) 2012, 2015-2023 Diligent Circle <diligentcircle@riseup.net>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 3
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DEFS_H
#define DEFS_H


// Macros
#include "SDL_image.h"
#define LIMIT(x, a, b) x = ((x) < (b) ? ((x) > (a) ? (x) : (a)) : (b))
#define LIMIT_ADD(x, y, a, b) x = (((x) + (y)) < (b) ? \
	(((x) + (y)) > (a) ? \
		((x) + (y)) : (a)) : (b))
#define WRAP_ADD(x, y, a, b) x = (((x) + (y)) + \
	((x) + (y) < (a) ? ((b) - (a)) : 0) + \
	((x) + (y) > (b) ? ((a) - (b)) : 0))
#define CHANCE(x) (((double)rand() / ((double)RAND_MAX+1)) < (x))
#define RANDRANGE(x, y) (((x) < (y)) ? \
	((x) + (rand() % (long)(1 + (y) - (x)))) : (x))
#define DRAND ((double)rand() / RAND_MAX)
#define _(s) gettext(s)
#define CSDLP(x) (((x) == SDL_PRESSED) ? 1 : 0)

// A soft dependency defines these as well, so check if they are
// defined before defining (avoids compiler warnings)
#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif


// Compile-time options
#ifndef VERSION
#define VERSION "???"
#endif

#ifndef DATADIR
#define DATADIR "."
#endif

#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH 800
#endif

#ifndef SCREEN_HEIGHT
#define SCREEN_HEIGHT 600
#endif

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#define STRMAX 2000
#define STRMAX_SHORT 200

#define FULLSCREEN SDL_WINDOW_FULLSCREEN_DESKTOP

#define DEFAULT_SCREEN_WIDTH MAX(SCREEN_WIDTH, 640)
#define DEFAULT_SCREEN_HEIGHT MAX(SCREEN_HEIGHT, 480)
#define STARS_NUM 200
#define X_VIEW_BORDER 100
#define Y_VIEW_BORDER 100
#define CAMERA_MAX_SPEED 3.

#define MAX_HOMING 20
#define MAX_DOUBLE_HOMING (game.difficulty != DIFFICULTY_ORIGINAL ? 15 : 10)
#define MAX_MICRO_HOMING 10

#define RAY_INTERVAL 250
#define RAY_DAMAGE_DELAY 5

#define WARP_SPEED MAX(12, 3 * screen->w / 200)
#define ALIEN_WARP_SPEED MIN(-15, -3 * screen->w / 160)
#define ALIEN_WARP_ACCEL (game.difficulty == DIFFICULTY_ORIGINAL ? -15: -0.5)

#define BAD_TARGET_ALLOW_TIME 30

#define SLAVE_RESCUE_TARGET 250

// various sizing stuff
#define MSGBOX_HEIGHT 100

#define TTF_FONT_SIZE 20

#define PIXFONT_LINE_HEIGHT 16
#define PIXFONT_W 8
#define PIXFONT_H 14

#define MENU_Y (screen->h / 3 + 50)
#define MENU_W 500
#define MENU_SPACING 30

#define BRIEFING_WIDTH 600

#define JS_DEADZONE 0.05
#define JS_MAX (32767 - JS_DEADZONE*32767)

#define KLINE_STAGE1_SHIELD (aliens[ALIEN_KLINE].maxShield * 3 / 4)
#define KLINE_STAGE2_SHIELD (aliens[ALIEN_KLINE].maxShield / 2)
#define KLINE_STAGE3_SHIELD (aliens[ALIEN_KLINE].maxShield / 4)
#define KLINE_SHIELD_TINY (aliens[ALIEN_KLINE].maxShield / 20)
#define KLINE_SHIELD_SMALL (aliens[ALIEN_KLINE].maxShield / 4)
#define KLINE_SHIELD_MEDIUM (aliens[ALIEN_KLINE].maxShield * 3 / 8)

// Radio life/speed
#define RADIO_LIFE_INSTANT  60
#define RADIO_LIFE_FAST     120
#define RADIO_LIFE_NORMAL   240
#define RADIO_LIFE_SLOW     360
#define RADIO_LIFE_SLOTH    480
#define DEFAULT_RADIO_LIFE  RADIO_LIFE_NORMAL

// Object Flags
#define FL_WEAPCO 			(1L << 0)
#define FL_FRIEND 			(1L << 1)
#define FL_IMMORTAL 		(1L << 2)
#define FL_NOMOVE 			(1L << 3)
#define FL_NOFIRE 			(1L << 4)
#define FL_FIRERAY 			(1L << 5)
#define FL_DAMAGEOWNER 		(1L << 6)
#define FL_LEAVESECTOR 		(1L << 7)
#define FL_ESCAPED 			(1L << 8)
#define FL_DROPMINES 		(1L << 9)
#define FL_AIMS 			(1L << 10)
#define FL_DISABLED 		(1L << 11)
#define FL_RUNSAWAY 		(1L << 12)
#define FL_ALWAYSFACE 		(1L << 13) // Kline doesn't turn his back on you! ;)
#define FL_CIRCLES 			(1L << 14) // Kline can circle around
#define FL_CONTINUOUS_FIRE 	(1L << 15) // Go absolutely nutts(!)
#define FL_DEPLOYDRONES 	(1L << 16) // Deploys small drone - Used by Boss 2
#define FL_CANCLOAK 		(1L << 17)
#define FL_ISCLOAKED 		(1L << 18)
#define FL_ACTIVATE 		(1L << 19)
#define FL_HASMINIMUMSPEED 	(1L << 20)
#define FL_FIRELASER 		(1L << 21)
#define FL_NOBANTER 		(1L << 22)

// Weapon flags
#define WF_SPREAD 4
#define WF_SCATTER 8
#define WF_VARIABLE_SPEED 16
#define WF_HOMING 32
#define WF_SHOCKWAVE 64
#define WF_WEAPCO 128
#define WF_FRIEND 256
#define WF_AIMED 512
#define WF_DISABLE 1024
#define WF_TIMEDEXPLOSION 2048

#define MT_NONE -2

#define MAX_CARGO 20
#define MAX_INFOLINES 3
#define MAX_EVENTS 20
#define MAX_PLANETS 10


enum keys {
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_FIRE,
	KEY_ALTFIRE,
	KEY_SWITCH,
	KEY_PAUSE,
	KEY_ESCAPE,
	KEY_FULLSCREEN,
	KEY_DUMMY,
	KEY_LAST
};

// AI Types
enum {
	AI_NORMAL = 1,
	AI_DEFENSIVE,
	AI_OFFENSIVE,
	AI_EVASIVE,
	AI_WANDER
};

// These are for Alien *indexes* NOT classdefs!!
enum {
	ALIEN_NORMAL_LAST = 20,
	ALIEN_BOSS,
	ALIEN_BOSS_PART1,
	ALIEN_BOSS_PART2,
	ALIEN_BOSS_PART3,
	ALIEN_BOSS_PART4,
	ALIEN_BOSS_PART5,
	ALIEN_BOSS_PART6,
	ALIEN_KLINE,
	ALIEN_PHOEBE,
	ALIEN_URSULA,
	ALIEN_SID,
	ALIEN_FRIEND1,
	ALIEN_FRIEND2,
	ALIEN_MAX
};

// Droppables
enum {
	P_ANYTHING = 1,
	P_WEAPONS,
	P_CASH,
	P_ROCKET,
	P_SHIELD,
	P_CARGO,
	P_PLASMA_AMMO,
	P_PLASMA_RATE,
	P_PLASMA_SHOT,
	P_PLASMA_DAMAGE,
	P_MINE,				// mines detonate when you "pick them up!"
	P_PHOEBE,			// only used as an attachment(!)
	P_SLAVES,
	P_ESCAPEPOD,
	P_ORE,
	P_SUPER
};

// Jobs
enum {
	WT_PLASMA = 1,
	WT_ROCKET,
	WT_ENERGYRAY,
	WT_LASER,
	WT_MICROROCKET,
	WT_CHARGER,
	WT_DIRECTIONAL,
	WT_SPREAD
};

// Weapons
enum {
	W_NONE = -1,
	W_PLAYER_WEAPON,
	W_PLAYER_WEAPON2,
	W_SINGLE_SHOT,
	W_DOUBLE_SHOT,
	W_TRIPLE_SHOT,
	W_ROCKETS,
	W_DOUBLE_ROCKETS,
	W_MICRO_ROCKETS,
	W_ENERGYRAY,
	W_LASER,
	W_CHARGER,
	W_HOMING_MISSILE,
	W_DOUBLE_HOMING_MISSILES,
	W_MICRO_HOMING_MISSILES,
	W_AIMED_SHOT,
	W_SPREADSHOT,
	W_IONCANNON,
	W_DIRSHOCKMISSILE,
	W_MAX
};

// Mission types
enum {
	M_NONE,
	M_DESTROY_ALL_TARGETS,
	M_DESTROY_TARGET_TYPE,
	M_COLLECT,
	M_PROTECT_PICKUP,
	M_PROTECT_TARGET,
	M_DISABLE_TARGET,
	M_ESCAPE_TARGET
};

enum {
	OB_JUST_FAILED = -2,
	OB_FAILED,
	OB_INCOMPLETE,
	OB_COMPLETED,
	OB_JUST_COMPLETED,
	OB_CONDITION,
	OB_HIDDEN
};

// Class Defs - Some of these are just place holders
enum {
	CD_DUALFIGHTER, 	// 0
	CD_MISSILEBOAT,
	CD_PROTOFIGHTER,
	CD_FRIEND,
	CD_FRIGATE,
	CD_FRIGATE_WING1,
	CD_FRIGATE_WING2,
	CD_TRANSPORTSHIP,
	CD_CARGOSHIP,
 	CD_MINER,
	CD_KLINE, 			// 10
	CD_AIMFIGHTER,
	CD_SLAVETRANSPORT,
	CD_GOODTRANSPORT,
	CD_SID,
	CD_MINEBOSS,
	CD_BOSS2_WING1,
	CD_BOSS2_WING2,
	CD_BOSS2_WING3,
	CD_BOSS2_WING4,
	CD_DRONE,			// 20
	CD_CLOAKFIGHTER,
	CD_EVILURSULA,
	CD_KRASS,
	CD_EXEC,
	CD_ASTEROID,
	CD_ASTEROID2,
	CD_ESCORT,
	CD_MOBILE_RAY,
	CD_REBELCARRIER,
	CD_PLUTOBOSS, 		// 30
	CD_BARRIER,
	CD_NEPTUNEBOSS,
	CD_MOBILESHIELD,
	CD_PIRATE,
	CD_FIREFLY,
	CD_URANUSBOSS,
	CD_URANUSBOSSWING1,
	CD_URANUSBOSSWING2,
	CD_MAX,

	// Some special ones
	CD_ANY = 100,
	CD_BOSS,
	CD_PHOEBE,
	CD_URSULA
};

// Sprites
enum {
	// Intermission
	SP_CURSOR,
	SP_START_MISSION,
	SP_MAP,
	SP_STATUS,
	SP_SAVE,
	SP_SHOP,
	SP_COMM,
	SP_OPTIONS,
	SP_EXIT,
	SP_PLASMA_MAX_OUTPUT,
	SP_PLASMA_MAX_POWER,
	SP_PLASMA_MAX_RATE,
	SP_PLASMA_AMMO,
	SP_ROCKET_AMMO,
	SP_PLASMA_MIN_OUTPUT,
	SP_PLASMA_MIN_POWER,
	SP_PLASMA_MIN_RATE,
	SP_PLASMA_MAX_AMMO,
	SP_ROCKET_MAX_AMMO,
	SP_DOUBLE_ROCKETS,
	SP_MICRO_ROCKETS,
	SP_LASER,
	SP_HOMING_MISSILE,
	SP_CHARGER,
	SP_DOUBLE_HOMING_MISSILES,
	SP_MICRO_HOMING_MISSILES,
	SP_GOTO,
	SP_BUY,
	SP_SELL,
	SP_FIREFLY,
	SP_SUN,
	SP_PLANET_GREEN,
	SP_PLANET_BLUE,
	SP_PLANET_RED,
	SP_PLANET_ORANGE,

	// Bullets
	SP_PLASMA_GREEN,
	SP_PLASMA_RED,
	SP_DIR_PLASMA_GREEN,
	SP_DIR_PLASMA_RED,
	SP_ION,
	SP_ROCKET,
	SP_ROCKET_L,

	// Explosions
	SP_SMALL_EXPLOSION,
	SP_SMALL_EXPLOSION_2,
	SP_SMALL_EXPLOSION_3,
	SP_SMALL_EXPLOSION_L,
	SP_BIG_EXPLOSION,
	SP_BIG_EXPLOSION_2,
	SP_BIG_EXPLOSION_3,
	SP_BIG_EXPLOSION_L,
	SP_SMOKE,
	SP_SMOKE_2,
	SP_SMOKE_3,
	SP_SMOKE_L,
	SP_TINY_EXPLOSION,
	SP_TINY_EXPLOSION_2,
	SP_TINY_EXPLOSION_3,
	SP_TINY_EXPLOSION_L,
	SP_ELECTRICAL,
	SP_ELECTRICAL_2,
	SP_ELECTRICAL_3,
	SP_ELECTRICAL_L,

	// Pickups
	SP_PICKUP_MONEY,
	SP_PICKUP_PLASMA,
	SP_PICKUP_ROCKETS,
	SP_PICKUP_SHIELD,
	SP_PICKUP_PLASMA_OUTPUT,
	SP_PICKUP_PLASMA_POWER,
	SP_PICKUP_PLASMA_RATE,
	SP_SUPERCHARGE,
	SP_CARGO,
	SP_ESCAPE_POD,
	SP_ORE,
	SP_ORE_2,
	SP_ORE_L,
	SP_CHAIN_LINK,
	SP_MINE,

	// Targeting system
	SP_ARROW_NORTH,
	SP_ARROW_NORTHEAST,
	SP_ARROW_EAST,
	SP_ARROW_SOUTHEAST,
	SP_ARROW_SOUTH,
	SP_ARROW_SOUTHWEST,
	SP_ARROW_WEST,
	SP_ARROW_NORTHWEST,
	SP_ARROW_FRIEND_NORTH,
	SP_ARROW_FRIEND_NORTHEAST,
	SP_ARROW_FRIEND_EAST,
	SP_ARROW_FRIEND_SOUTHEAST,
	SP_ARROW_FRIEND_SOUTH,
	SP_ARROW_FRIEND_SOUTHWEST,
	SP_ARROW_FRIEND_WEST,
	SP_ARROW_FRIEND_NORTHWEST,
	SP_INDICATOR_TARGET,
	SP_INDICATOR_SID,
	SP_INDICATOR_PHOEBE,
	SP_INDICATOR_URSULA,
	SP_INDICATOR_KLINE,

	SP_MAX
};

// Face sprites
enum {
	FS_CHRIS,
	FS_SID,
	FS_KRASS,
	FS_KLINE,
	FS_PHOEBE,
	FS_URSULA,
	FS_CREW,
	FS_MAX
};

// Ship sprites
enum {
	SS_FIREFLY,
	SS_FIREFLY_L,
	SS_SID,
	SS_SID_L,
	SS_FRIEND,
	SS_FRIEND_L,
	SS_GOODTRANSPORT,
	SS_GOODTRANSPORT_L,
	SS_REBELCARRIER,
	SS_REBELCARRIER_L,
	SS_DUALFIGHTER,
	SS_DUALFIGHTER_L,
	SS_MISSILEBOAT,
	SS_MISSILEBOAT_L,
	SS_PROTOFIGHTER,
	SS_PROTOFIGHTER_L,
	SS_AIMFIGHTER,
	SS_AIMFIGHTER_L,
	SS_DRONE,
	SS_DRONE_L,
	SS_MINER,
	SS_MINER_L,
	SS_ESCORT,
	SS_ESCORT_L,
	SS_MOBILE_RAY,
	SS_MOBILE_RAY_L,
	SS_TRANSPORTSHIP,
	SS_TRANSPORTSHIP_L,
	SS_CARGOSHIP,
	SS_CARGOSHIP_L,
	SS_SLAVETRANSPORT,
	SS_SLAVETRANSPORT_L,
	SS_BARRIER,
	SS_MOBILESHIELD,
	SS_MOBILESHIELD_L,
	SS_ASTEROID,
	SS_ASTEROID_SMALL,
	SS_ASTEROID_SMALL_L,
	SS_CLOAKFIGHTER,
	SS_CLOAKFIGHTER_L,
	SS_EVILURSULA,
	SS_EVILURSULA_L,
	SS_KRASS,
	SS_KRASS_L,
	SS_FRIGATE,
	SS_FRIGATE_L,
	SS_FRIGATE_WING1,
	SS_FRIGATE_WING1_L,
	SS_FRIGATE_WING2,
	SS_FRIGATE_WING2_L,
	SS_MINERBOSS,
	SS_MINERBOSS_L,
	SS_MINERBOSS_WING1,
	SS_MINERBOSS_WING1_L,
	SS_MINERBOSS_WING2,
	SS_MINERBOSS_WING2_L,
	SS_MINERBOSS_WING3,
	SS_MINERBOSS_WING3_L,
	SS_MINERBOSS_WING4,
	SS_MINERBOSS_WING4_L,
	SS_EXEC,
	SS_EXEC_L,
	SS_PLUTOBOSS,
	SS_PLUTOBOSS_L,
	SS_URANUSBOSS,
	SS_URANUSBOSS_L,
	SS_URANUSBOSS_WING1,
	SS_URANUSBOSS_WING1_L,
	SS_URANUSBOSS_WING2,
	SS_URANUSBOSS_WING2_L,
	SS_KLINE,
	SS_KLINE_L,
	SS_HIT_INDEX,
	SS_MAX = SS_HIT_INDEX * 2
};

// Shop sprites
enum {
	SHOP_S_PRIMARY,
	SHOP_S_POWERUP,
	SHOP_S_SECONDARY,
	SHOP_S_CATALOG,
	SHOP_S_SHIP_INFO,
	SHOP_S_ITEM_INFO,
	SHOP_S_MAX
};

// Text sprites
enum {
	// Main menu
	TS_DILIGENTCIRCLE = MAX_INFOLINES,
	TS_PRESENTS,
	TS_AN_SDL_GAME,
	TS_ORIGINALLY_BY,
	TS_START_NEW_GAME,
	TS_LOAD_GAME,
	TS_CONTINUE_CURRENT_GAME,
	TS_OPTIONS,
	TS_CREDITS,
	TS_CHEAT_OPTIONS,
	TS_QUIT,
	TS_SOUND,
	TS_MUSIC,
	TS_FULLSCREEN,
	TS_AUTOPAUSE,
	TS_RADIO_SPEED,
	TS_BACK_TO_MAIN_MENU,
	TS_SAVESLOT_0,
	TS_SAVESLOT_1,
	TS_SAVESLOT_2,
	TS_SAVESLOT_3,
	TS_SAVESLOT_4,
	TS_SAVESLOT_5,
	TS_UNLIMITED_SHIELD,
	TS_UNLIMITED_AMMO,
	TS_UNLIMITED_CASH,
	TS_UNLIMITED_TIME,
	TS_START_GAME,
	TS_DIFFICULTY,

	// Intermission
	TS_CURRENT_SYSTEM,
	TS_INFO_START_MISSION,
	TS_INFO_GOTO,
	TS_INFO_MAP,
	TS_INFO_STATUS,
	TS_INFO_SAVE_GAME,
	TS_INFO_SHOP,
	TS_INFO_COMMS,
	TS_INFO_OPTIONS,
	TS_INFO_EXIT,
	TS_CURRENT_PLANET,
	TS_DEST_PLANET,

	// Status
	TS_STATUS_HEADER,
	TS_STATUS_DIFFICULTY,
	TS_SHOTS_FIRED,
	TS_HITS_SCORED,
	TS_ACCURACY,
	TS_OTHER_KILLS,
	TS_CASH_EARNED,
	TS_CHRIS_HEADER,
	TS_CHRIS_KILLS,
	TS_CHRIS_SHIELD_PICKUPS,
	TS_CHRIS_PLASMA_PICKUPS,
	TS_CHRIS_ROCKET_PICKUPS,
	TS_CHRIS_POWERUP_PICKUPS,
	TS_CHRIS_MINES_KILLED,
	TS_CHRIS_SLAVES_RESCUED,
	TS_PHOEBE_HEADER,
	TS_PHOEBE_KILLS,
	TS_PHOEBE_DEATHS,
	TS_URSULA_HEADER,
	TS_URSULA_KILLS,
	TS_URSULA_DEATHS,
	TS_STATUS_FOOTER,

	// Mission
	TS_RADIO,
	TS_SHIELD,
	TS_PLASMA_T,
	TS_AMMO_T,
	TS_TARGET,
	TS_TARGET_SID,
	TS_TARGET_PHOEBE,
	TS_TARGET_KLINE,
	TS_CASH_T,
	TS_OBJECTIVES_T,
	TS_TIME_T,
	TS_POWER,
	TS_OUTPUT,
	TS_COOLER,
	TS_PAUSED,
	TS_TIME,
	TS_PLASMA,
	TS_AMMO,
	TS_CASH,
	TS_OBJECTIVES,

	TS_MAX
};

// Menu types
enum {
	MENU_MAIN,
	MENU_DIFFICULTY,
	MENU_LOAD,
	MENU_OPTIONS,
	MENU_CHEAT
};

// Shop items
// Note: The error codes are set manually because every actual items
// must be >= 0; the real items are used to index an array.
enum {
	SHOP_ERROR_WEAPON_CAPACITY = -9,
	SHOP_ERROR_ALREADY_OWNED = -8,
	SHOP_ERROR_IS_NOT_ROCKETS = -7,
	SHOP_ERROR_NOTHING_TO_SELL = -6,
	SHOP_ERROR_CANNOT_SELL = -5,
	SHOP_ERROR_AMMO_LIMIT = -4,
	SHOP_ERROR_CANNOT_UPGRADE = -3,
	SHOP_ERROR_INSUFFICIENT_FUNDS = -2,
	SHOP_NOTHING = -1,
	SHOP_PLASMA_MAX_OUTPUT,
	SHOP_PLASMA_MAX_DAMAGE,
	SHOP_PLASMA_MAX_RATE,
	SHOP_PLASMA_MIN_OUTPUT,
	SHOP_PLASMA_MIN_DAMAGE,
	SHOP_PLASMA_MIN_RATE,
	SHOP_PLASMA_AMMO,
	SHOP_ROCKET_AMMO,
	SHOP_PLASMA_MAX_AMMO,
	SHOP_ROCKET_MAX_AMMO,
	SHOP_DOUBLE_ROCKETS,
	SHOP_MICRO_ROCKETS,
	SHOP_LASER,
	SHOP_HOMING_MISSILE,
	SHOP_CHARGER,
	SHOP_DOUBLE_HOMING_MISSILES,
	SHOP_MICRO_HOMING_MISSILES,
	SHOP_MAX
};

// Font Colors
enum {
	FONT_WHITE,
	FONT_RED,
	FONT_YELLOW,
	FONT_GREEN,
	FONT_CYAN,
	FONT_OUTLINE, // a dark blue color
	FONT_MAX
};

// Sounds
enum {
	SFX_EXPLOSION,
	SFX_HIT,
	SFX_DEATH,
	SFX_MISSILE,
	SFX_PLASMA,
	SFX_CLOCK,
	SFX_FLY,
	SFX_ENERGYRAY,
	SFX_PICKUP,
	SFX_SHIELDUP,
	SFX_CLOAK,
	SFX_DEBRIS,
	SFX_DEBRIS2,
	SFX_LASER,
	SFX_PLASMA2,
	SFX_PLASMA3,
	SFX_MAX
};

// Sections
enum {
	SECTION_TITLE,
	SECTION_INTERMISSION,
	SECTION_GAME
};

	
// Systems
enum {
	SYSTEM_SPIRIT,
	SYSTEM_EYANANTH,
	SYSTEM_MORDOR,
	SYSTEM_SOL,
	SYSTEM_MAX
};

// Planets (Spirit)
enum {
	PLANET_HAIL,
	PLANET_CERADSE,
	PLANET_HINSTAG,
	PLANET_JOLDAR,
	PLANET_MOEBO
};

// Planets (Eyananth)
enum {
	PLANET_NEROD,
	PLANET_ALLEZ,
	PLANET_URUSOR,
	PLANET_DORIM,
	PLANET_ELAMALE,
	PLANET_RESCUESLAVES = 9 // For save compatibility
};

// Planets (Mordor)
enum {
	PLANET_ODEON,
	PLANET_FELLON,
	PLANET_SIVEDI,
	PLANET_ALMARTHA,
	PLANET_POSWIC,
	PLANET_ELLESH,
	PLANET_CLOAKFIGHTER = 9 // For save compatibility
};

// Planets (Sol)
enum {
	PLANET_MERCURY,
	PLANET_VENUS,
	PLANET_EARTH,
	PLANET_MARS,
	PLANET_JUPITER,
	PLANET_SATURN,
	PLANET_URANUS,
	PLANET_NEPTUNE,
	PLANET_PLUTO
};

// Missions
enum {
	MISN_START,
	MISN_HAIL,
	MISN_CERADSE,
	MISN_HINSTAG,
	MISN_JOLDAR,
	MISN_MOEBO,
	MISN_RESCUESLAVES,
	MISN_NEROD,
	MISN_ALLEZ,
	MISN_URUSOR,
	MISN_DORIM,
	MISN_ELAMALE,
	MISN_CLOAKFIGHTER,
	MISN_ODEON,
	MISN_FELLON,
	MISN_SIVEDI,
	MISN_ALMARTHA,
	MISN_POSWIC,
	MISN_ELLESH,
	MISN_PLUTO,
	MISN_NEPTUNE,
	MISN_URANUS,
	MISN_SATURN,
	MISN_JUPITER,
	MISN_MARS,
	MISN_EARTH,
	MISN_VENUS,
	MISN_INTERCEPTION,
	MISN_MAX
};

// Difficulties
enum {
	DIFFICULTY_SUPEREASY = 0,
	DIFFICULTY_EASY,
	DIFFICULTY_NORMAL,
	DIFFICULTY_HARD,
	DIFFICULTY_NIGHTMARE,
	DIFFICULTY_ORIGINAL,
	DIFFICULTY_MAX
};

// Engine state values (used for engine.done)
enum {
	ENGINE_RUNNING = 0,
	ENGINE_CLOSING,
	ENGINE_SYSEXIT,
	ENGINE_FORMATION
};

extern const char * const systemBackground[];
extern const int rate2reload[6];

#endif
