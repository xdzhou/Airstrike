#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "airstrike.h"
#include "sprite_types.h"
#include "config.h"
#include "text.h"
#include "sound.h"
#include "console.h"
#include "players.h"
#include "mech.h"
#include "level.h"
#include "winds.h"
#include "math.h"
#include "prototype.h"
#include "network.h"
#include "teams.h"
#include <unistd.h>
#include "SDL/SDL_mixer.h"

sprite_group_t *mech_group;
sprite_group_t *bullet_group;
sprite_group_t *bomb_group;
sprite_group_t *effects_group;
sprite_group_t *foreground_group;
sprite_group_t *ui_group;
sprite_group_t *ui_group_connect;

void init_spawn_delays();

static int paused = 0;
static int max_points;
static Uint32 displayflags = 0;
static int show_debug = 1; /* If true print dbg info on screen */
static Uint32 frame_times[32];
static int frame_time_i = 0;
static int screen_w;
static int screen_h;

static int nbTeams;
static int nbPlayers; //TODO: eliminate nbPlayers or player count, redundant

static int timeToStop;
static int planeCollision=1;

static void atexit_cleanup(void)
{
	/* Since we don't want a crash in fullscreen mode
	 we hope that this will bring us back to a window */
	SDL_SetVideoMode(screen_w, screen_h, 0, 0);
}

static int general_setup(void)
{
	int i;

	maths_setup();
	config_setup();

	screen_w = cfgnum("screen.width", 800);
	screen_h = cfgnum("screen.height", 600);
	if (cfgnum("fullscreen", 0) == 1)
		displayflags |= SDL_FULLSCREEN;

	sprite_global.opt_alpha = cfgnum("alpha", 0);

	i = SDL_Init(SDL_INITFLAGS);
	assert(i == 0);

	atexit(atexit_cleanup);

	SDL_WM_SetCaption(VERSIONSTRING, 0);

	sprite_global.display = SDL_SetVideoMode(screen_w, screen_h, 0, displayflags);
	assert(sprite_global.display);
	SDL_ShowCursor(SDL_DISABLE);

	if (cfgnum("sound", 0) == 1)
	{
		sound_setup();
		/* place the ear on the ground, in the center */
		sound_move_ear(screen_w / 2, screen_h);
	}
	sprite_setup();
	text_setup();
	console_setup();
	console_set_pos(9, 254);
	console_load_bg(path_to_data("console-bg.png"));
	sprite_types_setup();

	char bg1[255];
	char bg2[255];
	sprintf(bg1,"data/bg%d.png",screen_w);
	sprintf(bg2,"data/bgmask%d.png",screen_w);

	sprite_background_load(bg1, bg2);

	level_setup();
	winds_setup();

	return 0;
}

void players_setup(void)
{
	sprite_t *sp;
	playerCount = nbPlayers;
	player_init();
	//assert((playerCount >= 0) && (playerCount <= MAXPLAYERS)); This is now tested at startup
	int remains = nbPlayers % nbTeams;
	int perTeam = nbPlayers/nbTeams;
	int nbMembers = 0;
	int team = 0;
	int i;
	for (i = 0; i < playerCount; i++)
	{
		players[i].sprite_type = &blueplane;
		players[i].points = 0;
		player_sethuman(i);
		players[i].team = &teams[team];
		players[i].id_in_team=nbMembers;
		players[i].startpos[0] = screen_w-200;
		players[i].startpos[1] = (players[i].team->id+1)*90;
		nbMembers++;
		//printf("Player %d is in team %d, %d\n", i, team, players[i].team);

		if ((nbMembers == perTeam && team > (remains-1)) || nbMembers > perTeam)
		{
			team++;
			nbMembers=0;
		}

	}
}

void teams_setup(void)
{
	int i;
	teamCount = nbTeams;
	teams_init(nbTeams, nbPlayers);
	max_points=cfgnum("game.teamWinScore",400);
}

void engine_setup(void)
{
	mech_group = sprite_group_create();
	bomb_group = sprite_group_create();
	bullet_group = sprite_group_create();
	effects_group = sprite_group_create();
	foreground_group = sprite_group_create();
	ui_group = sprite_group_create();
	ui_group_connect = sprite_group_create();
}

