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

#ifndef SHOP_H
#define SHOP_H

#include "defs.h"
#include "structs.h"

#define SHOP_WIDTH 600
#define SHOP_HEIGHT 336
#define SHOP_X (screen->w / 2 - SHOP_WIDTH / 2)
#define SHOP_Y (INTERMISSION_YCENTER - SHOP_HEIGHT / 2)

void shop_init();
void shop_show();

#endif
