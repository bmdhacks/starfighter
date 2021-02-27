/*
Copyright (C) 2003 Parallel Realities
Copyright (C) 2011, 2012 Guus Sliepen
Copyright (C) 2012, 2015-2020 The Diligent Circle <diligentcircle@riseup.net>

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

#include <libintl.h>
#include <math.h>
#include <stdlib.h>

#include "defs.h"
#include "structs.h"

#include "alien.h"
#include "audio.h"
#include "bullet.h"
#include "cargo.h"
#include "collectable.h"
#include "engine.h"
#include "game.h"
#include "gfx.h"
#include "info.h"
#include "mission.h"
#include "player.h"
#include "radio.h"
#include "screen.h"
#include "ship.h"
#include "weapons.h"

Object alien_defs[CD_MAX];
Object aliens[ALIEN_MAX];


/*
This simply pulls back an alien from the array that is
"dead" (no shield) and returns the index number so we can have
a new one.
*/
static int alien_getFreeIndex()
{
	for (int i = 0 ; i < engine.maxAliens ; i++)
	{
		if (!aliens[i].active)
		{
			return i;
		}
	}

	return -1;
}

void alien_nerf(int index)
{
	if (game.difficulty == DIFFICULTY_SUPEREASY)
	{
		if ((aliens[index].classDef == CD_SID)
			|| (aliens[index].classDef == CD_PHOEBE)
			|| (aliens[index].classDef == CD_URSULA)
			|| (aliens[index].classDef == CD_GOODTRANSPORT)
			|| (aliens[index].classDef == CD_REBELCARRIER)
			|| ((game.area == MISN_URUSOR)
				&& (aliens[index].classDef == CD_CARGOSHIP)))
		{
			aliens[index].shield *= 4;
			aliens[index].maxShield *= 4;
		}
		else if ((aliens[index].classDef != CD_ASTEROID)
			&& (aliens[index].classDef != CD_ASTEROID2))
		{
			aliens[index].shield /= 2;
			aliens[index].maxShield /= 2;
		}
	}
}