void objects_setup(void)
{
	int i, x, y;
	sprite_t *sp;
	float v[2];

	srand(23);

	sprite_t *s;

	for (i=0;i<nbTeams;i++){
		s = sprite_create(&teamicon,&teams[i]);
		sprite_set_pos(s,75+100*(i), sprite_global.display->h - 75);
		sprite_group_insert(ui_group, s);
	}

	for (i=0;i<nbTeams;i++){

		int x = 190+300*(i%5);
		int y = 80;
		if (i>4)
			y+=400;

		s = sprite_create(&teamicon,&teams[i]);
		sprite_set_pos(s,x, y);
		sprite_group_insert(ui_group_connect, s);
	}


	/*sp = sprite_create(&bonusmachine);
	 sprite_group_insert(mech_group,sp);
	 sprite_set_pos(sp,level.xmax/2,15);*/

	/*for (i=0;i<3;i++)
	 {
	 sp = sprite_create(&bird);
	 sprite_group_insert(mech_group,sp);
	 }*/

	/*for (i=0;i<5;i++)
	 {
	 sp = sprite_create(&bouncer);
	 sprite_group_insert(mech_group,sp);
	 sprite_set_pos(sp,rand() % 700 + 50,rand() % 200 + 100);
	 }*/

	/*sp = sprite_create(&cloud);
	 sprite_group_insert(foreground_group,sp);
	 sprite_set_pos(sp,400,70);
	 v[0] = 15; v[1] = 0;
	 sprite_set_vel(sp,v);*/

	/*sp = sprite_create(&cloud);
	 sprite_group_insert(foreground_group,sp);
	 sprite_set_pos(sp,0,150);
	 v[0] = 10; v[1] = 0;
	 sprite_set_vel(sp,v);*/

	/*sp = sprite_create(&zeppelin);
	 sprite_group_insert(mech_group,sp);
	 sprite_set_pos(sp,50,200);*/

	/*sp = sprite_create(&airballoon);
	 sprite_group_insert(mech_group,sp);
	 sprite_set_pos(sp,100,150);
	 v[0] = 5; v[1] = 3;
	 sprite_set_vel(sp,v);*/

	/*sp = sprite_create(&cannon);
	 sprite_group_insert(mech_group,sp);
	 sprite_set_pos(sp,380,518);*/
}

static int process_events(void)
{

	SDL_Event event;

	int ret = 1;
	while (SDL_PollEvent(&event) >= 1)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			ret = 0;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				paused = 1 - paused;
				break;
			case SDLK_s:
				SDL_SaveBMP(sprite_global.display,"screenshot.bmp");
				break;
			case SDLK_RETURN:
				if (!inGame){
					inGame = 1-inGame;
					init_spawn_delays();
					timeToStop=sprite_global.game_clock+1000*cfgnum("game.max_duration",60);
				}
				break;
			default:
				break;
			}
			break;
			default:
				break;
		}
	}
	return ret;
}

static int wait_for_key(void)
{
	SDL_Event event;
	while (1)
	{
		while (SDL_PollEvent(&event) >= 1)
		{
			switch (event.type)
			{
			case SDL_QUIT:
				exit(EXIT_SUCCESS);
				break;
			case SDL_KEYDOWN:
				return event.key.keysym.sym;
				break;
			default:
				break;
			}
		}
		SDL_Delay(sprite_global.display_dt);
	}
}

void draw_ui(void)
{
	int i;
	char cbuf[256];
	float av_dt, s_dt;
	if(inGame){

		for(i=0;i<nbTeams;i++){
			sprintf(cbuf, "%2.2i", teams[i].points);
			text_render(sprite_global.display, 0, big_font, 60+100*(i), sprite_global.display->h - 90, ALIGN_LEFT, ALIGN_BOTTOM, cbuf);
		}

		for(i=0;i<nbPlayers;i++){
			if(players[i].sprite){
				sprintf(cbuf, "%s", players[i].name);
				text_render(sprite_global.display, 0, medium_font,  players[i].sprite->x, players[i].sprite->y-20, ALIGN_LEFT, ALIGN_BOTTOM, cbuf);
			}
		}
		int t = (timeToStop-sprite_global.game_clock)/1000;
		if(t>900){t=0;}
		sprintf(cbuf, "%d", t);
		text_render(sprite_global.display, 0, big_font,  sprite_global.display->w-100, sprite_global.display->h - 90, ALIGN_LEFT, ALIGN_BOTTOM, cbuf);

	}else{
		for (i = 0; i < playerCount; i++)
		{
			int x = 100+300*(players[i].team->id%5);
			int y = 100+30*players[i].id_in_team;
			if (players[i].team->id>4)
				y+=400;

			if (players[i].isConnected){
				sprintf(cbuf, "[%d]-%s",i,players[i].name);
			}else{
				sprintf(cbuf, "[%d]-_______", i);
			}
			text_render(sprite_global.display, 0, big_font, x,y, ALIGN_LEFT, ALIGN_TOP, cbuf);
		}
	}

	if (show_debug)
	{
		/* calculate frame time stats */
		av_dt = 0;
		s_dt = 0;
		for (i = 0; i < 32; i++)
			av_dt += frame_times[i];
		av_dt /= 32;
		fps=1000/av_dt;
		for (i = 0; i < 32; i++)
			s_dt += (frame_times[i] - av_dt) * (frame_times[i] - av_dt);
		s_dt = sqrt(s_dt / 32);
		sprintf(cbuf, "average dt = %3.2f (stddev = %3.2f)", av_dt, s_dt);
		text_render(sprite_global.display, 0, small_font, 10, 10, ALIGN_LEFT, ALIGN_TOP, cbuf);
	}
}

