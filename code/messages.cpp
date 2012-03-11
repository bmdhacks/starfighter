/*
Copyright (C) 2003 Parallel Realities

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "Starfighter.h"

static const char *deathMessage[6] = {
	"Oh my God... No!",
	"NOOOOOOOOOOOOOOOOOOOOOOOOOOO!!!!",
	"Please tell me that didn't just happen...",
	"Chris, Answer Me!!",
	"What the hell happened?!",
	"Chriiiiiiiiiiiiiiiiiiiiiiiiiiis!!!!",
};

static const char *killMessage[10] = {
	"Chalk another one up for me!",
	"That'll teach you!",
	"One more for me!",
	"Target destroyed!",
	"You aint so tough!",
	"Kicked your ass!",

	"That takes me up to %d",

	"Hey %s, you asleep over there?!",
	"I'm catching up with you, %s!",

	"Number One, Baby!",
};

static const char *missFireMessage[5] = {
	"I am NOT your enemy!",
	"Hey! Watch it!",
	"What are you doing?! Shoot THEM!",
	"OW!!! I hope that was an accident!",
	"Open your eyes!!",
};

static const char *playerHitMessage[3] = {
	"Oops! Sorry!",
	"Get out of the way!",
	"Don't fly into my missiles!",
};

void getKillMessage(object *ally)
{
	char in[50], name[30], otherName[30];
	int kills, difference;
	bool firstPlace = false;
	int faceToUse = FACE_PHOEBE;

	if (ally == &enemy[FR_PHOEBE])
	{
		strcpy(name, "Phoebe");
		strcpy(otherName, "Ursula");
		kills = currentGame.wingMate1Kills;
		difference = currentGame.wingMate1Kills - currentGame.wingMate2Kills;
		if ((currentGame.wingMate1Kills > currentGame.wingMate2Kills) && (currentGame.wingMate1Kills > currentGame.totalKills))
			firstPlace = true;
		faceToUse = FACE_PHOEBE;
	}
	else
	{
		strcpy(name, "Ursula");
		strcpy(otherName, "Phoebe");
		kills = currentGame.wingMate2Kills;
		difference = currentGame.wingMate2Kills - currentGame.wingMate1Kills;
		if ((currentGame.wingMate2Kills > currentGame.wingMate1Kills) && (currentGame.wingMate2Kills > currentGame.totalKills))
			firstPlace = true;
		faceToUse = FACE_URSULA;
	}

	int r = rand() % 10;
	
	if (currentGame.hasWingMate2 == 0)
		r = rand() % 7;

	switch(r)
	{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			sprintf(in, killMessage[rand() % 6]);
			break;

		case 6:
		case 7:
			sprintf(in, killMessage[6], kills);
			break;

		case 8:
			if (difference > 0)
			{
				sprintf(in, killMessage[7], otherName);
			}
			else
			{
				sprintf(in, killMessage[8], otherName);
			}
			break;

		case 9:
			if (firstPlace)
			{
				sprintf(in, killMessage[9]);
			}
			else
			{
				sprintf(in, killMessage[rand() % 6]);
			}
			break;
	}

	setRadioMessage(faceToUse, in, 0);
}

const char *getKlineInsult()
{
	static const char insult[][40] = {
	"Pathetic", "How very disappointing...", "Heroic. And stupid", "Fool", "And now you're nothing but a DEAD hero"
	};

	if (currentGame.area != 26)
		return (insult[rand() % 3]);
	else
		return (insult[3 + (rand() % 2)]);
}

void getPlayerDeathMessage()
{
	if (enemy[WC_KLINE].active)
	{
		setRadioMessage(FACE_KLINE, getKlineInsult(), 1);
		return;
	}
	
	if ((enemy[WC_BOSS].active) && (enemy[WC_BOSS].classDef == CD_KRASS))
	{
		setRadioMessage(FACE_KRASS, "That was the easiest $90,000,000 I've ever earned! Bwwah!! Ha!! Ha!! Ha!!", 1);
		return;
	}

	int faceToUse = FACE_PHOEBE;
	

	if ((currentGame.area == 9) || (currentGame.area == 17) || (currentGame.area == 25)) {
		faceToUse = FACE_SID;
	}
	else if (currentGame.hasWingMate2)
	{
		if ((rand() % 2) == 0)
			faceToUse = FACE_URSULA;
	}

	setRadioMessage(faceToUse, deathMessage[rand() % 6], 1);
}

void getMissFireMessage(object *ally)
{
	char name[30], in[50];
	int faceToUse = FACE_PHOEBE;

	if (ally == &enemy[FR_PHOEBE])
	{
		strcpy(name, "Phoebe");
		faceToUse = FACE_PHOEBE;
	}
	else
	{
		strcpy(name, "Ursula");
		faceToUse = FACE_URSULA;
	}

	sprintf(in, missFireMessage[rand() % 5]);
	setRadioMessage(faceToUse, in, 0);
}

void getPlayerHitMessage(object *ally)
{
	char name[30], in[50];
	int faceToUse = FACE_PHOEBE;

	if (ally == &enemy[FR_PHOEBE])
	{
		strcpy(name, "Phoebe");
		faceToUse = FACE_PHOEBE;
	}
	else
	{
		strcpy(name, "Ursula");
		faceToUse = FACE_URSULA;
	}

	sprintf(in, playerHitMessage[rand() % 3]);
	setRadioMessage(faceToUse, in, 0);
}