void alien_defs_init()
{
	// Dual Plasma Fighter.
	alien_defs[CD_DUALFIGHTER].classDef = CD_DUALFIGHTER;
	alien_defs[CD_DUALFIGHTER].AIType = AI_NORMAL;
	alien_defs[CD_DUALFIGHTER].AITypeOriginal = AI_NORMAL;
	alien_defs[CD_DUALFIGHTER].speed = 4;
	alien_defs[CD_DUALFIGHTER].maxShield = 5;
	alien_defs[CD_DUALFIGHTER].shield = 5;
	alien_defs[CD_DUALFIGHTER].imageIndex[0] = SS_DUALFIGHTER;
	alien_defs[CD_DUALFIGHTER].imageIndex[1] = SS_DUALFIGHTER_L;
	alien_defs[CD_DUALFIGHTER].weaponType[0] = W_DOUBLE_SHOT;
	alien_defs[CD_DUALFIGHTER].weaponType[1] = W_ROCKETS;
	alien_defs[CD_DUALFIGHTER].chance[0] = 100;
	alien_defs[CD_DUALFIGHTER].chance[1] = 1;
	alien_defs[CD_DUALFIGHTER].collectChance = 50;
	alien_defs[CD_DUALFIGHTER].collectType = P_ANYTHING;
	alien_defs[CD_DUALFIGHTER].collectValue = 50;
	alien_defs[CD_DUALFIGHTER].collectTypeOriginal = P_ANYTHING;
	alien_defs[CD_DUALFIGHTER].collectValueOriginal = 50;
	alien_defs[CD_DUALFIGHTER].score = 25;
	alien_defs[CD_DUALFIGHTER].flags = FL_WEAPCO;

	// Missile Boat
	alien_defs[CD_MISSILEBOAT].classDef = CD_MISSILEBOAT;
	alien_defs[CD_MISSILEBOAT].AIType = AI_DEFENSIVE;
	alien_defs[CD_MISSILEBOAT].AITypeOriginal = AI_DEFENSIVE;
	alien_defs[CD_MISSILEBOAT].speed = 2;
	alien_defs[CD_MISSILEBOAT].maxShield = 50;
	alien_defs[CD_MISSILEBOAT].shield = 50;
	alien_defs[CD_MISSILEBOAT].imageIndex[0] = SS_MISSILEBOAT;
	alien_defs[CD_MISSILEBOAT].imageIndex[1] = SS_MISSILEBOAT_L;
	alien_defs[CD_MISSILEBOAT].weaponType[0] = W_ROCKETS;
	alien_defs[CD_MISSILEBOAT].weaponType[1] = W_DOUBLE_ROCKETS;
	alien_defs[CD_MISSILEBOAT].chance[0] = 25;
	alien_defs[CD_MISSILEBOAT].chance[1] = 4;
	alien_defs[CD_MISSILEBOAT].collectChance = 25;
	alien_defs[CD_MISSILEBOAT].collectType = P_ANYTHING;
	alien_defs[CD_MISSILEBOAT].collectValue = 75;
	alien_defs[CD_MISSILEBOAT].collectTypeOriginal = P_ANYTHING;
	alien_defs[CD_MISSILEBOAT].collectValueOriginal = 75;
	alien_defs[CD_MISSILEBOAT].score = 250;
	alien_defs[CD_MISSILEBOAT].flags = FL_WEAPCO;

	//Prototype fighter
	alien_defs[CD_PROTOFIGHTER].classDef = CD_PROTOFIGHTER;
	alien_defs[CD_PROTOFIGHTER].AIType = AI_DEFENSIVE;
	alien_defs[CD_PROTOFIGHTER].AITypeOriginal = AI_DEFENSIVE;
	alien_defs[CD_PROTOFIGHTER].speed = 5;
	alien_defs[CD_PROTOFIGHTER].maxShield = 15;
	alien_defs[CD_PROTOFIGHTER].shield = 15;
	alien_defs[CD_PROTOFIGHTER].imageIndex[0] = SS_PROTOFIGHTER;
	alien_defs[CD_PROTOFIGHTER].imageIndex[1] = SS_PROTOFIGHTER_L;
	alien_defs[CD_PROTOFIGHTER].weaponType[0] = W_TRIPLE_SHOT;
	alien_defs[CD_PROTOFIGHTER].weaponType[1] = P_ANYTHING;
	alien_defs[CD_PROTOFIGHTER].chance[0] = 100;
	alien_defs[CD_PROTOFIGHTER].chance[1] = 1;
	alien_defs[CD_PROTOFIGHTER].collectChance = 50;
	alien_defs[CD_PROTOFIGHTER].collectType = P_ANYTHING;
	alien_defs[CD_PROTOFIGHTER].collectValue = 50;
	alien_defs[CD_PROTOFIGHTER].collectTypeOriginal = P_ANYTHING;
	alien_defs[CD_PROTOFIGHTER].collectValueOriginal = 50;
	alien_defs[CD_PROTOFIGHTER].score = 50;
	alien_defs[CD_PROTOFIGHTER].flags = FL_WEAPCO;

	// Phoebe and Ursula
	alien_defs[CD_FRIEND].classDef = CD_FRIEND;
	alien_defs[CD_FRIEND].AIType = AI_OFFENSIVE;
	alien_defs[CD_FRIEND].AITypeOriginal = AI_OFFENSIVE;
	alien_defs[CD_FRIEND].speed = 3;
	alien_defs[CD_FRIEND].maxShield = 50;
	alien_defs[CD_FRIEND].shield = 50;
	alien_defs[CD_FRIEND].imageIndex[0] = SS_FRIEND;
	alien_defs[CD_FRIEND].imageIndex[1] = SS_FRIEND_L;
	alien_defs[CD_FRIEND].weaponType[0] = W_DOUBLE_SHOT;
	alien_defs[CD_FRIEND].weaponType[1] = W_HOMING_MISSILE;
	alien_defs[CD_FRIEND].chance[0] = 100;
	alien_defs[CD_FRIEND].chance[1] = 5;
	alien_defs[CD_FRIEND].collectChance = 0;
	alien_defs[CD_FRIEND].collectType = P_CASH;
	alien_defs[CD_FRIEND].collectValue = 0;
	alien_defs[CD_FRIEND].collectTypeOriginal = P_CASH;
	alien_defs[CD_FRIEND].collectValueOriginal = 0;
	alien_defs[CD_FRIEND].score = 0;
	alien_defs[CD_FRIEND].flags = FL_FRIEND;

	// Boss 1
	alien_defs[CD_FRIGATE].classDef = CD_BOSS;
	alien_defs[CD_FRIGATE].AIType = AI_NORMAL;
	alien_defs[CD_FRIGATE].AITypeOriginal = AI_NORMAL;
	alien_defs[CD_FRIGATE].speed = 2;
	alien_defs[CD_FRIGATE].maxShield = 550;
	alien_defs[CD_FRIGATE].shield = 550;
	alien_defs[CD_FRIGATE].imageIndex[0] = SS_FRIGATE;
	alien_defs[CD_FRIGATE].imageIndex[1] = SS_FRIGATE_L;
	alien_defs[CD_FRIGATE].weaponType[0] = W_MICRO_ROCKETS;
	alien_defs[CD_FRIGATE].weaponType[1] = W_ENERGYRAY;
	alien_defs[CD_FRIGATE].chance[0] = 100;
	alien_defs[CD_FRIGATE].chance[1] = 85;
	alien_defs[CD_FRIGATE].collectChance = 100;
	alien_defs[CD_FRIGATE].collectType = P_CASH;
	alien_defs[CD_FRIGATE].collectValue = 500;
	alien_defs[CD_FRIGATE].collectTypeOriginal = P_CASH;
	alien_defs[CD_FRIGATE].collectValueOriginal = 250;
	alien_defs[CD_FRIGATE].score = 500;
	alien_defs[CD_FRIGATE].flags = FL_WEAPCO | FL_NOBANTER;

	alien_defs[CD_FRIGATE_WING1].classDef = CD_FRIGATE_WING1;
	alien_defs[CD_FRIGATE_WING1].AIType = AI_NORMAL;
	alien_defs[CD_FRIGATE_WING1].AITypeOriginal = AI_NORMAL;
	alien_defs[CD_FRIGATE_WING1].speed = 2;
	alien_defs[CD_FRIGATE_WING1].maxShield = 100;
	alien_defs[CD_FRIGATE_WING1].shield = 100;
	alien_defs[CD_FRIGATE_WING1].imageIndex[0] = SS_FRIGATE_WING1;
	alien_defs[CD_FRIGATE_WING1].imageIndex[1] = SS_FRIGATE_WING1_L;
	alien_defs[CD_FRIGATE_WING1].weaponType[0] = W_TRIPLE_SHOT;
	alien_defs[CD_FRIGATE_WING1].weaponType[1] = W_ROCKETS;
	alien_defs[CD_FRIGATE_WING1].chance[0] = 100;
	alien_defs[CD_FRIGATE_WING1].chance[1] = 10;
	alien_defs[CD_FRIGATE_WING1].collectChance = 100;
	alien_defs[CD_FRIGATE_WING1].collectType = P_ANYTHING;
	alien_defs[CD_FRIGATE_WING1].collectValue = 250;
	alien_defs[CD_FRIGATE_WING1].collectTypeOriginal = P_ANYTHING;
	alien_defs[CD_FRIGATE_WING1].collectValueOriginal = 250;
	alien_defs[CD_FRIGATE_WING1].score = 500;
	alien_defs[CD_FRIGATE_WING1].flags = FL_WEAPCO | FL_DAMAGEOWNER;

	alien_defs[CD_FRIGATE_WING2].classDef = CD_FRIGATE_WING2;
	alien_defs[CD_FRIGATE_WING2].AIType = AI_NORMAL;
	alien_defs[CD_FRIGATE_WING2].AITypeOriginal = AI_NORMAL;
	alien_defs[CD_FRIGATE_WING2].speed = 2;
	alien_defs[CD_FRIGATE_WING2].maxShield = 100;
	alien_defs[CD_FRIGATE_WING2].shield = 100;
	alien_defs[CD_FRIGATE_WING2].imageIndex[0] = SS_FRIGATE_WING2;
	alien_defs[CD_FRIGATE_WING2].imageIndex[1] = SS_FRIGATE_WING2_L;
	alien_defs[CD_FRIGATE_WING2].weaponType[0] = W_TRIPLE_SHOT;
	alien_defs[CD_FRIGATE_WING2].weaponType[1] = W_ROCKETS;
	alien_defs[CD_FRIGATE_WING2].chance[0] = 100;
	alien_defs[CD_FRIGATE_WING2].chance[1] = 10;
	alien_defs[CD_FRIGATE_WING2].collectChance = 100;
	alien_defs[CD_FRIGATE_WING2].collectType = P_ANYTHING;
	alien_defs[CD_FRIGATE_WING2].collectValue = 250;
	alien_defs[CD_FRIGATE_WING2].collectTypeOriginal = P_ANYTHING;
	alien_defs[CD_FRIGATE_WING2].collectValueOriginal = 250;
	alien_defs[CD_FRIGATE_WING2].score = 500;
	alien_defs[CD_FRIGATE_WING2].flags = FL_WEAPCO | FL_DAMAGEOWNER;

	// Transport ship
	alien_defs[CD_TRANSPORTSHIP].classDef = CD_TRANSPORTSHIP;
	alien_defs[CD_TRANSPORTSHIP].AIType = AI_EVASIVE;
	alien_defs[CD_TRANSPORTSHIP].AITypeOriginal = AI_EVASIVE;
	alien_defs[CD_TRANSPORTSHIP].speed = 4;
	alien_defs[CD_TRANSPORTSHIP].maxShield = 10;
	alien_defs[CD_TRANSPORTSHIP].shield = 10;
	alien_defs[CD_TRANSPORTSHIP].imageIndex[0] = SS_TRANSPORTSHIP;
	alien_defs[CD_TRANSPORTSHIP].imageIndex[1] = SS_TRANSPORTSHIP_L;
	alien_defs[CD_TRANSPORTSHIP].weaponType[0] = W_DOUBLE_SHOT;
	alien_defs[CD_TRANSPORTSHIP].weaponType[1] = W_DOUBLE_SHOT;
	alien_defs[CD_TRANSPORTSHIP].chance[0] = 0;
	alien_defs[CD_TRANSPORTSHIP].chance[1] = 0;
	alien_defs[CD_TRANSPORTSHIP].collectChance = 100;
	alien_defs[CD_TRANSPORTSHIP].collectType = P_WEAPONS;
	alien_defs[CD_TRANSPORTSHIP].collectValue = 40;
	alien_defs[CD_TRANSPORTSHIP].collectTypeOriginal = P_WEAPONS;
	alien_defs[CD_TRANSPORTSHIP].collectValueOriginal = 30;
	alien_defs[CD_TRANSPORTSHIP].score = 25;
	alien_defs[CD_TRANSPORTSHIP].flags = FL_WEAPCO | FL_NOFIRE;

	// Cargo ship
	alien_defs[CD_CARGOSHIP].classDef = CD_CARGOSHIP;
	alien_defs[CD_CARGOSHIP].AIType = AI_EVASIVE;
	alien_defs[CD_CARGOSHIP].AITypeOriginal = AI_EVASIVE;
	alien_defs[CD_CARGOSHIP].speed = 4;
	alien_defs[CD_CARGOSHIP].maxShield = 10;
	alien_defs[CD_CARGOSHIP].shield = 10;
	alien_defs[CD_CARGOSHIP].imageIndex[0] = SS_CARGOSHIP;
	alien_defs[CD_CARGOSHIP].imageIndex[1] = SS_CARGOSHIP_L;
	alien_defs[CD_CARGOSHIP].weaponType[0] = W_DOUBLE_SHOT;
	alien_defs[CD_CARGOSHIP].weaponType[1] = W_DOUBLE_SHOT;
	alien_defs[CD_CARGOSHIP].chance[0] = 0;
	alien_defs[CD_CARGOSHIP].chance[1] = 0;
	alien_defs[CD_CARGOSHIP].collectChance = 50;
	alien_defs[CD_CARGOSHIP].collectType = P_ANYTHING;
	alien_defs[CD_CARGOSHIP].collectValue = 100;
	alien_defs[CD_CARGOSHIP].collectTypeOriginal = P_ANYTHING;
	alien_defs[CD_CARGOSHIP].collectValueOriginal = 100;
	alien_defs[CD_CARGOSHIP].score = 25;
	alien_defs[CD_CARGOSHIP].flags = FL_WEAPCO | FL_NOFIRE;

	// Weapco Miner
	alien_defs[CD_MINER].classDef = CD_MINER;
	alien_defs[CD_MINER].AIType = AI_EVASIVE;
	alien_defs[CD_MINER].AITypeOriginal = AI_EVASIVE;
	alien_defs[CD_MINER].speed = 4;
	alien_defs[CD_MINER].maxShield = 25;
	alien_defs[CD_MINER].shield = 25;
	alien_defs[CD_MINER].imageIndex[0] = SS_MINER;
	alien_defs[CD_MINER].imageIndex[1] = SS_MINER_L;
	alien_defs[CD_MINER].weaponType[0] = W_DOUBLE_SHOT;
	alien_defs[CD_MINER].weaponType[1] = W_DOUBLE_SHOT;
	alien_defs[CD_MINER].chance[0] = 0;
	alien_defs[CD_MINER].chance[1] = 0;
	alien_defs[CD_MINER].collectChance = 100;
	alien_defs[CD_MINER].collectType = P_ANYTHING;
	alien_defs[CD_MINER].collectValue = 30;
	alien_defs[CD_MINER].collectTypeOriginal = P_ANYTHING;
	alien_defs[CD_MINER].collectValueOriginal = 30;
	alien_defs[CD_MINER].score = 100;
	alien_defs[CD_MINER].flags = FL_WEAPCO | FL_NOFIRE | FL_DROPMINES;

	// Kline
	alien_defs[CD_KLINE].classDef = CD_KLINE;
	alien_defs[CD_KLINE].AIType = AI_OFFENSIVE;
	alien_defs[CD_KLINE].AITypeOriginal = AI_OFFENSIVE;
	alien_defs[CD_KLINE].speed = 5;
	alien_defs[CD_KLINE].maxShield = 2000;
	alien_defs[CD_KLINE].shield = 2000;
	alien_defs[CD_KLINE].imageIndex[0] = SS_KLINE;
	alien_defs[CD_KLINE].imageIndex[1] = SS_KLINE_L;
	alien_defs[CD_KLINE].weaponType[0] = W_TRIPLE_SHOT;
	alien_defs[CD_KLINE].weaponType[1] = W_MICRO_ROCKETS;
	alien_defs[CD_KLINE].chance[0] = 100;
	alien_defs[CD_KLINE].chance[1] = 2;
	alien_defs[CD_KLINE].collectChance = 0;
	alien_defs[CD_KLINE].collectType = P_ANYTHING;
	alien_defs[CD_KLINE].collectValue = 0;
	alien_defs[CD_KLINE].collectTypeOriginal = P_ANYTHING;
	alien_defs[CD_KLINE].collectValueOriginal = 0;
	alien_defs[CD_KLINE].score = 0;
	alien_defs[CD_KLINE].flags = FL_WEAPCO | FL_ALWAYSFACE | FL_CIRCLES | FL_NOBANTER;

	// Aim Fighter
	alien_defs[CD_AIMFIGHTER].classDef = CD_AIMFIGHTER;
	alien_defs[CD_AIMFIGHTER].AIType = AI_NORMAL;
	alien_defs[CD_AIMFIGHTER].AITypeOriginal = AI_NORMAL;
	alien_defs[CD_AIMFIGHTER].speed = 3;
	alien_defs[CD_AIMFIGHTER].maxShield = 15;
	alien_defs[CD_AIMFIGHTER].shield = 15;
	alien_defs[CD_AIMFIGHTER].imageIndex[0] = SS_AIMFIGHTER;
	alien_defs[CD_AIMFIGHTER].imageIndex[1] = SS_AIMFIGHTER_L;
	alien_defs[CD_AIMFIGHTER].weaponType[0] = W_AIMED_SHOT;
	alien_defs[CD_AIMFIGHTER].weaponType[1] = W_AIMED_SHOT;
	alien_defs[CD_AIMFIGHTER].chance[0] = 7;
	alien_defs[CD_AIMFIGHTER].chance[1] = 1;
	alien_defs[CD_AIMFIGHTER].collectChance = 75;
	alien_defs[CD_AIMFIGHTER].collectType = P_ANYTHING;
	alien_defs[CD_AIMFIGHTER].collectValue = 100;
	alien_defs[CD_AIMFIGHTER].collectTypeOriginal = P_ANYTHING;
	alien_defs[CD_AIMFIGHTER].collectValueOriginal = 100;
	alien_defs[CD_AIMFIGHTER].score = 50;
	alien_defs[CD_AIMFIGHTER].flags = FL_WEAPCO | FL_AIMS;

	// Slave ship
	alien_defs[CD_SLAVETRANSPORT].classDef = CD_SLAVETRANSPORT;
	alien_defs[CD_SLAVETRANSPORT].AIType = AI_EVASIVE;
	alien_defs[CD_SLAVETRANSPORT].AITypeOriginal = AI_EVASIVE;
	alien_defs[CD_SLAVETRANSPORT].speed = 2;
	alien_defs[CD_SLAVETRANSPORT].maxShield = 10;
	alien_defs[CD_SLAVETRANSPORT].shield = 20;
	alien_defs[CD_SLAVETRANSPORT].imageIndex[0] = SS_SLAVETRANSPORT;
	alien_defs[CD_SLAVETRANSPORT].imageIndex[1] = SS_SLAVETRANSPORT_L;
	alien_defs[CD_SLAVETRANSPORT].weaponType[0] = W_DOUBLE_SHOT;
	alien_defs[CD_SLAVETRANSPORT].weaponType[1] = W_DOUBLE_SHOT;
	alien_defs[CD_SLAVETRANSPORT].chance[0] = 0;
	alien_defs[CD_SLAVETRANSPORT].chance[1] = 0;
	alien_defs[CD_SLAVETRANSPORT].collectChance = 100;
	alien_defs[CD_SLAVETRANSPORT].collectType = P_SLAVES;
	alien_defs[CD_SLAVETRANSPORT].collectValue = 25;
	alien_defs[CD_SLAVETRANSPORT].collectTypeOriginal = P_SLAVES;
	alien_defs[CD_SLAVETRANSPORT].collectValueOriginal = 25;
	alien_defs[CD_SLAVETRANSPORT].score = 25;
	alien_defs[CD_SLAVETRANSPORT].flags = FL_WEAPCO | FL_NOFIRE | FL_NOBANTER;

	// Good Transport
	alien_defs[CD_GOODTRANSPORT].classDef = CD_GOODTRANSPORT;
	alien_defs[CD_GOODTRANSPORT].AIType = AI_EVASIVE;
	alien_defs[CD_GOODTRANSPORT].AITypeOriginal = AI_EVASIVE;
	alien_defs[CD_GOODTRANSPORT].speed = 3;
	alien_defs[CD_GOODTRANSPORT].maxShield = 75;
	alien_defs[CD_GOODTRANSPORT].shield = 75;
	alien_defs[CD_GOODTRANSPORT].imageIndex[0] = SS_GOODTRANSPORT;
	alien_defs[CD_GOODTRANSPORT].imageIndex[1] = SS_GOODTRANSPORT_L;
	alien_defs[CD_GOODTRANSPORT].weaponType[0] = W_AIMED_SHOT;
	alien_defs[CD_GOODTRANSPORT].weaponType[1] = W_AIMED_SHOT;
	alien_defs[CD_GOODTRANSPORT].chance[0] = 100;
	alien_defs[CD_GOODTRANSPORT].chance[1] = 100;
	alien_defs[CD_GOODTRANSPORT].collectChance = 0;
	alien_defs[CD_GOODTRANSPORT].collectType = P_ANYTHING;
	alien_defs[CD_GOODTRANSPORT].collectValue = 0;
	alien_defs[CD_GOODTRANSPORT].collectTypeOriginal = P_ANYTHING;
	alien_defs[CD_GOODTRANSPORT].collectValueOriginal = 0;
	alien_defs[CD_GOODTRANSPORT].score = 0;
	alien_defs[CD_GOODTRANSPORT].flags = FL_FRIEND | FL_NOFIRE | FL_AIMS;

	// Sid Wilson
	alien_defs[CD_SID].classDef = CD_SID;
	alien_defs[CD_SID].AIType = AI_NORMAL;
	alien_defs[CD_SID].AITypeOriginal = AI_NORMAL;
	alien_defs[CD_SID].speed = 3;
	alien_defs[CD_SID].maxShield = 50;
	alien_defs[CD_SID].shield = 50;
	alien_defs[CD_SID].imageIndex[0] = SS_SID;
	alien_defs[CD_SID].imageIndex[1] = SS_SID_L;
	alien_defs[CD_SID].weaponType[0] = W_IONCANNON;
	alien_defs[CD_SID].weaponType[1] = W_IONCANNON;
	alien_defs[CD_SID].chance[0] = 100;
	alien_defs[CD_SID].chance[1] = 0;
	alien_defs[CD_SID].collectChance = 0;
	alien_defs[CD_SID].collectType = P_ANYTHING;
	alien_defs[CD_SID].collectValue = 0;
	alien_defs[CD_SID].collectTypeOriginal = P_ANYTHING;
	alien_defs[CD_SID].collectValueOriginal = 0;
	alien_defs[CD_SID].score = 0;
	alien_defs[CD_SID].flags = FL_FRIEND | FL_AIMS;

	// Mining Vessel Boss
	alien_defs[CD_MINEBOSS].classDef = CD_BOSS;
	alien_defs[CD_MINEBOSS].AIType = AI_NORMAL;
	alien_defs[CD_MINEBOSS].AITypeOriginal = AI_NORMAL;
	alien_defs[CD_MINEBOSS].speed = 3;
	alien_defs[CD_MINEBOSS].maxShield = 1000;
	alien_defs[CD_MINEBOSS].shield = 1000;
	alien_defs[CD_MINEBOSS].imageIndex[0] = SS_MINERBOSS;
	alien_defs[CD_MINEBOSS].imageIndex[1] = SS_MINERBOSS_L;
	alien_defs[CD_MINEBOSS].weaponType[0] = W_TRIPLE_SHOT;
	alien_defs[CD_MINEBOSS].weaponType[1] = W_SPREADSHOT;
	alien_defs[CD_MINEBOSS].chance[0] = 0;
	alien_defs[CD_MINEBOSS].chance[1] = 0;
	alien_defs[CD_MINEBOSS].collectChance = 100;
	alien_defs[CD_MINEBOSS].collectType = P_ANYTHING;
	alien_defs[CD_MINEBOSS].collectValue = 1000;
	alien_defs[CD_MINEBOSS].collectTypeOriginal = P_ANYTHING;
	alien_defs[CD_MINEBOSS].collectValueOriginal = 255;
	alien_defs[CD_MINEBOSS].score = 1000;
	alien_defs[CD_MINEBOSS].flags = FL_WEAPCO | FL_IMMORTAL | FL_NOBANTER;

	alien_defs[CD_BOSS2_WING1].classDef = CD_BOSS2_WING1;
	alien_defs[CD_BOSS2_WING1].AIType = AI_NORMAL;
	alien_defs[CD_BOSS2_WING1].AITypeOriginal = AI_NORMAL;
	alien_defs[CD_BOSS2_WING1].speed = 1;
	alien_defs[CD_BOSS2_WING1].maxShield = 250;
	alien_defs[CD_BOSS2_WING1].shield = 250;
	alien_defs[CD_BOSS2_WING1].imageIndex[0] = SS_MINERBOSS_WING1;
	alien_defs[CD_BOSS2_WING1].imageIndex[1] = SS_MINERBOSS_WING1_L;
	alien_defs[CD_BOSS2_WING1].weaponType[0] = W_TRIPLE_SHOT;
	alien_defs[CD_BOSS2_WING1].weaponType[1] = W_SPREADSHOT;
	alien_defs[CD_BOSS2_WING1].chance[0] = 0;
	alien_defs[CD_BOSS2_WING1].chance[1] = 0;
	alien_defs[CD_BOSS2_WING1].collectChance = 100;
	alien_defs[CD_BOSS2_WING1].collectType = P_ANYTHING;
	alien_defs[CD_BOSS2_WING1].collectValue = 250;
	alien_defs[CD_BOSS2_WING1].collectTypeOriginal = P_ANYTHING;
	alien_defs[CD_BOSS2_WING1].collectValueOriginal = 255;
	alien_defs[CD_BOSS2_WING1].score = 1000;
	alien_defs[CD_BOSS2_WING1].flags = FL_WEAPCO | FL_DAMAGEOWNER | FL_NOBANTER;

	alien_defs[CD_BOSS2_WING2].classDef = CD_BOSS2_WING2;
	alien_defs[CD_BOSS2_WING2].AIType = AI_NORMAL;
	alien_defs[CD_BOSS2_WING2].AITypeOriginal = AI_NORMAL;
	alien_defs[CD_BOSS2_WING2].speed = 1;
	alien_defs[CD_BOSS2_WING2].maxShield = 500;
	alien_defs[CD_BOSS2_WING2].shield = 500;
	alien_defs[CD_BOSS2_WING2].imageIndex[0] = SS_MINERBOSS_WING2;
	alien_defs[CD_BOSS2_WING2].imageIndex[1] = SS_MINERBOSS_WING2_L;
	alien_defs[CD_BOSS2_WING2].weaponType[0] = W_TRIPLE_SHOT;
	alien_defs[CD_BOSS2_WING2].weaponType[1] = W_SPREADSHOT;
	alien_defs[CD_BOSS2_WING2].chance[0] = 0;
	alien_defs[CD_BOSS2_WING2].chance[1] = 0;
	alien_defs[CD_BOSS2_WING2].collectChance = 100;
	alien_defs[CD_BOSS2_WING2].collectType = P_ANYTHING;
	alien_defs[CD_BOSS2_WING2].collectValue = 250;
	alien_defs[CD_BOSS2_WING2].collectTypeOriginal = P_ANYTHING;
	alien_defs[CD_BOSS2_WING2].collectValueOriginal = 255;
	alien_defs[CD_BOSS2_WING2].score = 1000;
	alien_defs[CD_BOSS2_WING2].flags = FL_WEAPCO | FL_DEPLOYDRONES | FL_DAMAGEOWNER | FL_NOBANTER;

	alien_defs[CD_BOSS2_WING3].classDef = CD_BOSS2_WING3;
	alien_defs[CD_BOSS2_WING3].AIType = AI_NORMAL;
	alien_defs[CD_BOSS2_WING3].AITypeOriginal = AI_NORMAL;
	alien_defs[CD_BOSS2_WING3].speed = 1;
	alien_defs[CD_BOSS2_WING3].maxShield = 500;
	alien_defs[CD_BOSS2_WING3].shield = 500;
	alien_defs[CD_BOSS2_WING3].imageIndex[0] = SS_MINERBOSS_WING3;
	alien_defs[CD_BOSS2_WING3].imageIndex[1] = SS_MINERBOSS_WING3_L;
	alien_defs[CD_BOSS2_WING3].weaponType[0] = W_TRIPLE_SHOT;
	alien_defs[CD_BOSS2_WING3].weaponType[1] = W_SPREADSHOT;
	alien_defs[CD_BOSS2_WING3].chance[0] = 0;
	alien_defs[CD_BOSS2_WING3].chance[1] = 0;
	alien_defs[CD_BOSS2_WING3].collectChance = 100;
	alien_defs[CD_BOSS2_WING3].collectType = P_ANYTHING;
	alien_defs[CD_BOSS2_WING3].collectValue = 250;
	alien_defs[CD_BOSS2_WING3].collectTypeOriginal = P_ANYTHING;
	alien_defs[CD_BOSS2_WING3].collectValueOriginal = 255;
	alien_defs[CD_BOSS2_WING3].score = 1000;
	alien_defs[CD_BOSS2_WING3].flags = FL_WEAPCO | FL_DEPLOYDRONES | FL_DAMAGEOWNER | FL_NOBANTER;

	alien_defs[CD_BOSS2_WING4].classDef = CD_BOSS2_WING4;
	alien_defs[CD_BOSS2_WING4].AIType = AI_NORMAL;
	alien_defs[CD_BOSS2_WING4].AITypeOriginal = AI_NORMAL;
	alien_defs[CD_BOSS2_WING4].speed = 1;
	alien_defs[CD_BOSS2_WING4].maxShield = 250;
	alien_defs[CD_BOSS2_WING4].shield = 250;
	alien_defs[CD_BOSS2_WING4].imageIndex[0] = SS_MINERBOSS_WING4;
	alien_defs[CD_BOSS2_WING4].imageIndex[1] = SS_MINERBOSS_WING4_L;
	alien_defs[CD_BOSS2_WING4].weaponType[0] = W_TRIPLE_SHOT;
	alien_defs[CD_BOSS2_WING4].weaponType[1] = W_SPREADSHOT;
	alien_defs[CD_BOSS2_WING4].chance[0] = 0;
	alien_defs[CD_BOSS2_WING4].chance[1] = 0;
	alien_defs[CD_BOSS2_WING4].collectChance = 100;
	alien_defs[CD_BOSS2_WING4].collectType = P_ANYTHING;
	alien_defs[CD_BOSS2_WING4].collectValue = 250;
	alien_defs[CD_BOSS2_WING4].collectTypeOriginal = P_ANYTHING;
	alien_defs[CD_BOSS2_WING4].collectValueOriginal = 255;
	alien_defs[CD_BOSS2_WING4].score = 1000;
	alien_defs[CD_BOSS2_WING4].flags = FL_WEAPCO | FL_DAMAGEOWNER | FL_NOBANTER;

	// Drone
	alien_defs[CD_DRONE].classDef = CD_DRONE;
	alien_defs[CD_DRONE].AIType = AI_OFFENSIVE;
	alien_defs[CD_DRONE].AITypeOriginal = AI_OFFENSIVE;
	alien_defs[CD_DRONE].speed = 8;
	alien_defs[CD_DRONE].maxShield = 5;
	alien_defs[CD_DRONE].shield = 5;
	alien_defs[CD_DRONE].imageIndex[0] = SS_DRONE;
	alien_defs[CD_DRONE].imageIndex[1] = SS_DRONE_L;
	alien_defs[CD_DRONE].weaponType[0] = W_DOUBLE_SHOT;
	alien_defs[CD_DRONE].weaponType[1] = W_LASER;
	alien_defs[CD_DRONE].chance[0] = 100;
	alien_defs[CD_DRONE].chance[1] = 0;
	alien_defs[CD_DRONE].collectChance = 10;
	alien_defs[CD_DRONE].collectType = P_SHIELD;
	alien_defs[CD_DRONE].collectValue = 1;
	alien_defs[CD_DRONE].collectTypeOriginal = P_SHIELD;
	alien_defs[CD_DRONE].collectValueOriginal = 1;
	alien_defs[CD_DRONE].score = 0;
	alien_defs[CD_DRONE].flags = FL_WEAPCO | FL_NOBANTER;

	// Experimental Fighter
	alien_defs[CD_CLOAKFIGHTER].classDef = CD_CLOAKFIGHTER;
	alien_defs[CD_CLOAKFIGHTER].AIType = AI_OFFENSIVE;
	alien_defs[CD_CLOAKFIGHTER].AITypeOriginal = AI_OFFENSIVE;
	alien_defs[CD_CLOAKFIGHTER].speed = 6;
	alien_defs[CD_CLOAKFIGHTER].maxShield = 1000;
	alien_defs[CD_CLOAKFIGHTER].shield = 1000;
	alien_defs[CD_CLOAKFIGHTER].imageIndex[0] = SS_CLOAKFIGHTER;
	alien_defs[CD_CLOAKFIGHTER].imageIndex[1] = SS_CLOAKFIGHTER_L;
	alien_defs[CD_CLOAKFIGHTER].weaponType[0] = W_SPREADSHOT;
	alien_defs[CD_CLOAKFIGHTER].weaponType[1] = W_DOUBLE_ROCKETS;
	alien_defs[CD_CLOAKFIGHTER].chance[0] = 100;
	alien_defs[CD_CLOAKFIGHTER].chance[1] = 5;
	alien_defs[CD_CLOAKFIGHTER].collectChance = 100;
	alien_defs[CD_CLOAKFIGHTER].collectType = P_CASH;
	alien_defs[CD_CLOAKFIGHTER].collectValue = 250;
	alien_defs[CD_CLOAKFIGHTER].collectTypeOriginal = P_CASH;
	alien_defs[CD_CLOAKFIGHTER].collectValueOriginal = 255;
	alien_defs[CD_CLOAKFIGHTER].score = 550;
	alien_defs[CD_CLOAKFIGHTER].flags = FL_WEAPCO | FL_CANCLOAK | FL_RUNSAWAY;

	// Evil Ursula
	alien_defs[CD_EVILURSULA].classDef = CD_EVILURSULA;
	alien_defs[CD_EVILURSULA].AIType = AI_OFFENSIVE;
	alien_defs[CD_EVILURSULA].AITypeOriginal = AI_OFFENSIVE;
	alien_defs[CD_EVILURSULA].speed = 5;
	alien_defs[CD_EVILURSULA].maxShield = 500;
	alien_defs[CD_EVILURSULA].shield = 500;
	alien_defs[CD_EVILURSULA].imageIndex[0] = SS_EVILURSULA;
	alien_defs[CD_EVILURSULA].imageIndex[1] = SS_EVILURSULA_L;
	alien_defs[CD_EVILURSULA].weaponType[0] = W_TRIPLE_SHOT;
	alien_defs[CD_EVILURSULA].weaponType[1] = W_HOMING_MISSILE;
	alien_defs[CD_EVILURSULA].chance[0] = 100;
	alien_defs[CD_EVILURSULA].chance[1] = 100;
	alien_defs[CD_EVILURSULA].collectChance = 100;
	alien_defs[CD_EVILURSULA].collectType = P_ESCAPEPOD;
	alien_defs[CD_EVILURSULA].collectValue = 1;
	alien_defs[CD_EVILURSULA].collectTypeOriginal = P_ESCAPEPOD;
	alien_defs[CD_EVILURSULA].collectValueOriginal = 1;
	alien_defs[CD_EVILURSULA].score = 500;
	alien_defs[CD_EVILURSULA].flags = FL_WEAPCO | FL_NOBANTER;

	// Mercenary
	alien_defs[CD_KRASS].classDef = CD_KRASS;
	alien_defs[CD_KRASS].AIType = AI_OFFENSIVE;
	alien_defs[CD_KRASS].AITypeOriginal = AI_OFFENSIVE;
	alien_defs[CD_KRASS].speed = 5;
	alien_defs[CD_KRASS].maxShield = 1000;
	alien_defs[CD_KRASS].shield = 1000;
	alien_defs[CD_KRASS].imageIndex[0] = SS_KRASS;
	alien_defs[CD_KRASS].imageIndex[1] = SS_KRASS_L;
	alien_defs[CD_KRASS].weaponType[0] = W_SPREADSHOT;
	alien_defs[CD_KRASS].weaponType[1] = W_CHARGER;
	alien_defs[CD_KRASS].chance[0] = 100;
	alien_defs[CD_KRASS].chance[1] = 0;
	alien_defs[CD_KRASS].collectChance = 100;
	alien_defs[CD_KRASS].collectType = P_ANYTHING;
	alien_defs[CD_KRASS].collectValue = 2250;
	alien_defs[CD_KRASS].collectTypeOriginal = P_ANYTHING;
	alien_defs[CD_KRASS].collectValueOriginal = 255;
	alien_defs[CD_KRASS].score = 2000;
	alien_defs[CD_KRASS].flags = FL_FRIEND | FL_IMMORTAL | FL_NOBANTER;

	// Executive Transport
	alien_defs[CD_EXEC].classDef = CD_BOSS;
	alien_defs[CD_EXEC].AIType = AI_EVASIVE;
	alien_defs[CD_EXEC].AITypeOriginal = AI_NORMAL;
	alien_defs[CD_EXEC].speed = 5;
	alien_defs[CD_EXEC].maxShield = 1000;
	alien_defs[CD_EXEC].shield = 1000;
	alien_defs[CD_EXEC].imageIndex[0] = SS_EXEC;
	alien_defs[CD_EXEC].imageIndex[1] = SS_EXEC_L;
	alien_defs[CD_EXEC].weaponType[0] = W_SPREADSHOT;
	alien_defs[CD_EXEC].weaponType[1] = W_HOMING_MISSILE;
	alien_defs[CD_EXEC].chance[0] = 0;
	alien_defs[CD_EXEC].chance[1] = 0;
	alien_defs[CD_EXEC].collectChance = 0;
	alien_defs[CD_EXEC].collectType = P_ANYTHING;
	alien_defs[CD_EXEC].collectValue = 0;
	alien_defs[CD_EXEC].collectTypeOriginal = P_ANYTHING;
	alien_defs[CD_EXEC].collectValueOriginal = 0;
	alien_defs[CD_EXEC].score = 2000;
	alien_defs[CD_EXEC].flags = FL_WEAPCO | FL_NOFIRE | FL_NOBANTER;

	// Asteroid
	alien_defs[CD_ASTEROID].classDef = CD_ASTEROID;
	alien_defs[CD_ASTEROID].AIType = AI_WANDER;
	alien_defs[CD_ASTEROID].AITypeOriginal = AI_WANDER;
	alien_defs[CD_ASTEROID].speed = 1;
	alien_defs[CD_ASTEROID].maxShield = 50;
	alien_defs[CD_ASTEROID].shield = 50;
	alien_defs[CD_ASTEROID].imageIndex[0] = SS_ASTEROID;
	alien_defs[CD_ASTEROID].imageIndex[1] = SS_ASTEROID;
	alien_defs[CD_ASTEROID].weaponType[0] = W_SPREADSHOT;
	alien_defs[CD_ASTEROID].weaponType[1] = W_HOMING_MISSILE;
	alien_defs[CD_ASTEROID].chance[0] = 0;
	alien_defs[CD_ASTEROID].chance[1] = 0;
	alien_defs[CD_ASTEROID].collectChance = 25;
	alien_defs[CD_ASTEROID].collectType = P_ORE;
	alien_defs[CD_ASTEROID].collectValue = 1;
	alien_defs[CD_ASTEROID].collectTypeOriginal = P_ORE;
	alien_defs[CD_ASTEROID].collectValueOriginal = 1;
	alien_defs[CD_ASTEROID].score = 0;
	alien_defs[CD_ASTEROID].flags = FL_WEAPCO | FL_NOBANTER;

	alien_defs[CD_ASTEROID2].classDef = CD_ASTEROID2;
	alien_defs[CD_ASTEROID2].AIType = AI_WANDER;
	alien_defs[CD_ASTEROID2].AITypeOriginal = AI_WANDER;
	alien_defs[CD_ASTEROID2].speed = 1;
	alien_defs[CD_ASTEROID2].maxShield = 10;
	alien_defs[CD_ASTEROID2].shield = 10;
	alien_defs[CD_ASTEROID2].imageIndex[0] = SS_ASTEROID_SMALL;
	alien_defs[CD_ASTEROID2].imageIndex[1] = SS_ASTEROID_SMALL_L;
	alien_defs[CD_ASTEROID2].weaponType[0] = W_SPREADSHOT;
	alien_defs[CD_ASTEROID2].weaponType[1] = W_HOMING_MISSILE;
	alien_defs[CD_ASTEROID2].chance[0] = 0;
	alien_defs[CD_ASTEROID2].chance[1] = 0;
	alien_defs[CD_ASTEROID2].collectChance = 25;
	alien_defs[CD_ASTEROID2].collectType = P_ORE;
	alien_defs[CD_ASTEROID2].collectValue = 1;
	alien_defs[CD_ASTEROID2].collectTypeOriginal = P_ORE;
	alien_defs[CD_ASTEROID2].collectValueOriginal = 1;
	alien_defs[CD_ASTEROID2].score = 0;
	alien_defs[CD_ASTEROID2].flags = FL_WEAPCO | FL_NOBANTER;

	// Escort
	alien_defs[CD_ESCORT].classDef = CD_ESCORT;
	alien_defs[CD_ESCORT].AIType = AI_NORMAL;
	alien_defs[CD_ESCORT].AITypeOriginal = AI_NORMAL;
	alien_defs[CD_ESCORT].speed = 3;
	alien_defs[CD_ESCORT].maxShield = 200;
	alien_defs[CD_ESCORT].shield = 200;
	alien_defs[CD_ESCORT].imageIndex[0] = SS_ESCORT;
	alien_defs[CD_ESCORT].imageIndex[1] = SS_ESCORT_L;
	alien_defs[CD_ESCORT].weaponType[0] = W_LASER;
	alien_defs[CD_ESCORT].weaponType[1] = W_LASER;
	alien_defs[CD_ESCORT].chance[0] = 25;
	alien_defs[CD_ESCORT].chance[1] = 25;
	alien_defs[CD_ESCORT].collectChance = 100;
	alien_defs[CD_ESCORT].collectType = P_ANYTHING;
	alien_defs[CD_ESCORT].collectValue = 100;
	alien_defs[CD_ESCORT].collectTypeOriginal = P_ANYTHING;
	alien_defs[CD_ESCORT].collectValueOriginal = 100;
	alien_defs[CD_ESCORT].score = 450;
	alien_defs[CD_ESCORT].flags = FL_WEAPCO;

	// Mobile Ray Cannon
	alien_defs[CD_MOBILE_RAY].classDef = CD_MOBILE_RAY;
	alien_defs[CD_MOBILE_RAY].AIType = AI_OFFENSIVE;
	alien_defs[CD_MOBILE_RAY].AITypeOriginal = AI_OFFENSIVE;
	alien_defs[CD_MOBILE_RAY].speed = 5;
	alien_defs[CD_MOBILE_RAY].maxShield = 250;
	alien_defs[CD_MOBILE_RAY].shield = 250;
	alien_defs[CD_MOBILE_RAY].imageIndex[0] = SS_MOBILE_RAY;
	alien_defs[CD_MOBILE_RAY].imageIndex[1] = SS_MOBILE_RAY_L;
	alien_defs[CD_MOBILE_RAY].weaponType[0] = W_ENERGYRAY;
	alien_defs[CD_MOBILE_RAY].weaponType[1] = W_ENERGYRAY;
	alien_defs[CD_MOBILE_RAY].chance[0] = 50;
	alien_defs[CD_MOBILE_RAY].chance[1] = 0;
	alien_defs[CD_MOBILE_RAY].collectChance = 75;
	alien_defs[CD_MOBILE_RAY].collectType = P_ANYTHING;
	alien_defs[CD_MOBILE_RAY].collectValue = 100;
	alien_defs[CD_MOBILE_RAY].collectTypeOriginal = P_SHIELD;
	alien_defs[CD_MOBILE_RAY].collectValueOriginal = 2;
	alien_defs[CD_MOBILE_RAY].score = 1000;
	alien_defs[CD_MOBILE_RAY].flags = FL_WEAPCO;

	// Rebel Carrier
	alien_defs[CD_REBELCARRIER].classDef = CD_REBELCARRIER;
	alien_defs[CD_REBELCARRIER].AIType = AI_OFFENSIVE;
	alien_defs[CD_REBELCARRIER].AITypeOriginal = AI_OFFENSIVE;
	alien_defs[CD_REBELCARRIER].speed = 2;
	alien_defs[CD_REBELCARRIER].maxShield = 100;
	alien_defs[CD_REBELCARRIER].shield = 100;
	alien_defs[CD_REBELCARRIER].imageIndex[0] = SS_REBELCARRIER;
	alien_defs[CD_REBELCARRIER].imageIndex[1] = SS_REBELCARRIER_L;
	alien_defs[CD_REBELCARRIER].weaponType[0] = W_DOUBLE_ROCKETS;
	alien_defs[CD_REBELCARRIER].weaponType[1] = W_MICRO_ROCKETS;
	alien_defs[CD_REBELCARRIER].chance[0] = 50;
	alien_defs[CD_REBELCARRIER].chance[1] = 2;
	alien_defs[CD_REBELCARRIER].collectChance = 0;
	alien_defs[CD_REBELCARRIER].collectType = P_SHIELD;
	alien_defs[CD_REBELCARRIER].collectValue = 0;
	alien_defs[CD_REBELCARRIER].collectTypeOriginal = P_SHIELD;
	alien_defs[CD_REBELCARRIER].collectValueOriginal = 0;
	alien_defs[CD_REBELCARRIER].score = 0;
	alien_defs[CD_REBELCARRIER].flags = FL_FRIEND;

	// Pluto Boss
	alien_defs[CD_PLUTOBOSS].classDef = CD_PLUTOBOSS;
	alien_defs[CD_PLUTOBOSS].AIType = AI_OFFENSIVE;
	alien_defs[CD_PLUTOBOSS].AITypeOriginal = AI_OFFENSIVE;
	alien_defs[CD_PLUTOBOSS].speed = 4;
	alien_defs[CD_PLUTOBOSS].maxShield = 500;
	alien_defs[CD_PLUTOBOSS].shield = 500;
	alien_defs[CD_PLUTOBOSS].imageIndex[0] = SS_PLUTOBOSS;
	alien_defs[CD_PLUTOBOSS].imageIndex[1] = SS_PLUTOBOSS_L;
	alien_defs[CD_PLUTOBOSS].weaponType[0] = W_DOUBLE_ROCKETS;
	alien_defs[CD_PLUTOBOSS].weaponType[1] = W_MICRO_ROCKETS;
	alien_defs[CD_PLUTOBOSS].chance[0] = 50;
	alien_defs[CD_PLUTOBOSS].chance[1] = 2;
	alien_defs[CD_PLUTOBOSS].collectChance = 100;
	alien_defs[CD_PLUTOBOSS].collectType = P_ANYTHING;
	alien_defs[CD_PLUTOBOSS].collectValue = 1000;
	alien_defs[CD_PLUTOBOSS].collectTypeOriginal = P_SHIELD;
	alien_defs[CD_PLUTOBOSS].collectValueOriginal = 0;
	alien_defs[CD_PLUTOBOSS].score = 1000;
	alien_defs[CD_PLUTOBOSS].flags = FL_WEAPCO | FL_NOBANTER;

	// Pluto Boss Barrier
	alien_defs[CD_BARRIER].classDef = CD_BARRIER;
	alien_defs[CD_BARRIER].AIType = AI_OFFENSIVE;
	alien_defs[CD_BARRIER].AITypeOriginal = AI_OFFENSIVE;
	alien_defs[CD_BARRIER].speed = 1;
	alien_defs[CD_BARRIER].maxShield = 250;
	alien_defs[CD_BARRIER].shield = 250;
	alien_defs[CD_BARRIER].imageIndex[0] = SS_BARRIER;
	alien_defs[CD_BARRIER].imageIndex[1] = SS_BARRIER;
	alien_defs[CD_BARRIER].weaponType[0] = W_DOUBLE_SHOT;
	alien_defs[CD_BARRIER].weaponType[1] = W_MICRO_ROCKETS;
	alien_defs[CD_BARRIER].chance[0] = 0;
	alien_defs[CD_BARRIER].chance[1] = 0;
	alien_defs[CD_BARRIER].collectChance = 100;
	alien_defs[CD_BARRIER].collectType = P_ANYTHING;
	alien_defs[CD_BARRIER].collectValue = 25;
	alien_defs[CD_BARRIER].collectTypeOriginal = P_ANYTHING;
	alien_defs[CD_BARRIER].collectValueOriginal = 25;
	alien_defs[CD_BARRIER].score = 1000;
	alien_defs[CD_BARRIER].flags = FL_WEAPCO | FL_NOFIRE | FL_NOBANTER;

	// Neptune Boss
	alien_defs[CD_NEPTUNEBOSS].classDef = CD_NEPTUNEBOSS;
	alien_defs[CD_NEPTUNEBOSS].AIType = AI_OFFENSIVE;
	alien_defs[CD_NEPTUNEBOSS].AITypeOriginal = AI_OFFENSIVE;
	alien_defs[CD_NEPTUNEBOSS].speed = 4;
	alien_defs[CD_NEPTUNEBOSS].maxShield = 800;
	alien_defs[CD_NEPTUNEBOSS].shield = 800;
	alien_defs[CD_NEPTUNEBOSS].imageIndex[0] = SS_PLUTOBOSS;
	alien_defs[CD_NEPTUNEBOSS].imageIndex[1] = SS_PLUTOBOSS_L;
	alien_defs[CD_NEPTUNEBOSS].weaponType[0] = W_DOUBLE_SHOT;
	alien_defs[CD_NEPTUNEBOSS].weaponType[1] = W_MICRO_ROCKETS;
	alien_defs[CD_NEPTUNEBOSS].chance[0] = 100;
	alien_defs[CD_NEPTUNEBOSS].chance[1] = 0;
	alien_defs[CD_NEPTUNEBOSS].collectChance = 100;
	alien_defs[CD_NEPTUNEBOSS].collectType = P_ANYTHING;
	alien_defs[CD_NEPTUNEBOSS].collectValue = 1000;
	alien_defs[CD_NEPTUNEBOSS].collectTypeOriginal = P_ANYTHING;
	alien_defs[CD_NEPTUNEBOSS].collectValueOriginal = 25;
	alien_defs[CD_NEPTUNEBOSS].score = 1000;
	alien_defs[CD_NEPTUNEBOSS].flags = FL_WEAPCO | FL_NOBANTER;

	// Mobile Shield
	alien_defs[CD_MOBILESHIELD].classDef = CD_MOBILESHIELD;
	alien_defs[CD_MOBILESHIELD].AIType = AI_EVASIVE;
	alien_defs[CD_MOBILESHIELD].AITypeOriginal = AI_EVASIVE;
	alien_defs[CD_MOBILESHIELD].speed = 6;
	alien_defs[CD_MOBILESHIELD].maxShield = 150;
	alien_defs[CD_MOBILESHIELD].shield = 150;
	alien_defs[CD_MOBILESHIELD].imageIndex[0] = SS_MOBILESHIELD;
	alien_defs[CD_MOBILESHIELD].imageIndex[1] = SS_MOBILESHIELD_L;
	alien_defs[CD_MOBILESHIELD].weaponType[0] = W_DOUBLE_SHOT;
	alien_defs[CD_MOBILESHIELD].weaponType[1] = W_MICRO_ROCKETS;
	alien_defs[CD_MOBILESHIELD].chance[0] = 0;
	alien_defs[CD_MOBILESHIELD].chance[1] = 0;
	alien_defs[CD_MOBILESHIELD].collectChance = 100;
	alien_defs[CD_MOBILESHIELD].collectType = P_ANYTHING;
	alien_defs[CD_MOBILESHIELD].collectValue = 25;
	alien_defs[CD_MOBILESHIELD].collectTypeOriginal = P_ANYTHING;
	alien_defs[CD_MOBILESHIELD].collectValueOriginal = 25;
	alien_defs[CD_MOBILESHIELD].score = 250;
	alien_defs[CD_MOBILESHIELD].flags = FL_WEAPCO | FL_NOFIRE | FL_NOBANTER;

	// Firefly
	alien_defs[CD_FIREFLY].classDef = CD_FIREFLY;
	alien_defs[CD_FIREFLY].AIType = AI_OFFENSIVE;
	alien_defs[CD_FIREFLY].AITypeOriginal = AI_OFFENSIVE;
	alien_defs[CD_FIREFLY].speed = 5;
	alien_defs[CD_FIREFLY].maxShield = 250;
	alien_defs[CD_FIREFLY].shield = 250;
	alien_defs[CD_FIREFLY].imageIndex[0] = SS_FIREFLY;
	alien_defs[CD_FIREFLY].imageIndex[1] = SS_FIREFLY_L;
	alien_defs[CD_FIREFLY].weaponType[0] = W_TRIPLE_SHOT;
	alien_defs[CD_FIREFLY].weaponType[1] = W_DOUBLE_ROCKETS;
	alien_defs[CD_FIREFLY].chance[0] = 100;
	alien_defs[CD_FIREFLY].chance[1] = 5;
	alien_defs[CD_FIREFLY].collectChance = 100;
	alien_defs[CD_FIREFLY].collectType = P_ANYTHING;
	alien_defs[CD_FIREFLY].collectValue = 250;
	alien_defs[CD_FIREFLY].collectTypeOriginal = P_ANYTHING;
	alien_defs[CD_FIREFLY].collectValueOriginal = 250;
	alien_defs[CD_FIREFLY].score = 500;
	alien_defs[CD_FIREFLY].flags = FL_WEAPCO | FL_NOBANTER;

	// Uranus Boss
	alien_defs[CD_URANUSBOSS].classDef = CD_URANUSBOSS;
	alien_defs[CD_URANUSBOSS].AIType = AI_OFFENSIVE;
	alien_defs[CD_URANUSBOSS].AITypeOriginal = AI_OFFENSIVE;
	alien_defs[CD_URANUSBOSS].speed = 4;
	alien_defs[CD_URANUSBOSS].maxShield = 750;
	alien_defs[CD_URANUSBOSS].shield = 750;
	alien_defs[CD_URANUSBOSS].imageIndex[0] = SS_URANUSBOSS;
	alien_defs[CD_URANUSBOSS].imageIndex[1] = SS_URANUSBOSS_L;
	alien_defs[CD_URANUSBOSS].weaponType[0] = W_SPREADSHOT;
	alien_defs[CD_URANUSBOSS].weaponType[1] = W_DOUBLE_ROCKETS;
	alien_defs[CD_URANUSBOSS].chance[0] = 100;
	alien_defs[CD_URANUSBOSS].chance[1] = 5;
	alien_defs[CD_URANUSBOSS].collectChance = 100;
	alien_defs[CD_URANUSBOSS].collectType = P_ANYTHING;
	alien_defs[CD_URANUSBOSS].collectValue = 500;
	alien_defs[CD_URANUSBOSS].collectTypeOriginal = P_ANYTHING;
	alien_defs[CD_URANUSBOSS].collectValueOriginal = 250;
	alien_defs[CD_URANUSBOSS].score = 500;
	alien_defs[CD_URANUSBOSS].flags = FL_WEAPCO | FL_NOBANTER;

	// Uranus Boss Wing 1
	alien_defs[CD_URANUSBOSSWING1].classDef = CD_URANUSBOSSWING1;
	alien_defs[CD_URANUSBOSSWING1].AIType = AI_OFFENSIVE;
	alien_defs[CD_URANUSBOSSWING1].AITypeOriginal = AI_OFFENSIVE;
	alien_defs[CD_URANUSBOSSWING1].speed = 4;
	alien_defs[CD_URANUSBOSSWING1].maxShield = 250;
	alien_defs[CD_URANUSBOSSWING1].shield = 250;
	alien_defs[CD_URANUSBOSSWING1].imageIndex[0] = SS_URANUSBOSS_WING1;
	alien_defs[CD_URANUSBOSSWING1].imageIndex[1] = SS_URANUSBOSS_WING1_L;
	alien_defs[CD_URANUSBOSSWING1].weaponType[0] = W_DOUBLE_ROCKETS;
	alien_defs[CD_URANUSBOSSWING1].weaponType[1] = W_DOUBLE_ROCKETS;
	alien_defs[CD_URANUSBOSSWING1].chance[0] = 5;
	alien_defs[CD_URANUSBOSSWING1].chance[1] = 0;
	alien_defs[CD_URANUSBOSSWING1].collectChance = 100;
	alien_defs[CD_URANUSBOSSWING1].collectType = P_ANYTHING;
	alien_defs[CD_URANUSBOSSWING1].collectValue = 250;
	alien_defs[CD_URANUSBOSSWING1].collectTypeOriginal = P_ANYTHING;
	alien_defs[CD_URANUSBOSSWING1].collectValueOriginal = 250;
	alien_defs[CD_URANUSBOSSWING1].score = 500;
	alien_defs[CD_URANUSBOSSWING1].flags = FL_WEAPCO | FL_IMMORTAL | FL_DAMAGEOWNER;

	// Uranus Boss Wing 2
	alien_defs[CD_URANUSBOSSWING2].classDef = CD_URANUSBOSSWING2;
	alien_defs[CD_URANUSBOSSWING2].AIType = AI_OFFENSIVE;
	alien_defs[CD_URANUSBOSSWING2].AITypeOriginal = AI_OFFENSIVE;
	alien_defs[CD_URANUSBOSSWING2].speed = 4;
	alien_defs[CD_URANUSBOSSWING2].maxShield = 250;
	alien_defs[CD_URANUSBOSSWING2].shield = 250;
	alien_defs[CD_URANUSBOSSWING2].imageIndex[0] = SS_URANUSBOSS_WING2;
	alien_defs[CD_URANUSBOSSWING2].imageIndex[1] = SS_URANUSBOSS_WING2_L;
	alien_defs[CD_URANUSBOSSWING2].weaponType[0] = W_DOUBLE_ROCKETS;
	alien_defs[CD_URANUSBOSSWING2].weaponType[1] = W_DOUBLE_ROCKETS;
	alien_defs[CD_URANUSBOSSWING2].chance[0] = 5;
	alien_defs[CD_URANUSBOSSWING2].chance[1] = 0;
	alien_defs[CD_URANUSBOSSWING2].collectChance = 100;
	alien_defs[CD_URANUSBOSSWING2].collectType = P_ANYTHING;
	alien_defs[CD_URANUSBOSSWING2].collectValue = 250;
	alien_defs[CD_URANUSBOSSWING2].collectTypeOriginal = P_ANYTHING;
	alien_defs[CD_URANUSBOSSWING2].collectValueOriginal = 250;
	alien_defs[CD_URANUSBOSSWING2].score = 500;
	alien_defs[CD_URANUSBOSSWING2].flags = FL_WEAPCO | FL_IMMORTAL | FL_DAMAGEOWNER;
}