/* This draws things when we are in console (paused) mode */
void console_frame(void)
{
	sprite_start_frame();
	sprite_group_draw2(mech_group);
	sprite_group_draw(bullet_group);
	sprite_group_draw(bomb_group);
	sprite_group_draw(effects_group);
	sprite_group_draw(foreground_group);
	if (inGame){
		sprite_group_draw(ui_group);
	}else{
		sprite_group_draw(ui_group_connect);
	}

	winds_change();

	draw_ui();
	console_draw();
	sprite_end_frame();
}

/* choices is null terminated. Return the
 index of the choosen item, or -1 if escape was
 pressed.*/
int select_mode(char *header, char *choices[])
{
	int i, key, choice = 0;
	while (1)
	{
		console_clear();
		console_write(header);
		console_write("\n");
		for (i = 0; choices[i]; i++)
		{
			if (choice == i)
				console_write("*");
			else
				console_write(" ");
			console_write(choices[i]);
			console_write("\n");
		}
		console_frame();
		key = wait_for_key();
		switch (key)
		{
		case SDLK_UP:
			choice--;
			if (choice < 0)
			{
				/* seek last item */
				choice = 0;
				while (choices[choice + 1] != 0)
					choice++;
			}
			break;
		case SDLK_DOWN:
			choice++;
			if (choices[choice] == 0)
				choice = 0;
			break;
		case SDLK_ESCAPE:
			return -1;
		default:
			return choice;
		}
	}
}

//void player_keys_mode(int p)
//{
//	int i;
//	int key;
//	char cbuf[2] =
//	{ 0, 0 };
//	again: console_clear();
//	console_write("Setting up key bindings,\nhit the appropriate keys.\n");
//	i = 0;
//	while (control_key_names[i].signal)
//	{
//		console_write(control_key_names[i].name);
//		console_write(" [");
//		console_write(SDL_GetKeyName(players[p].keymap[i][0]));
//		console_write("] ");
//		console_frame();
//		key = wait_for_key();
//		console_write(SDL_GetKeyName(key));
//		console_write("\n");
//		players[p].keymap[i][0] = key;
//		players[p].keymap[i][1] = control_key_names[i].signal;
//		i++;
//	}
//	console_write("Done. Press any key to continue.\n");
//	console_frame();
//	key = wait_for_key();
//}

//void player_setup_mode(void)
//{
//	char *items[] =
//	{ 0, 0, "Left player keys", "Right player keys", "Go back", 0 };
//	while (1)
//	{
//		if (players[1].ishuman)
//			items[0] = "Left Player: Keyboard";
//		else
//			items[0] = "Left Player: AI";
//		if (players[0].ishuman)
//			items[1] = "Right Player: Keyboard";
//		else
//			items[1] = "Right Player: AI";
//
//		switch (select_mode("Player setup", items))
//		{
//		case 0:
//			players[1].ishuman = 1 - players[1].ishuman;
//			if (players[1].ishuman)
//				player_sethuman(1);
//			else
//				player_setai(1);
//			break;
//		case 1:
//			players[0].ishuman = 1 - players[0].ishuman;
//			if (players[0].ishuman)
//				player_sethuman(0);
//			else
//				player_setai(0);
//			break;
//		case 2:
//			player_keys_mode(1);
//			break;
//		case 3:
//			player_keys_mode(0);
//			break;
//		default:
//			return;
//		}
//	}
//}

