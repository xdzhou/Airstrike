#ifndef PLAYERS_H
#define PLAYERS_H

#include "sprite.h"
#include "control.h"
#include "teams.h"

#define MAXPLAYERS 128


//struct player;
typedef struct player
{
	int id;
	int id_in_team;
	int ishuman;
	int points;
	int damage;
	struct player * lastEnnemi;
	int keymap[MAX_CONTROLLER_KEYS][2];
	char name[32];
	sprite_t *sprite;
	sprite_type_t *sprite_type;
	controller_t *controller;
	int startpos[2];
	struct team *team;
	sprite_timer_t spawnTimer;
	int timeBeforeRespawn;
	int isConnected;
	int schrodinger;
} player_t;

extern player_t players[MAXPLAYERS];

void player_init();
void player_setai(int playerId);
void player_sethuman(int playerId);

#endif