void aliens_init()
{
	int placeAttempt;
	int barrierSpeed;

	engine.targetIndex = -1;
	barrierSpeed = 1;

	for (int i = 0 ; i < ALIEN_MAX ; i++)
	{
		aliens[i].active = 0;
		aliens[i].shield = -1;
		aliens[i].flags = 0;
		aliens[i].badTargetCount = 0;
	}

	switch (game.area)
	{
		case MISN_START:
			aliens[0] = alien_defs[CD_TRANSPORTSHIP];
			break;

		case MISN_CERADSE:
			aliens[0] = alien_defs[CD_CARGOSHIP];
			aliens[1] = alien_defs[CD_CARGOSHIP];
			aliens[2] = alien_defs[CD_CARGOSHIP];
			aliens[3] = alien_defs[CD_CARGOSHIP];
			aliens[4] = alien_defs[CD_CARGOSHIP];
			aliens[5] = alien_defs[CD_CARGOSHIP];
			break;

		case MISN_HINSTAG:
			aliens[0] = alien_defs[CD_MISSILEBOAT];
			aliens[1] = alien_defs[CD_MISSILEBOAT];
			aliens[2] = alien_defs[CD_MISSILEBOAT];
			aliens[3] = alien_defs[CD_MISSILEBOAT];
			aliens[4] = alien_defs[CD_MISSILEBOAT];
			break;

		case MISN_JOLDAR:
			aliens[0] = alien_defs[CD_MINER];
			aliens[1] = alien_defs[CD_MINER];
			aliens[2] = alien_defs[CD_MINER];
			aliens[3] = alien_defs[CD_MINER];
			aliens[4] = alien_defs[CD_MINER];
			aliens[5] = alien_defs[CD_MINER];
			aliens[6] = alien_defs[CD_MINER];
			aliens[7] = alien_defs[CD_MINER];
			aliens[8] = alien_defs[CD_MINER];
			break;

		case MISN_MOEBO:
			aliens[ALIEN_BOSS] = alien_defs[CD_FRIGATE];
			aliens[ALIEN_BOSS_PART1] = alien_defs[CD_FRIGATE_WING1];
			aliens[ALIEN_BOSS_PART2] = alien_defs[CD_FRIGATE_WING2];
			break;

		case MISN_NEROD:
			aliens[0] = alien_defs[CD_CARGOSHIP];
			break;

		case MISN_ALLEZ:
			aliens[ALIEN_FRIEND1] = alien_defs[CD_GOODTRANSPORT];
			break;

		case MISN_URUSOR:
			aliens[ALIEN_BOSS_PART1] = alien_defs[CD_CARGOSHIP];
			aliens[ALIEN_BOSS_PART2] = alien_defs[CD_CARGOSHIP];
			aliens[ALIEN_BOSS_PART3] = alien_defs[CD_CARGOSHIP];
			aliens[ALIEN_BOSS_PART4] = alien_defs[CD_CARGOSHIP];
			aliens[ALIEN_BOSS_PART5] = alien_defs[CD_CARGOSHIP];
			break;

		case MISN_ELAMALE:
			aliens[ALIEN_BOSS] = alien_defs[CD_MINEBOSS];
			aliens[ALIEN_BOSS_PART1] = alien_defs[CD_BOSS2_WING1];
			aliens[ALIEN_BOSS_PART2] = alien_defs[CD_BOSS2_WING2];
			aliens[ALIEN_BOSS_PART3] = alien_defs[CD_BOSS2_WING3];
			aliens[ALIEN_BOSS_PART4] = alien_defs[CD_BOSS2_WING4];
			aliens[ALIEN_KLINE] = alien_defs[CD_KLINE];
			aliens[ALIEN_BOSS_PART5] = alien_defs[CD_SLAVETRANSPORT];
			aliens[ALIEN_BOSS_PART6] = alien_defs[CD_SLAVETRANSPORT];
			break;

		case MISN_ODEON:
			aliens[ALIEN_BOSS] = alien_defs[CD_EVILURSULA];
			break;

		case MISN_FELLON:
			aliens[ALIEN_BOSS] = alien_defs[CD_MINEBOSS];
			aliens[ALIEN_BOSS_PART1] = alien_defs[CD_BOSS2_WING1];
			aliens[ALIEN_BOSS_PART2] = alien_defs[CD_BOSS2_WING2];
			aliens[ALIEN_BOSS_PART3] = alien_defs[CD_BOSS2_WING3];
			aliens[ALIEN_BOSS_PART4] = alien_defs[CD_BOSS2_WING4];
			aliens[ALIEN_FRIEND1] = alien_defs[CD_REBELCARRIER];
			aliens[ALIEN_FRIEND2] = alien_defs[CD_REBELCARRIER];
			break;

		case MISN_ALMARTHA:
			aliens[ALIEN_BOSS_PART1] = alien_defs[CD_CLOAKFIGHTER];
			aliens[ALIEN_BOSS_PART2] = alien_defs[CD_CLOAKFIGHTER];
			aliens[ALIEN_BOSS_PART3] = alien_defs[CD_CLOAKFIGHTER];
			aliens[ALIEN_FRIEND1] = alien_defs[CD_KRASS];
			break;

		case MISN_POSWIC:
			aliens[0] = alien_defs[CD_ESCORT];
			aliens[1] = alien_defs[CD_ESCORT];
			aliens[2] = alien_defs[CD_ESCORT];
			aliens[3] = alien_defs[CD_ESCORT];
			aliens[4] = alien_defs[CD_ESCORT];
			aliens[ALIEN_BOSS] = alien_defs[CD_EXEC];
			break;

		case MISN_ELLESH:
			aliens[ALIEN_BOSS] = alien_defs[CD_EXEC];
			break;

		case MISN_PLUTO:
			aliens[ALIEN_BOSS] = alien_defs[CD_PLUTOBOSS];
			aliens[ALIEN_BOSS_PART1] = alien_defs[CD_BARRIER];
			aliens[ALIEN_BOSS_PART2] = alien_defs[CD_BARRIER];
			aliens[ALIEN_BOSS_PART3] = alien_defs[CD_BARRIER];
			aliens[ALIEN_BOSS_PART4] = alien_defs[CD_BARRIER];
			aliens[ALIEN_BOSS_PART5] = alien_defs[CD_BARRIER];
			aliens[ALIEN_BOSS_PART6] = alien_defs[CD_BARRIER];
			break;

		case MISN_NEPTUNE:
			aliens[ALIEN_BOSS] = alien_defs[CD_NEPTUNEBOSS];
			aliens[ALIEN_BOSS_PART1] = alien_defs[CD_MOBILESHIELD];
			aliens[ALIEN_BOSS_PART2] = alien_defs[CD_MOBILESHIELD];
			aliens[ALIEN_BOSS_PART3] = alien_defs[CD_MOBILESHIELD];
			aliens[ALIEN_BOSS_PART4] = alien_defs[CD_MOBILESHIELD];
			aliens[ALIEN_BOSS_PART5] = alien_defs[CD_MOBILESHIELD];
			break;

		case MISN_URANUS:
			aliens[ALIEN_BOSS] = alien_defs[CD_URANUSBOSS];
			aliens[ALIEN_BOSS_PART1] = alien_defs[CD_URANUSBOSSWING1];
			aliens[ALIEN_BOSS_PART2] = alien_defs[CD_URANUSBOSSWING2];
			break;

		case MISN_SATURN:
			aliens[ALIEN_BOSS_PART1] = alien_defs[CD_MOBILE_RAY];
			aliens[ALIEN_BOSS_PART2] = alien_defs[CD_MOBILE_RAY];
			aliens[ALIEN_BOSS_PART3] = alien_defs[CD_MOBILE_RAY];
			aliens[ALIEN_BOSS_PART4] = alien_defs[CD_MOBILE_RAY];
			aliens[ALIEN_BOSS_PART5] = alien_defs[CD_MOBILE_RAY];
			aliens[ALIEN_BOSS_PART6] = alien_defs[CD_MOBILE_RAY];
			break;

		case MISN_JUPITER:
			aliens[ALIEN_BOSS] = alien_defs[CD_KRASS];
			aliens[ALIEN_BOSS_PART1] = alien_defs[CD_FIREFLY];
			aliens[ALIEN_BOSS_PART2] = alien_defs[CD_FIREFLY];
			aliens[ALIEN_BOSS_PART3] = alien_defs[CD_FIREFLY];
			aliens[ALIEN_BOSS_PART4] = alien_defs[CD_FIREFLY];
			break;

		case MISN_EARTH:
		case MISN_VENUS:
			aliens[ALIEN_KLINE] = alien_defs[CD_KLINE];
			break;
	}

	for (int i = 0 ; i < ALIEN_MAX ; i++)
	{
		if (aliens[i].shield != -1)
		{
			placeAttempt = 0;

			aliens[i].owner = &aliens[i];
			aliens[i].target = &aliens[i];
			aliens[i].face = RANDRANGE(0, 1);
			aliens[i].active = 1;

			/*
			we make 1000 attempts to place this enemy since it is required. If after
			1000 attempts we still haven't managed to place the alien, then it
			simply isn't going to happen and we will just exit the game. The chances
			of this happening are very very low!
			*/
			while (1)
			{
				placeAttempt++;

				if (alien_place(&aliens[i]))
					break;

				if (placeAttempt > 1000)
					engine_showError(2, "");
			}

			if (game.area == MISN_CERADSE)
				cargo_add(&aliens[i], P_CARGO);
			else if (game.area == MISN_NEROD)
				cargo_add(&aliens[i], P_PHOEBE);

			if (i == ALIEN_KLINE)
			{
				aliens[ALIEN_KLINE].target = &player;
			}

			if (aliens[i].classDef == CD_CLOAKFIGHTER)
			{
				aliens[i].active = 0;
				aliens[i].maxShield = 400;
				aliens[i].shield = 400;
				aliens[i].flags &= ~FL_RUNSAWAY;
				aliens[i].speed = 3;
			}

			if ((aliens[i].classDef == CD_MOBILE_RAY) && (i >= ALIEN_BOSS_PART3))
			{
				aliens[i].active = 0;
			}

			if (aliens[i].classDef == CD_FIREFLY)
			{
				aliens[i].active = 0;
			}

			if (aliens[i].classDef == CD_BARRIER)
			{
				aliens[i].owner = &aliens[ALIEN_BOSS];
				aliens[i].speed = barrierSpeed;
				barrierSpeed++;
			}

			if ((game.area == MISN_POSWIC)
					&& (aliens[i].classDef == CD_BOSS))
			{
				aliens[i].flags |= FL_IMMORTAL;
			}

			if (game.area == MISN_ELLESH)
				aliens[i].flags |= FL_HASMINIMUMSPEED;

			if (game.area == MISN_JUPITER)
			{
				aliens[i].flags = FL_WEAPCO | FL_NOBANTER;
				if (i == ALIEN_BOSS)
					aliens[i].chance[1] = 5;
			}
		}
	}

	if (game.area == MISN_MOEBO)
	{
		aliens[ALIEN_BOSS].target = &player;
		aliens[ALIEN_BOSS].x = -screen->w / 2;
		aliens[ALIEN_BOSS].y = screen->h / 2;

		aliens[ALIEN_BOSS_PART1].owner = &aliens[ALIEN_BOSS];
		aliens[ALIEN_BOSS_PART1].target = &player;
		aliens[ALIEN_BOSS_PART1].dx = -25;
		aliens[ALIEN_BOSS_PART1].dy = -21;

		aliens[ALIEN_BOSS_PART2].owner = &aliens[ALIEN_BOSS];
		aliens[ALIEN_BOSS_PART2].target = &player;
		aliens[ALIEN_BOSS_PART2].dx = -20;
		aliens[ALIEN_BOSS_PART2].dy = 37;
	}
	else if ((game.area == MISN_ELAMALE)
			|| (game.area == MISN_FELLON))
	{
		aliens[ALIEN_BOSS].target = &player;
		aliens[ALIEN_BOSS].x = -screen->w / 2;
		aliens[ALIEN_BOSS].y = screen->h / 2;

		aliens[ALIEN_BOSS_PART1].owner = &aliens[ALIEN_BOSS_PART2];
		aliens[ALIEN_BOSS_PART1].target = &player;
		aliens[ALIEN_BOSS_PART1].dx = -35;
		aliens[ALIEN_BOSS_PART1].dy = -12;

		aliens[ALIEN_BOSS_PART2].owner = &aliens[ALIEN_BOSS];
		aliens[ALIEN_BOSS_PART2].target = &player;
		aliens[ALIEN_BOSS_PART2].dx = 15;
		aliens[ALIEN_BOSS_PART2].dy = -22;

		aliens[ALIEN_BOSS_PART3].owner = &aliens[ALIEN_BOSS];
		aliens[ALIEN_BOSS_PART3].target = &player;
		aliens[ALIEN_BOSS_PART3].dx = 15;
		aliens[ALIEN_BOSS_PART3].dy = 22;

		aliens[ALIEN_BOSS_PART4].owner = &aliens[ALIEN_BOSS_PART3];
		aliens[ALIEN_BOSS_PART4].target = &player;
		aliens[ALIEN_BOSS_PART4].dx = -35;
		aliens[ALIEN_BOSS_PART4].dy = 20;

		if (game.area == MISN_FELLON)
		{
			aliens[ALIEN_BOSS].AIType = AI_EVASIVE;
		}
	}
	else if (game.area == MISN_URANUS)
	{
		aliens[ALIEN_BOSS].target = &player;
		aliens[ALIEN_BOSS].x = -screen->w / 2;
		aliens[ALIEN_BOSS].y = screen->h / 2;

		aliens[ALIEN_BOSS_PART1].owner = &aliens[ALIEN_BOSS];
		aliens[ALIEN_BOSS_PART1].dy = -16;

		aliens[ALIEN_BOSS_PART2].owner = &aliens[ALIEN_BOSS];
		aliens[ALIEN_BOSS_PART2].dy = 20;
	}
}