/* Display the message and wait for a keypress */
void message_mode(char *message)
{
	sprite_start_frame();
	sprite_group_draw2(mech_group);
	sprite_group_draw(bullet_group);
	sprite_group_draw(bomb_group);
	sprite_group_draw(effects_group);
	sprite_group_draw(foreground_group);
	sprite_group_draw(ui_group);
	text_render(sprite_global.display, 0, big_font, sprite_global.display->w / 2, sprite_global.display->h / 2, ALIGN_CENTER, ALIGN_CENTER, message);
	sprite_end_frame();
	sprite_dirty_all();
	wait_for_key();
}

void message_time(char *message,int duration)
{
	sprite_start_frame();
	sprite_group_draw2(mech_group);
	sprite_group_draw(bomb_group);
	sprite_group_draw(bullet_group);
	sprite_group_draw(effects_group);
	sprite_group_draw(foreground_group);
	sprite_group_draw(ui_group);
	text_render(sprite_global.display, 0, big_font, sprite_global.display->w / 2, sprite_global.display->h / 2, ALIGN_CENTER, ALIGN_CENTER, message);
	draw_ui();
	sprite_end_frame();
	sprite_dirty_all();
	sleep(duration);
}

void about_mode(char *filename)
{
	FILE *message = fopen(path_to_data(filename), "r");
	char cbuf[2001];
	size_t n;

	if (message)
	{
		n = fread(cbuf, 1, 2000, message);
		cbuf[n] = 0;
		fclose(message);
	}
	else
	{
		sprintf(cbuf, "Error: could not open '%s'.", filename);
	}
	message_mode(cbuf);
}

void console_mode()
{
	char *items[] =
	{ "Resume game", "Toggle fullscreen", "Toggle debug mode", "Quit game", 0 };
	while (1)
	{
		switch (select_mode("Airstrike menu", items))
		{
		case 0:
			return;
			break;
		case 1:
			displayflags ^= SDL_FULLSCREEN;
			sprite_global.display = SDL_SetVideoMode(screen_w, screen_h, 0, displayflags);
			assert(sprite_global.display);
			sprite_dirty_all();
			sprite_bg_dirty_all();
			break;
		case 2:
			show_debug = 1 - show_debug;
			break;
		case 3:
			netStop=1;
			//wait for the network thread to stop
			while (netStopped!=1){
			}
			exit(EXIT_SUCCESS);
			break;
		default:
			return;
			break;
		}
	}
}

int startDelay[MAXTEAMS];
void init_spawn_delays()
{

	int timefirstspawn=1000*cfgnum("game.firstSpawn",3);
	int delaySpawn=1000*cfgnum("game.delaySpawn",2);

	int a;
	for (a = 0; a< nbTeams; a++){
		startDelay[a]=timefirstspawn;
	}
	for (a = 0; a < playerCount; a++)
	{
		int tid = players[a].team->id;
		players[a].timeBeforeRespawn=startDelay[tid];
		if(players[a].isConnected){
			startDelay[tid]+=delaySpawn;
		}
		printf("Player %d will start after %d\n",a,players[a].timeBeforeRespawn);
	}

}


/* Keeps track of dead planes, recreates them and award points */

