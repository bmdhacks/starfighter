/*
Copyright (C) 2003 Parallel Realities
Copyright (C) 2011 Guus Sliepen
Copyright (C) 2015-2023 Diligent Circle <diligentcircle@riseup.net>

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

#ifndef INTERMISSION_H
#define INTERMISSION_H

#include "defs.h"
#include "structs.h"

extern Planet intermission_planets[MAX_PLANETS];

#define INTERMISSION_YCENTER (50 + (screen->h - 150) / 2)

// Intermission Menu Sections
enum {
  INTERMISSION_START_OR_GOTO = 0,
  INTERMISSION_VIEW_MAP = 1,
  INTERMISSION_CURRENT_STATUS = 2,
  INTERMISSION_SAVE = 3,
  INTERMISSION_UPGRADE = 4,
  INTERMISSION_MISSIONS = 5,
  INTERMISSION_OPTIONS = 6,
  INTERMISSION_EXIT = 7,
  INTERMISSION_INTERCEPTION = 8
};

#define INTERMISSION_ICON_SIZE 62
	
void intermission_initPlanets(int system);
void intermission_unlockPlanets();
void intermission_updateSystemStatus();
int intermission();

#endif