int alien_add()
{
	int index = alien_getFreeIndex();

	if ((index == -1) || (game.area == MISN_JUPITER)
			|| (game.area == MISN_VENUS))
		return 0;

	int *alienArray;
	int numberOfAliens = 1;

	alienArray = malloc(8 * sizeof(*alienArray));
	if (alienArray == NULL)
	{
		engine_warn("Failed to allocate memory for aliens");
		return 0;
	}

	switch(game.area)
	{
		case MISN_START:
		case MISN_HINSTAG:
		case MISN_ELAMALE:
			numberOfAliens = 1;
			alienArray[0] = CD_DUALFIGHTER;
			break;
		case MISN_HAIL:
		case MISN_CERADSE:
		case MISN_JOLDAR:
		case MISN_MOEBO:
			numberOfAliens = 2;
			alienArray[0] = CD_DUALFIGHTER;
			alienArray[1] = CD_PROTOFIGHTER;
			break;
		case MISN_NEROD:
		case MISN_ALLEZ:
			numberOfAliens = 3;
			alienArray[0] = CD_DUALFIGHTER;
			alienArray[1] = CD_PROTOFIGHTER;
			alienArray[2] = CD_AIMFIGHTER;
			break;
		case MISN_URUSOR:
			// This is the mission where you need to disable cargo ships.
			// Missiles are extremely bad in this mission, not because
			// of the damage they do to you, but because they tend to
			// accidentally destroy the cargo ships. Therefore, ships
			// with missiles (dual fighters and missile boats) are
			// excluded from this mission.
			numberOfAliens = 2;
			alienArray[0] = CD_PROTOFIGHTER;
			alienArray[1] = CD_AIMFIGHTER;
			// ...That is, unless we're in Classic difficulty, because
			// the original game actually had these enemies.
			if (game.difficulty == DIFFICULTY_ORIGINAL)
			{
				numberOfAliens = 4;
				alienArray[2] = CD_DUALFIGHTER;
				alienArray[3] = CD_MISSILEBOAT;
			}
			break;
		case MISN_DORIM:
		case MISN_SIVEDI:
			numberOfAliens = 1;
			alienArray[0] = CD_ASTEROID;
			break;
		case MISN_ODEON:
		case MISN_FELLON:
		case MISN_ALMARTHA:
			numberOfAliens = 4;
			alienArray[0] = CD_DUALFIGHTER;
			alienArray[1] = CD_PROTOFIGHTER;
			alienArray[2] = CD_MISSILEBOAT;
			alienArray[3] = CD_AIMFIGHTER;
			break;
		case MISN_ELLESH:
			numberOfAliens = 2;
			alienArray[0] = CD_DUALFIGHTER;
			alienArray[1] = CD_MINER;
			break;
		case MISN_SATURN:
			numberOfAliens = 2;
			alienArray[0] = CD_AIMFIGHTER;
			alienArray[1] = CD_DUALFIGHTER;
			break;
		case MISN_MARS:
			numberOfAliens = 2;
			alienArray[0] = CD_ASTEROID;
			alienArray[1] = CD_ASTEROID2;
			break;
		case MISN_EARTH:
			numberOfAliens = 6;
			alienArray[0] = CD_DUALFIGHTER;
			alienArray[1] = CD_PROTOFIGHTER;
			alienArray[2] = CD_MISSILEBOAT;
			alienArray[3] = CD_AIMFIGHTER;
			alienArray[4] = CD_ESCORT;
			alienArray[5] = CD_MOBILE_RAY;
			break;
		case MISN_INTERCEPTION:
			numberOfAliens = 1;
			alienArray[0] = CD_DUALFIGHTER;
			if (game.system > SYSTEM_SPIRIT)
			{
				numberOfAliens = 3;
				alienArray[1] = CD_MISSILEBOAT;
				alienArray[2] = CD_AIMFIGHTER;
				if (game.system == SYSTEM_MORDOR)
				{
					numberOfAliens = 4;
					alienArray[3] = CD_PROTOFIGHTER;
				}
			}
			break;
		default:
			numberOfAliens = 1;
			alienArray[0] = CD_DUALFIGHTER;
			break;
	}

	int randEnemy = alienArray[rand() % numberOfAliens];

	if ((game.area != MISN_DORIM)
		&& (game.area != MISN_SIVEDI)
		&& (game.area != MISN_MARS))
	{
		if ((game.system == SYSTEM_EYANANTH) && (game.area == MISN_INTERCEPTION))
		{
			if (CHANCE(1. / 5.))
				randEnemy = CD_SLAVETRANSPORT;
		}

		if (CHANCE(1. / 6.))
			randEnemy = CD_TRANSPORTSHIP;
	}

	free(alienArray);

	aliens[index] = alien_defs[randEnemy];
	aliens[index].active = 1;
	aliens[index].face = RANDRANGE(0, 1);
	aliens[index].owner = &aliens[index]; // Most enemies will own themselves
	aliens[index].target = &aliens[index];
	aliens[index].thinktime = RANDRANGE(50, 100);
	aliens[index].systemPower = aliens[index].maxShield;
	aliens[index].deathCounter = 0 - (aliens[index].maxShield * 3);
	aliens[index].hit = 0;

	LIMIT(aliens[index].deathCounter, -250, 0);

	// Attempts to place an alien. If it fails, the alien is deactivated.
	if (!alien_place(&aliens[index]))
	{
		aliens[index].active = 0;
		return 0;
	}

	if (aliens[index].classDef == CD_CARGOSHIP)
		cargo_add(&aliens[index], P_CARGO);

	if (aliens[index].classDef == CD_MOBILE_RAY)
		aliens[index].shield = 25;

	if (aliens[index].classDef == CD_ESCORT)
		aliens[index].shield = 50;

	aliens[index].dx = RANDRANGE(-2, 2);
	aliens[index].dy = RANDRANGE(-2, 2);

	aliens[index].ammo[0] = 0;

	if (game.area == MISN_ELLESH)
		aliens[index].flags |= FL_HASMINIMUMSPEED;

	alien_nerf(index);

	return 1;
}