void scorekeeper()
{
	char cbuf[500];
	char cbuf2[40];
	sprite_t *s;
	int i;
	update_teams_score(nbTeams,nbPlayers);

	int endOfGame=0;
	int maxscore = 0;
	/** une équipe atteint le score max**/
	for (i = 0; i < nbTeams; i++){
		if (teams[i].points>=max_points)
			endOfGame=1;
		if (teams[i].points>maxscore)
			maxscore=teams[i].points;
	}
	/** le temps est écoulé **/
	if (sprite_global.game_clock>timeToStop)
		endOfGame=1;

	if(endOfGame){
		sprintf(cbuf,"Les gagnants sont :\n");
		for (i = 0; i < nbTeams; i++){
			if(teams[i].points==maxscore){
				/*<string-array name="colors">
			        <item>vert Clair</item>
			        <item>Rouge</item>
			        <item>Bleu</item>
			        <item>Jaune</item>
			        <item>Turquoise</item>
			        <item>Violet</item>
			        <item>Orange</item>
			        <item>Gris</item>
			        <item>Vert Foncé</item>
			        <item>Bleu Foncé</item>
			    </string-array>*/

				switch (i){
				case 0:
					sprintf(cbuf2,"     Les Verts Clairs \n");
					break;
				case 1:
					sprintf(cbuf2,"     Les Rouges \n");
					break;
				case 2:
					sprintf(cbuf2,"     Les Bleus \n");
					break;
				case 3:
					sprintf(cbuf2,"     Les Jaunes \n");
					break;
				case 4:
					sprintf(cbuf2,"     Les Turquoises \n");
					break;
				case 5:
					sprintf(cbuf2,"     Les Violets \n");
					break;
				case 6:
					sprintf(cbuf2,"     Les Oranges \n");
					break;
				case 7:
					sprintf(cbuf2,"     Les Gris \n");
					break;
				case 8:
					sprintf(cbuf2,"     Les Verts Foncés \n");
					break;
				case 9:
					sprintf(cbuf2,"     Les Bleus Foncés \n");
					break;
				default:
					break;
				}

				//sprintf(cbuf2,"     Team %d\n",i);
				strcat(cbuf,cbuf2);
			}
		}
		sprintf(cbuf2,"\n\n %d points\n",maxscore);
		strcat(cbuf,cbuf2);
		message_time(cbuf,10);
		message_time("Starting in 3",1);
		message_time("Starting in 2",1);
		message_time("Starting in 1",1);
		message_time("GO !",1);

		init_spawn_delays();
		int j;
		for (j = 0; j< nbTeams; j++){
			teams[j].points=0;
		}

		for (j = 0; j < playerCount; j++){
			players[j].points=0;
			sprite_kill(players[j].sprite);
		}
		timeToStop=sprite_global.game_clock+1000*cfgnum("game.max_duration",60);
		return;

	}



	for (i = 0; i < playerCount; i++)
	{
		if (!sprite_isvalid(&(players[i].controller->target)))
		{
			if (!players[i].spawnTimer){
				sprite_release(&(players[i].sprite));
				sprite_timer_set(&(players[i].spawnTimer),players[i].timeBeforeRespawn);
				printf("timer set player %d %d\n", i ,players[i].timeBeforeRespawn );

			}else{
				if (sprite_timer_finished(players[i].spawnTimer)){
					s = sprite_create(players[i].sprite_type,&players[i]);
					sprite_aquire(s, &(players[i].sprite));
					sprite_aquire(players[i].sprite, &(players[i].controller->target));
					assert(sprite_isvalid(&(players[i].controller->target)));
					s = players[i].sprite;
					sprite_group_insert(mech_group, s);
					sprite_set_pos(s, players[i].startpos[0], players[i].startpos[1]);

					players[i].spawnTimer=NULL;
				}
			}
		}
	}
}

/* Creates a new game frame */
void game_frame()
{
	//usleep(100000);
	int i;
	/* collect frame time statistics */
	static Uint32 lasttime = 0;
	Uint32 now = SDL_GetTicks();
	frame_times[frame_time_i] = now - lasttime;
	frame_time_i = (frame_time_i + 1) & 31;
	lasttime = now;

	/*  sprite_viewport_center_on(player_sprite[0]);*/


	sprite_start_frame();

	sprite_group_move(mech_group, sprite_global.dt);
	sprite_group_move(bullet_group, sprite_global.dt);
	sprite_group_move(bomb_group, sprite_global.dt);
	sprite_group_move(effects_group, sprite_global.dt);
	sprite_group_move(foreground_group, sprite_global.dt);

	sprite_group_animate(mech_group, sprite_global.dt);
	sprite_group_animate(bullet_group, sprite_global.dt);
	sprite_group_animate(bomb_group, sprite_global.dt);
	sprite_group_animate(effects_group, sprite_global.dt);
	sprite_group_animate(foreground_group, sprite_global.dt);

	mech_gravity(mech_group); /* has to be before bg_coll */
	mech_gravity(bomb_group); /* has to be before bg_coll */

	if(planeCollision){
		sprite_group_coll(mech_group, mech_sprite_collide);
	}
	sprite_group_coll2(bullet_group, mech_group, 0);
	sprite_group_coll2(bomb_group, mech_group, 0);
	sprite_group_coll2(bomb_group, bullet_group, 0);
	sprite_group_coll2(bomb_group, bomb_group, 0);
	sprite_group_bg_coll(mech_group, mech_sprite_bg_collide);
	sprite_group_bg_coll(bullet_group, 0);
	sprite_group_bg_coll(bomb_group, 0);


	for (i = 0; i < playerCount; i++)
	{
		players[i].controller->update(players[i].controller);
	}

	sprite_group_update(mech_group);
	sprite_group_update(bullet_group);
	sprite_group_update(bomb_group);
	sprite_group_update(effects_group);
	sprite_group_update(foreground_group);
	sprite_group_update(ui_group);

	sprite_group_cleanup(bullet_group);
	sprite_group_cleanup(bomb_group);
	sprite_group_cleanup(mech_group);
	sprite_group_cleanup(effects_group);
	sprite_group_cleanup(foreground_group);
	sprite_group_cleanup(ui_group);

	/* Refresh sprite positions to reflect changes
	 by collisions */

	sprite_group_pos_update(mech_group);


	sprite_group_draw(effects_group);
	sprite_group_draw2(mech_group);
	sprite_group_draw(bullet_group);
	sprite_group_draw(bomb_group);
	sprite_group_draw(foreground_group);
	sprite_group_draw(ui_group);



	draw_ui();

	if (!sprite_end_frame())
	{
		//printf("lag");
		//fflush(stdout);
	}

	scorekeeper();

}