void alien_addDrone(Object *hostAlien)
{
	int index = alien_getFreeIndex();

	if (index == -1)
		return;

	aliens[index] = alien_defs[CD_DRONE];
	aliens[index].active = 1;
	aliens[index].face = RANDRANGE(0, 1);
	aliens[index].owner = &aliens[index]; // Most enemies will own themselves
	aliens[index].target = &aliens[index];
	aliens[index].thinktime = RANDRANGE(50, 100);
	aliens[index].systemPower = aliens[index].maxShield;
	aliens[index].deathCounter = 0 - (aliens[index].maxShield * 3);
	aliens[index].hit = 0;

	aliens[index].x = hostAlien->x + RANDRANGE(0, 50);
	aliens[index].y = hostAlien->y + RANDRANGE(0, 50);
	
	alien_nerf(index);
}

void alien_addSmallAsteroid(Object *hostAlien)
{
	if (engine.missionCompleteTimer != 0)
		return;

	int index = -1;
	int debris = RANDRANGE(1, 10);

	for (int i = 0 ; i < debris ; i++)
		bullet_add(&weapons[W_ROCKETS], hostAlien, 0, 0);

	for (int i = 0 ; i <= ALIEN_NORMAL_LAST ; i++)
		if (!aliens[i].active)
			index = i;

	if (index == -1)
		return;

	if (CHANCE(3 / 5.))
	{
		aliens[index] = alien_defs[CD_ASTEROID2];
		aliens[index].imageIndex[0] = RANDRANGE(SS_ASTEROID_SMALL, SS_ASTEROID_SMALL_L);
		aliens[index].imageIndex[1] = aliens[index].imageIndex[0];
		aliens[index].image[0] = gfx_shipSprites[aliens[index].imageIndex[0]];
		aliens[index].image[1] = gfx_shipSprites[aliens[index].imageIndex[1]];
	}
	else
	{
		aliens[index] = alien_defs[CD_DRONE];
	}

	aliens[index].owner = &aliens[index]; // Most enemies will own themselves
	aliens[index].target = &aliens[index];
	aliens[index].thinktime = 1;
	aliens[index].systemPower = aliens[index].maxShield;
	aliens[index].deathCounter = 0 - (aliens[index].maxShield * 3);
	aliens[index].hit = 0;

	aliens[index].x = hostAlien->x;
	aliens[index].y = hostAlien->y;
	aliens[index].active = 1;
}

void alien_addFriendly(int type)
{
	if (type != ALIEN_SID)
		aliens[type] = alien_defs[CD_FRIEND];
	else
		aliens[type] = alien_defs[CD_SID];

	aliens[type].owner = &aliens[type];
	aliens[type].target = &aliens[type];
	aliens[type].active = 1;
	aliens[type].x = RANDRANGE((screen->w / 2) - 150, (screen->w / 2) + 150);
	aliens[type].y = RANDRANGE((screen->h / 2) - 150, (screen->h / 2) + 150);

	if (type == ALIEN_PHOEBE)
		aliens[type].classDef = CD_PHOEBE;

	if (type == ALIEN_URSULA)
		aliens[type].classDef = CD_URSULA;

	// For the sake of it being the final battle :)
	if (game.area == MISN_EARTH)
		aliens[type].flags |= FL_IMMORTAL;
	
	alien_nerf(type);
}

int alien_place(Object *alien)
{
	if (CHANCE(0.5))
		alien->x = RANDRANGE(screen->w, screen->w * 2);
	else
		alien->x = RANDRANGE(-screen->w, 0);

	if (CHANCE(0.5))
		alien->y = RANDRANGE(screen->h, screen->h * 2);
	else
		alien->y = RANDRANGE(-screen->h, 0);

	if (game.area == MISN_MARS)
	{
		if (game.difficulty == DIFFICULTY_SUPEREASY)
		{
			alien->x = screen->w + RANDRANGE(0, 1200);
			alien->y = RANDRANGE(-screen->h, screen->h);
		}
		else
		{
			alien->x = screen->w + RANDRANGE(0, 400);
			alien->y = RANDRANGE(-screen->h / 3, (4 * screen->h) / 3);
		}
	}

	for (int i = 0 ; i < ALIEN_MAX ; i++)
	{
		if ((aliens[i].owner != alien) && (aliens[i].shield > 0))
		{
			if (ship_collision(alien, &aliens[i]))
				return 0;
		}
	}

	return 1;
}