/* Dirty function to save an animation of the game */
void saveanimframe()
{
	static int n = 0;
	static SDL_Surface *img = 0;
	SDL_Rect r;
	char filename[100];
	r.x = 325;
	r.y = 458;
	r.w = 256;
	r.h = 96;
	if (!img)
	{
		img = SDL_CreateRGBSurface(SDL_SWSURFACE, r.w, r.h, 8, 0, 0, 0, 0);
		img = SDL_DisplayFormat(img);
	}
	SDL_BlitSurface(sprite_global.display, &r, img, 0);
	sprintf(filename, "%05i.bmp", n++);
	SDL_SaveBMP(img,filename);
}

void connect_frame(){
	//usleep(100000);
	int i;
	/* collect frame time statistics */
	static Uint32 lasttime = 0;
	Uint32 now = SDL_GetTicks();
	frame_times[frame_time_i] = now - lasttime;
	frame_time_i = (frame_time_i + 1) & 31;
	lasttime = now;

	sprite_start_frame();
	sprite_group_draw(ui_group_connect);
	draw_ui();

	if (!sprite_end_frame())
	{
		//printf("lag");
		//fflush(stdout);
	}
}

int main(int argc, char *argv[])
{
	int res;

	if (argc == 6)
	{
		nbTeams = (int) strtol(argv[1], &argv[1], 10);
		//TODO : mettre des #define pour equipe min et max
		if (nbTeams > 10 || nbTeams < 2)
		{
			printf("Please enter a number of teams between 2 and %d\n", 10); //TODO : wtf ? 50% string 50% %d ?
			exit(EXIT_SUCCESS);
		}

		nbPlayers = (int) strtol(argv[2], &argv[2], 10);
		if (nbPlayers > MAXPLAYERS || nbPlayers < 2)
		{
			printf("Please enter a number of players between 2 and %d\n", MAXPLAYERS);
			exit(EXIT_SUCCESS);
		}
		//tell photon server the ip adress
		NotifyPhotonServer(argv[5]);

		networkLoad = (int) strtol(argv[3], &argv[3], 10);
		networkLoadinterval = (int) strtol(argv[4], &argv[4], 10);
		printf("%d teams, %d players\n", nbTeams, nbPlayers);

	}
	else
	{
		printf("Airstrike nbOfTeams nbOfPlayers NetworkLoad(Bytes) networkLoadinterval(ms) IPadressOfPhoton:Port\n");
		exit(EXIT_SUCCESS);
	}

	res = general_setup() == 0;
	assert(res);
	engine_setup();
	DEBUGPOINT(1);
	teams_setup();
	DEBUGPOINT(42);
	players_setup();
	objects_setup();

	planeCollision=cfgnum("game.planeCollision",1);
	networkport = cfgnum("network.port",1234);

	network_init();


	Mix_Music *play_sound = NULL;
	play_sound = Mix_LoadMUS("data/sound/Xcyril-La_decouverte_et_la_conquete.wav");
	Mix_PlayMusic(play_sound, -1);
	//Mix_VolumeMusic(MIX_MAX_VOLUME/2);

	fprintf(stderr, "Entering main loop.\n");
	while (process_events())
	{
		if (!paused)
		{
			if(inGame){
				game_frame();
			}else{
				connect_frame();
			}
		}
		else
		{
			console_mode();
			paused = 0;
		}
	}
	// We probably never get here right now, because of exit() calls. 
	fprintf(stderr, "Left main loop.\n");
	sprite_group_free(mech_group);
	sprite_group_free(bomb_group);
	sprite_group_free(bullet_group);
	sprite_group_free(effects_group);
	sprite_group_free(foreground_group);
	return EXIT_SUCCESS;
}