void alien_setAI(Object *alien)
{
	int i;
	float tx;
	float ty;

	int chase = 0; // Chance in 10 of chasing player
	int area = 0; // Chance in 10 of moving to an area around the player
	int stop = 0; // Chance in 10 of hanging back
	int point = 0; // Size of area alien will move into

	// Make friendly craft generally concentrate on smaller fighters
	if ((alien->flags & FL_FRIEND) && (alien->target == &aliens[ALIEN_BOSS]))
	{
		if (CHANCE(0.2))
		{
			alien->target = alien;
			alien->thinktime = 0;
			return;
		}
	}

	i = RANDRANGE(0, 9);
	tx = alien->target->x;
	ty = alien->target->y;

	switch ((game.difficulty == DIFFICULTY_ORIGINAL) ?
			alien->AITypeOriginal : alien->AIType)
	{
		case AI_NORMAL:
			chase = 3;
			point = 6;
			stop = 9;
			area = 250;
			break;
		case AI_OFFENSIVE:
			chase = 7;
			point = 8;
			stop = 9;
			area = 50;
			break;
		case AI_DEFENSIVE:
			chase = 2;
			point = 6;
			stop = 8;
			area = 300;
			break;
		case AI_EVASIVE:
			chase = 1;
			point = 8;
			stop = 9;
			area = 600;
			break;
		case AI_WANDER:
			chase = -1;
			point = 0;
			stop = 10;
			area = 1200;
			break;
	}

	if (i <= chase)
	{
		// Chase the target
		alien->dx = ((alien->x - tx) / ((300 / alien->speed) + RANDRANGE(0, 100)));
		alien->dy = ((alien->y - ty) / ((300 / alien->speed) + RANDRANGE(0, 100)));
		return;
	}
	else if ((i >= point) && (i <= stop))
	{
		// Fly to a random point around the target
		tx += RANDRANGE(-area, area);
		ty += RANDRANGE(-area, area);
		alien->dx = ((alien->x - tx) / ((300 / alien->speed) + RANDRANGE(0, 100)));
		alien->dy = ((alien->y - ty) / ((300 / alien->speed) + RANDRANGE(0, 100)));
		return;
	}
	else
	{
		// Hang back
		alien->dx = 0;
		alien->dy = 0;
		return;
	}
}

void alien_setKlineAttackMethod(Object *alien)
{
	if (alien->shield <= alien->maxShield / 4)
	{
		/// Dialog (Kline Kethlan)
		/// Used when the final "stage" of the Venus fight is entered.
		radio_setMessage(FS_KLINE, _("ENOUGH! THIS ENDS NOW!"), 1);
		alien->weaponType[0] = W_AIMED_SHOT;
		alien->weaponType[1] = W_MICRO_HOMING_MISSILES;
		alien->flags |= FL_CANCLOAK;
		alien->chance[0] = 100;
		alien->chance[1] = 2;
	}
	else if (alien->shield <= alien->maxShield / 2)
	{
		/// Dialog (Kline Kethlan)
		/// Used when the third "stage" of the Vinus fight is entered.
		radio_setMessage(FS_KLINE, _("Your ability to stay alive irritates me! Try dodging some of these!"), 1);
		if (game.difficulty == DIFFICULTY_ORIGINAL)
		{
			alien->weaponType[0] = W_DIRSHOCKMISSILE;
			alien->chance[0] = 2;
		}
		alien->weaponType[1] = W_DIRSHOCKMISSILE;
		alien->chance[1] = 2;
		alien->flags |= FL_AIMS;
	}
	else
	{
		/// Dialog (Kline Kethlan)
		/// Used when the second "stage" of the Venus fight is entered.
		radio_setMessage(FS_KLINE, _("Very good, Bainfield. Now let's get a little more serious..."), 1);
		alien->weaponType[0] = W_SPREADSHOT;
		alien->chance[1] = 40;
	}
}

/*
This AI is exclusively for Kline.
*/
void alien_setKlineAI(Object *alien)
{
	int threshold;

	// Weapon type change
	if (CHANCE(1. / 3.))
	{
		if ((game.area != MISN_VENUS)
			|| (game.difficulty != DIFFICULTY_ORIGINAL))
		{
			alien->flags &= ~FL_AIMS;

			if (CHANCE(0.5))
			{
				if (game.difficulty == DIFFICULTY_SUPEREASY)
					threshold = 750;
				else
					threshold = 1500;

				if ((game.area != MISN_VENUS)
						|| (alien->shield > threshold))
					alien->weaponType[0] = W_TRIPLE_SHOT;
				else
					alien->weaponType[0] = W_SPREADSHOT;
			}
			else
			{
				alien->weaponType[0] = W_AIMED_SHOT;
				alien->flags |= FL_AIMS;

			}
		}
	}

	alien->flags &= ~(FL_CIRCLES | FL_CONTINUOUS_FIRE | FL_DROPMINES);

	switch(RANDRANGE(0, 9))
	{
		case 0:
			if ((alien->weaponType[0] != W_DIRSHOCKMISSILE)
					&& (alien->weaponType[1] != W_MICRO_HOMING_MISSILES))
				alien->flags |= FL_CONTINUOUS_FIRE;
			alien->dx = ((alien->x - alien->target->x) /
				((300 / alien->speed)  + RANDRANGE(0, 100)));
			alien->dy = ((alien->y - alien->target->y) /
				((300 / alien->speed)  + RANDRANGE(0, 100)));
			break;
		case 1:
		case 2:
			// Kline only attacks when he is ready!
			if ((!(alien->flags & FL_NOFIRE))
					&& ((game.area == MISN_ELAMALE)
						|| game.difficulty != DIFFICULTY_ORIGINAL))
				alien->flags |= FL_DROPMINES;
			break;
		case 3:
		case 4:
			alien->flags |= FL_CIRCLES;
			break;
		default:
			alien_setAI(alien);
			break;
	}
}

/*
"Looks" for an enemy by picking a randomly active enemy and using them
as a target. If the target is too far away, it will be ignored.
*/
void alien_searchForTarget(Object *alien)
{
	int i;
	Object *targetEnemy;
	int badTarget = 0;

	if (alien->flags & FL_WEAPCO)
	{
		if (CHANCE(1 / 10.))
		{
			alien->target = &player;
			alien->badTargetCount = 0;
			return;
		}
	}

	i = rand() % ALIEN_MAX;
	targetEnemy = &aliens[i];

	// Tell Sid not to attack craft that are already disabled or can
	// return fire. This will save him from messing about (unless we're on the last mission)
	if ((alien->classDef == CD_SID) && (game.area != MISN_EARTH))
	{
		if (targetEnemy->flags & FL_DISABLED)
			return;

		if (game.difficulty == DIFFICULTY_ORIGINAL)
		{
			if (!(targetEnemy->flags & FL_NOFIRE))
				badTarget = 1;
		}
		else
		{
			if ((targetEnemy->classDef != CD_CARGOSHIP)
					&& (targetEnemy->classDef != CD_BOSS))
			{
				badTarget = 1;
			}
		}
	}

	// Tell Phoebe and Ursula not to attack ships that cannot fire or are disabled (unless we're on the last mission)
	if (game.area != MISN_EARTH)
	{
		if ((alien->classDef == CD_PHOEBE) || (alien->classDef == CD_URSULA))
		{
			// Don't attack the boss or we could be here all day(!)
			if (targetEnemy->classDef == CD_BOSS)
				return;

			if ((targetEnemy->flags & FL_DISABLED)
					|| (targetEnemy->flags & FL_NOFIRE))
				badTarget = 1;
		}
	}

	if ((targetEnemy->shield < 1) || (!targetEnemy->active))
		return;

	if ((targetEnemy->flags & FL_WEAPCO) && (alien->flags & FL_WEAPCO))
		return;

	if ((targetEnemy->flags & FL_FRIEND) && (alien->flags & FL_FRIEND))
		return;

	if (abs((int)alien->x - (int)alien->target->x) > 550)
		badTarget = 1;

	if (abs((int)alien->y - (int)alien->target->y) > 400)
		badTarget = 1;

	if (badTarget)
	{
		if (game.difficulty == DIFFICULTY_ORIGINAL)
		{
			return;
		}
		else
		{
			alien->badTargetCount++;
			if (alien->badTargetCount < BAD_TARGET_ALLOW_TIME)
				return;
		}
	}

	alien->target = targetEnemy;
	alien->badTargetCount = 0;
}

/*
Do various checks to see if the alien can fire at the target.
*/
int alien_checkTarget(Object *alien)
{
	// No target
	if (alien->target == alien)
		return 0;

	// Whilst firing a Ray, no other weapons can be fired!
	if (alien->flags & FL_FIRERAY)
		return 0;

	// The target is on the same side as you!
	if ((alien->flags & FL_WEAPCO) && (alien->target->flags & FL_WEAPCO))
		return 0;
	if ((alien->flags & FL_FRIEND) && (alien->target->flags & FL_FRIEND))
		return 0;

	// You're facing the wrong way
	if ((alien->face == 0) && (alien->target->x < alien->x))
		return 0;
	if ((alien->face == 1) && (alien->target->x > alien->x))
		return 0;

	// Slightly more than half a screen away from you
	if (abs((int)alien->x - (int)alien->target->x) > 550)
		return 0;

	if ((alien->flags & FL_AIMS) || (alien->flags & FL_CONTINUOUS_FIRE))
		return 1;

	// Not at the correct vertical height
	if ((alien->y < alien->target->y - 15)
			|| (alien->y > alien->target->y + alien->target->image[0]->h + 15))
		return 0;

	return 1;
}

/*
Currently only used for the allies. Whilst flying around, the allies will fire on
any enemy craft that enter their line of sight.
*/
int alien_enemiesInFront(Object *alien)
{
	for (int i = 0 ; i < ALIEN_MAX ; i++)
	{
		if ((alien != &aliens[i]) && (aliens[i].flags & FL_WEAPCO)
				&& (aliens[i].shield > 0))
		{
			if ((alien->y > aliens[i].y - 15)
					&& (alien->y < aliens[i].y + aliens[i].image[0]->h + 15))
			{
				if ((alien->face == 1) && (aliens[i].x < alien->x))
					return 1;
				if ((alien->face == 0) && (aliens[i].x > alien->x))
					return 1;
			}
		}
	}

	return 0;
}

void alien_move(Object *alien)
{
	int checkCollisions;
	int collided = 0;

	if ((alien->flags & FL_LEAVESECTOR) || (alien->shield < 1))
		checkCollisions = 0;
	else
		checkCollisions = 1;

	if (alien->owner == alien)
	{
		if ((alien->flags & FL_CIRCLES) && (!(alien->flags & FL_LEAVESECTOR)))
		{
			if (alien->face == 0)
			{
				alien->dx += 0.02;
				alien->dy += 0.02;
			}
			else
			{
				alien->dx -= 0.02;
				alien->dy -= 0.02;
			}

			alien->x -= (sinf(alien->dx) * 4);
			alien->y -= (cosf(alien->dy) * 4);
		}
		else
		{
			alien->x -= alien->dx;
			alien->y -= alien->dy;
		}
	}

	if (checkCollisions)
	{
		for (int i = 0 ; i < ALIEN_MAX ; i++)
		{
			if ((aliens[i].owner != alien) && (aliens[i].active)
					&& (aliens[i].shield > 0)
					&& (!(aliens[i].flags & FL_LEAVESECTOR))
					&& (alien->classDef != CD_BARRIER)
					&& ship_collision(alien, &aliens[i]))
			{
				if ((game.difficulty == DIFFICULTY_ORIGINAL)
						&& (alien->classDef != CD_DRONE)
						&& (alien->classDef != CD_ASTEROID)
						&& (alien->classDef != CD_ASTEROID2)
						&& (alien->owner == alien)
						&& (game.area != MISN_ELLESH))
				{
					collided = 1;
				}

				if (aliens[i].classDef == CD_BARRIER)
				{
					alien->shield--;
					alien->hit = 3;
					alien->dx *= -1;
					alien->dy *= -1;
					audio_playSound(SFX_HIT, alien->x, alien->y);
				}
			}
		}

		// Handle a collision with the player
		if ((player.shield > 0) && ship_collision(alien, &player))
		{
			if (alien->classDef == CD_ASTEROID)
			{
				player_damage(alien->shield, 0);
				alien->shield = 0;
				audio_playSound(SFX_EXPLOSION, alien->x, alien->y);
			}

			if (alien->classDef == CD_ASTEROID2)
			{
				player_damage(alien->shield, 0);
				alien->shield = 0;
				audio_playSound(SFX_EXPLOSION, alien->x, alien->y);
			}

			if (alien->classDef == CD_BARRIER)
			{
				player_damage(1, 0);
			}
		}

		// Ship collision (only in Classic difficulty)
		if (collided)
		{
			if (alien->flags & FL_CIRCLES)
			{
				if (alien->face == 0)
				{
					alien->dx -= 0.02;
					alien->dy -= 0.02;
				}
				else
				{
					alien->dx += 0.02;
					alien->dy += 0.02;
				}

				alien->x += (sinf(alien->dx) * 4);
				alien->y += (cosf(alien->dy) * 4);

				alien->thinktime = 0;
			}
			else
			{
				alien->x += alien->dx;
				alien->y += alien->dy;

				alien->dx *= -0.2;
				alien->dy *= -0.2;

				LIMIT(alien->thinktime, 0, 15);
			}
		}
	}
}

/*
Fill in later...
*/
void alien_destroy(Object *alien, Object *attacker)
{
	int collect_type, collect_value, value;
	char msg[STRMAX];

	audio_playSound(SFX_EXPLOSION, alien->x, alien->y);

	// Chain reaction damage if needed (Classic Difficulty version)
	if ((game.difficulty == DIFFICULTY_ORIGINAL)
			&& (alien->owner != alien) && (alien->flags & FL_DAMAGEOWNER))
	{
		alien_hurt(alien->owner, attacker, alien->maxShield, 0);
	}

	if (alien->flags & FL_FRIEND)
	{
		if (alien->classDef == CD_PHOEBE)
			game.wingMate1Ejects++;
		else if (alien->classDef == CD_URSULA)
			game.wingMate2Ejects++;

		// Phoebe cannot eject on the rescue mission
		if (game.area != MISN_NEROD)
		{
			if ((alien->classDef == CD_PHOEBE) || (alien->classDef == CD_URSULA))
				info_setLine(_(">> Ally has ejected! <<"), FONT_RED);
			else
				info_setLine(_(">> Friendly craft has been destroyed! <<"), FONT_RED);
		}
	}

	if (attacker != NULL)
	{
		if (attacker == &player)
		{
			game.totalKills++;
			if (game.difficulty == DIFFICULTY_ORIGINAL)
			{
				if (game.area != MISN_INTERCEPTION)
				{
					game.cash += alien->score;
					game.cashEarned += alien->score;
				}
			}

			if (CHANCE(1 / 16.) && (alien->flags & FL_WEAPCO)
					&& (!(alien->flags & FL_NOBANTER)))
			{
				radio_getRandomMessage(msg, _(
					/// Chris brag messages
					/// This is a list of brags separated by "\n".  They are randomly
					/// broadcast when Chris successfully kills an enemy.
					/// Instead of directly translating these, please populate the list
					/// with brag messages that work well in the target language,
					/// following the English version only as a general guideline.  Any
					/// number of brag messages is permitted.
					"Take that, robot oppressors!\n"
					"Come on, WEAPCO, give me a challenge already!\n"
					"Is that all you've got?\n"
					"I could kill these robots with my hands tied behind my back!\n"
					"And now you're nothing but a pile of scrap metal!\n"
					"Who else wants some?!\n"
					"Humans do it better!\n"
					"A century of AI research is no match for me!\n"
					"What's the matter, WEAPCO? Can't keep up?\n"
					"I eat robots like you for breakfast!\n"
					"Target destroyed!\n"
					"Bring it on, WEAPCO!\n"
					"I wish the guys back at home could see this!\n"
					"How do you like that, WEAPCO?\n"
					"Maybe you should change your name to WEEPCO!"
				));
				radio_setMessage(FS_CHRIS, msg, 0);
			}
		}
		else if (attacker->classDef == CD_PHOEBE)
		{
			game.wingMate1Kills++;
			if (CHANCE(1 / 8.) && (alien-> flags & FL_WEAPCO)
					&& (!(alien->flags & FL_NOBANTER)))
			{
				radio_getRandomMessage(msg, _(
					/// Phoebe brag messages
					/// This is a list of brags separated by "\n".  They are randomly
					/// broadcast when Phoebe successfully kills an enemy.
					/// Instead of directly translating these, please populate the list
					/// with brag messages that work well in the target language,
					/// following the English version only as a general guideline.  Any
					/// number of brag messages is permitted.
					"I got another one!\n"
					"Target destroyed!\n"
					"One more for me!\n"
					"Yes! Did you see that, Chris?\n"
					"Hey, Chris, remind me to check my kill count later!\n"
					"Bring it on, WEAPCO!\n"
					"Take that, WEAPCO!\n"
					"My kill count is going up!\n"
					"Another one bites the dust!\n"
					"Yeah! Nothing can stand in our way!\n"
					"I got it!"
				));
				radio_setMessage(FS_PHOEBE, msg, 0);
			}
		}
		else if (attacker->classDef == CD_URSULA)
		{
			game.wingMate2Kills++;
			if (CHANCE(1 / 8.) && (alien-> flags & FL_WEAPCO)
					&& (!(alien->flags & FL_NOBANTER)))
			{
				radio_getRandomMessage(msg, _(
					/// Ursula brag messages
					/// This is a list of brags separated by "\n".  They are randomly
					/// broadcast when Ursula successfully kills an enemy.
					/// Instead of directly translating these, please populate the list
					/// with brag messages that work well in the target language,
					/// following the English version only as a general guideline.  Any
					/// number of brag messages is permitted.
					"Kicked your ass!\n"
					"You ain't so tough!\n"
					"I was always a better WEAPCO pilot than you!\n"
					"Target destroyed!\n"
					"That'll teach you!\n"
					"Take that, you cruddy robots!\n"
					"Is that all you've got?\n"
					"Who else wants some?!\n"
					"I'm not letting you beat me, Phoebe!\n"
					"Bring it on, you mindless drones!\n"
					"Oh, I'm sorry, are you getting your metal asses handed to you?\n"
					"No one messes with the Lexx family!\n"
					"Die, damned WEAPCO drones!\n"
					"Don't think you can get away from me!"
				));
				radio_setMessage(FS_URSULA, msg, 0);
			}
		}
		else
		{
			game.totalOtherKills++;
		}
	}

	mission_updateRequirements(M_DESTROY_TARGET_TYPE, alien->classDef, 1);
	mission_updateRequirements(M_PROTECT_TARGET, alien->classDef, 1);

	if (CHANCE(alien->collectChance / 100.))
	{
		if (game.difficulty == DIFFICULTY_ORIGINAL)
		{
			collect_type = alien->collectTypeOriginal;
			collect_value = alien->collectValueOriginal;
		}
		else
		{
			collect_type = alien->collectType;
			collect_value = alien->collectValue;
		}

		if (CHANCE(1 / 10.))
			collect_value *= 2;

		while (collect_value > 0)
		{
			value = RANDRANGE(10, 100);
			if (value > collect_value)
				value = collect_value;
			collectable_add(alien->x, alien->y, collect_type, value, MAX(600, 2 * collect_value));
			collect_value -= value;
		}
	}

	// Make it explode immediately
	if (alien->classDef == CD_ASTEROID)
	{
		alien->shield = -999;
	}

	if ((alien->classDef == CD_KRASS) && (attacker == &player))
	{
		/// Dialog (Chris Bainfield)
		/// Used when Krass Tyler is beaten in the Jupiter mission.
		radio_setMessage(FS_CHRIS, _("The name's CHRIS, old-timer."), 1);
	}

	if (alien->classDef == CD_KLINE)
	{
		/// Dialog (Kline Kethlan)
		/// Used when Kline is killed in the Venus mission.
		radio_setMessage(FS_KLINE, _("It was an honor... to have fought you..."), 1);
		alien->dx = alien->dy = 0;
		alien->shield = -150;
	}
}

void alien_hurt(Object *alien, Object *attacker, int damage, int ion)
{
	int ai_type;
	double run_chance;
	int stage1_shield, stage2_shield, stage3_shield;

	ai_type = ((game.difficulty == DIFFICULTY_ORIGINAL) ?
		alien->AITypeOriginal : alien->AIType);

	if (ion)
		alien->systemPower -= damage;
	else
		alien->shield -= damage;

	// Chain reaction damage if needed
	if ((game.difficulty != DIFFICULTY_ORIGINAL)
		&& (alien->owner != alien) && (alien->flags & FL_DAMAGEOWNER))
	{
		alien_hurt(alien->owner, attacker, damage, ion);
	}

	if (alien->classDef == CD_KLINE)
	{
		if (game.area == MISN_ELAMALE)
		{
			if (game.difficulty == DIFFICULTY_ORIGINAL)
				stage1_shield = KLINE_SHIELD_MEDIUM;
			else
				stage1_shield = KLINE_SHIELD_SMALL;

			if ((alien->shield <= alien->maxShield - stage1_shield)
				&& !(alien->flags & FL_LEAVESECTOR))
			{
				alien->flags |= FL_LEAVESECTOR;
				/// Dialog (Kline Kethlan)
				/// Used when Kline is beaten in the Elamale mission.
				radio_setMessage(FS_KLINE, _("Seems I underestimated you, Bainfield. We'll meet again!"), 1);
			}
		}
		else if (game.area == MISN_EARTH)
		{
			if (game.difficulty == DIFFICULTY_ORIGINAL)
				stage1_shield = KLINE_SHIELD_SMALL;
			else
				stage1_shield = KLINE_SHIELD_MEDIUM;

			if ((alien->shield <= alien->maxShield - stage1_shield)
				&& !(alien->flags & FL_LEAVESECTOR))
			{
				alien->flags |= FL_LEAVESECTOR;
				/// Dialog (Sid Wilson)
				/// Used when Kline is beaten in the Earth mission.
				radio_setMessage(FS_SID, _("Chris, Kethlan is getting away!"), 1);
			}
		}
		else if (game.area == MISN_VENUS)
		{
			stage1_shield = KLINE_STAGE1_SHIELD;
			stage2_shield = KLINE_STAGE2_SHIELD;
			stage3_shield = KLINE_STAGE3_SHIELD;

			if (alien->shield + damage > stage1_shield
					&& alien->shield <= stage1_shield)
			{
				alien_setKlineAttackMethod(alien);
				if (game.difficulty == DIFFICULTY_ORIGINAL)
					alien->shield = stage1_shield;
			}
			else if (alien->shield + damage > stage2_shield
					&& alien->shield <= stage2_shield)
			{
				alien_setKlineAttackMethod(alien);
				if (game.difficulty == DIFFICULTY_ORIGINAL)
					alien->shield = stage2_shield;
			}
			else if (alien->shield + damage > stage3_shield
					&& alien->shield <= stage3_shield)
			{
				alien_setKlineAttackMethod(alien);
				if (game.difficulty == DIFFICULTY_ORIGINAL)
					alien->shield = stage3_shield;
			}
		}
		else
		{
			stage1_shield = KLINE_SHIELD_TINY;

			if ((alien->shield <= alien->maxShield - stage1_shield)
				&& !(alien->flags & FL_LEAVESECTOR))
			{
				alien->flags |= FL_LEAVESECTOR;
			}
		}
	}

	run_chance = (game.difficulty == DIFFICULTY_ORIGINAL) ? 0.02 : damage / 50.;

	if ((alien->flags & FL_RUNSAWAY) && CHANCE(run_chance))
	{
		alien->flags |= FL_LEAVESECTOR;
	}

	audio_playSound(SFX_HIT, alien->x, alien->y);
	if (ai_type == AI_EVASIVE)
		alien->thinktime = 0;

	if (alien->shield < 1)
		alien_destroy(alien, attacker);

	if (alien->systemPower < 1)
	{
		if (!(alien->flags & FL_DISABLED))
		{
			alien->flags += FL_DISABLED;
			mission_updateRequirements(M_DISABLE_TARGET,
				alien->classDef, 1);
		}

		alien->systemPower = 0;
	}
}
