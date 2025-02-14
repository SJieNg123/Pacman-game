#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <string.h>
#include "game.h"
#include "shared.h"
#include "utility.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "pacman_obj.h"
#include "ghost.h"
#include "map.h"
#include "scene_settings.h"


// TODO-HACKATHON 2-0: Create one ghost
// Just modify the GHOST_NUM to 1
int GHOST_NUM;
// TODO-GC-ghost: create a least FOUR ghost!
/* global variables*/
extern const uint32_t GAME_TICK_CD;
extern uint32_t GAME_TICK;
extern ALLEGRO_TIMER* game_tick_timer;
int game_main_Score = 0;
bool game_over = false;
bool dead_animation_done = false;

/* Internal variables*/
ALLEGRO_TIMER* power_up_timer;
static const int power_up_duration = 10;
static Pacman* pman;
static Map* basic_map;
static Ghost** ghosts;
bool debug_mode = false;
bool cheat_mode = false;
bool game_pause = false;
bool ghost_back = false;
bool ctrl_pressed = false;
bool ghost_stop = false;
bool show_scoreboard = false;
bool animation = true;
bool input_name = false;

char player[20] = { "Me" };
int char_count = 0;

/* Declare static function prototypes */
static void init(void);
static void step(void);
static void checkItem(void);
static void status_update(void);
static void update(void);
static void draw(void);
static void printinfo(void);
static void destroy(void);
static void on_key_down(int key_code);
static void on_mouse_down(int btn, int x, int y, int dz);
static void render_init_screen(void);
static void draw_hitboxes(void);

static Score scores[5];

static void init(void) {
	game_over = false;
	game_main_Score = 0;
	// create map
	basic_map = create_map(NULL);
	// TODO-GC-read_txt: Create map from .txt file so that you can design your own map!!
	// basic_map = create_map("Assets/map_nthu.txt");
	if (!basic_map) {
		game_abort("error on creating map");
	}	
	// create pacman
	pman = pacman_create();
	if (!pman) {
		game_abort("error on creating pacamn\n");
	}
	
	// allocate ghost memory
	// TODO-HACKATHON 2-1: Allocate dynamic memory for ghosts array.
	if (ezmode)
		GHOST_NUM = 4;
	else
		GHOST_NUM = 8;

	ghosts = (Ghost**)malloc(sizeof(Ghost*) * GHOST_NUM);
	
	if(!ghosts){
		game_log("We haven't create any ghosts!\n");
	}
	else {
		// TODO-HACKATHON 2-2: create a ghost.
		// Try to look the definition of ghost_create in ghost.h and figure out what should be placed here.
		for (int i = 0; i < GHOST_NUM; i++) {
			game_log("creating ghost %d\n", i);
			ghosts[i] = ghost_create(i);
			if (!ghosts[i])
				game_abort("error creating ghost\n");
		}
	}
	GAME_TICK = 0;

	render_init_screen();
	power_up_timer = al_create_timer(1.0f); // 1 tick per second
	if (!power_up_timer)
		game_abort("Error on create timer\n");

	return;
}

static void step(void) {
	if (pman->objData.moveCD > 0)
		pman->objData.moveCD -= pman->speed;
	for (int i = 0; i < GHOST_NUM; i++) {
		// important for movement
		if (ghosts[i]->objData.moveCD > 0)
			ghosts[i]->objData.moveCD -= ghosts[i]->speed;
	}
}

static void checkItem(void) {
	int Grid_x = pman->objData.Coord.x, Grid_y = pman->objData.Coord.y;
	if (Grid_y >= basic_map->row_num - 1 || Grid_y <= 0 || Grid_x >= basic_map->col_num - 1 || Grid_x <= 0)
		return;
	// TODO-HACKATHON 1-3: check which item you are going to eat and use `pacman_eatItem` to deal with it.

	switch (basic_map->map[Grid_y][Grid_x])
	{
	case '.':
		//pacman_eatItem(...);
		pacman_eatItem(pman, '.');
		if (ezmode)
			game_main_Score += 1;
		else
			game_main_Score += 2;
		break;
		// ok
	case 'P':
		// TODO-GC-PB: ease power bean
		// pacman_eatItem(...);
		// stop and reset power_up_timer value
		// start the timer
		// dont do first
		pacman_eatItem(pman, 'P');
		for (int i = 0; i < GHOST_NUM; i++)
			ghosts[i]->status = FLEE;
		pman->powerUp = true;
		al_start_timer(power_up_timer);
		break;

	default:
		break;
	}

	// TODO-HACKATHON 1-4: erase the item you eat from map
	// Be careful, don't erase the wall block.
	if(basic_map->map[Grid_y][Grid_x] != '#')
		basic_map->map[Grid_y][Grid_x] = ' ';
}

static void status_update(void) {
	// TODO-PB: check powerUp duration
	/*
	if (pman->powerUp)
	{
		// Check the value of power_up_timer
		// If runs out of time reset all relevant variables and ghost's status
		// hint: ghost_toggle_FLEE
	}
	*/
	if (al_get_timer_count(power_up_timer) >= 10) {
		al_stop_timer(power_up_timer);
		pman->powerUp = false;

		for (int i = 0; i < GHOST_NUM; i++)
			if (ghosts[i]->status == FLEE)
				ghosts[i]->status = FREEDOM;

		al_set_timer_count(power_up_timer, 0);
	}
	// ok

	for (int i = 0; i < GHOST_NUM; i++) {
		if (ghosts[i]->status == GO_IN){
			continue;
		}
		else if (ghosts[i]->status == FREEDOM)
		{
			// TODO-GC-game_over: use `getDrawArea(..., GAME_TICK_CD)` and `RecAreaOverlap(..., GAME_TICK_CD)` functions to detect if pacman and ghosts collide with each other.
			// And perform corresponding operations.
			// [NOTE] You should have some if-else branch here if you want to implement power bean mode.
			// Uncomment Following Code
			/*
			if(!cheat_mode and collision of pacman and ghost)
			{
					game_log("collide with ghost\n");
					al_rest(1.0);
					pacman_die();
					game_over = true;
					break; // animation shouldn't be trigger twice.
			}
			*/

			RecArea pacmanArea = getDrawArea((object*)pman, GAME_TICK_CD);
			RecArea ghostArea = getDrawArea((object*)ghosts[i], GAME_TICK_CD);

			if (!cheat_mode && RecAreaOverlap(&pacmanArea, &ghostArea))
			{
				game_log("collide with ghost\n");
				al_rest(1.0);
				pacman_die();
				game_over = true;
				break; // animation shouldn't be trigger twice.
			}
		}
		else if (ghosts[i]->status == FLEE)
		{
			// TODO-GC-PB: if ghost is collided by pacman, it should go back to the cage immediately and come out after a period.
			/*
			if(!cheat_mode and collision of pacman and ghost)
			{
				ghost_collided(...)
			}
			*/
			RecArea pacmanArea = getDrawArea((object*)pman, GAME_TICK_CD);
			RecArea ghostArea = getDrawArea((object*)ghosts[i], GAME_TICK_CD);

			if (!cheat_mode && RecAreaOverlap(&pacmanArea, &ghostArea))
			{
				if (pman->powerUp && ghosts[i]->status == FLEE) {
					pacman_eatGhost();
					game_main_Score += 10;
					printf("eat ghost\n");
					ghosts[i]->status = GO_IN;
				}
			}
		}
	}
}

static void draw_gameover_status() {
	// Set up a rectangle for the mini window
	ALLEGRO_COLOR window_color = al_map_rgb(0, 0, 0);
	int window_w = SCREEN_W / 1.5;
	int window_h = SCREEN_H / 4;
	int window_x = (al_get_display_width(al_get_current_display()) - window_w) / 2;
	int window_y = (al_get_display_height(al_get_current_display()) - window_h) / 2;

	// Draw the mini window
	al_draw_filled_rectangle(window_x, window_y, window_x + window_w, window_y + window_h, window_color);

	// Set up font and draw the message

	int line_spacing = al_get_font_line_height(menuFont);

	al_draw_textf(
		menuFont,
		al_map_rgb(255, 255, 0),
		window_x + window_w / 2, window_y + window_h / 4,
		ALLEGRO_ALIGN_CENTER,
		"Your game score is %d", game_main_Score);

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 0),
		window_x + window_w / 2, window_y + window_h / 4 + line_spacing,
		ALLEGRO_ALIGN_CENTER,
		"Press <Z> to show scoreboard");

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 0),
		window_x + window_w / 2, window_y + window_h / 4 + 2 * line_spacing,
		ALLEGRO_ALIGN_CENTER,
		"Press <B> back to menu");
}
// done

static void update(void) {
	if (game_over) {
		// TODO-GC-game_over: start pman->death_anim_counter and schedule a game-over event (e.g change scene to menu) after Pacman's death animation finished
		// hint: refer al_get_timer_started(...), al_get_timer_count(...), al_stop_timer(...), al_rest(...)
		/*
			// start the timer if it hasn't been started.
			// check timer counter.
			// stop the timer if counter reach desired time.
			game_change_scene(...);
		*/
		if(!dead_animation_done){
			al_start_timer(pman->death_anim_counter);
			if (al_get_timer_count(pman->death_anim_counter) > 12) {
				al_stop_timer(pman->death_anim_counter);
				printf("death animation done\n");
				dead_animation_done = true;
			}
		}

		/*
		FILE* file;
		errno_t err = fopen_s(&file, "Assets/score.txt", "w");
		if (err == 0) {
			for (int i = 0; i < 5; i++) {
				fprintf(file, "%s, %d\n", scores[i].name, scores[i].score);
			}
			fclose(file); // Close the file only if it was successfully opened
		}
		else {
			perror("Error opening file");
			// Handle the error, such as by returning from the function
		}
		*/
		return;
	}

	if (!game_pause) {
		step();
		checkItem();
		status_update();
		pacman_move(pman, basic_map);

		if(!ghost_stop)
			for (int i = 0; i < GHOST_NUM; i++)
				ghosts[i]->move_script(ghosts[i], basic_map, pman);
	}
}

static void draw_pause_status() {
	// Set up a rectangle for the mini window
	int window_w = SCREEN_W / 1.5;
	int window_h = SCREEN_H / 4;
	int window_x = (al_get_display_width(al_get_current_display()) - window_w) / 2;
	int window_y = (al_get_display_height(al_get_current_display()) - window_h) / 2;

	// Draw the mini window
	al_draw_filled_rectangle(window_x, window_y, window_x + window_w, window_y + window_h, al_map_rgb(0, 0, 0));

	// Set up font and draw the message

	int line_spacing = al_get_font_line_height(menuFont);

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 0),
		window_x + window_w / 2, window_y + window_h / 4,
		ALLEGRO_ALIGN_CENTER,
		"Press <ESC> to Resume game");

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 0),
		window_x + window_w / 2, window_y + window_h / 4 + line_spacing,
		ALLEGRO_ALIGN_CENTER,
		"Press <B> back to menu");
}
// done

Score* open_scorefile() {
	FILE* file;
	errno_t err = fopen_s(&file, "Assets/score.txt", "r");
	if (err != 0 || file == NULL) {
		printf("Error opening score file\n");
		return NULL;
	}

	int i = 0;
	while (i < 5 && fscanf_s(file, "%49s %d", scores[i].name, (unsigned)_countof(scores[i].name), &scores[i].score) == 2) {
		i++;
	}
	fclose(file);
	return scores;
}

static void draw_scoreboard(Score* scores, float current_y) {
	for (int i = 0; i < 5; i++) {
		if (game_main_Score >= scores[i].score) {
			// Shift down all scores and names below this point
			for (int j = 4; j > i; j--) {
				scores[j].score = scores[j - 1].score;
				strncpy_s(scores[j].name, 20, scores[j - 1].name, _TRUNCATE);
				scores[j].name[19] = '\0';
			} // remember null 
			// Insert the new score and name

			scores[i].score = game_main_Score;
			strncpy_s(scores[i].name, 20, player, _TRUNCATE);
			scores[i].name[19] = '\0';
			break;
		}
	}

	ALLEGRO_COLOR window_color = al_map_rgb(0, 0, 0);
	int window_w = SCREEN_W / 1.5;
	int window_h = SCREEN_H / 3.5;
	int window_x = (al_get_display_width(al_get_current_display()) - window_w) / 2;
	int window_y = current_y;

	al_draw_filled_rectangle(window_x, window_y, window_x + window_w, window_y + window_h, window_color);

	int line_spacing = al_get_font_line_height(menuFont);
	int line_y = window_y + 10; // Starting Y position for text

	for (int i = 0; i < 5; ++i) {
		char text[100];
		snprintf(text, sizeof(text), "%d. %s %d", i + 1, scores[i].name, scores[i].score);
		al_draw_text(
			menuFont, 
			al_map_rgb(255, 255, 0),
			window_x + 10, 
			line_y,
			0,
			text);
		line_y += line_spacing;
	}

	al_draw_text(
	menuFont,
		al_map_rgb(255, 255, 0),
		window_x + 10,
		line_y,
		0,
		"press <Z> to return");
}

/*
static void showing_name() {
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 0), // Yellow color
		SCREEN_W / 2,
		SCREEN_H - 70,
		ALLEGRO_ALIGN_CENTER,
		"Key your name:"
	);

	if (al_get_next_event(event_queue, &ev) ){
		if (char_count < 19) {
			if (ev.keyboard.unichar >= ' ' && ev.keyboard.unichar <= '~') { // Basic printable ASCII
				player[char_count] = (char)ev.keyboard.unichar;
				char_count++;
				player[char_count] = '\0';
				// add null
			}
		}

		if (ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && char_count > 0) {
			char_count--;
			player[char_count] = '\0';
		}

		if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER || ev.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER) {
			// Enter key pressed, player name input finished
			input_name = true;
		}
	}

	// Draw the player's current input
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255), // White color for input text
		SCREEN_W / 2,
		SCREEN_H - 40, // Slightly below the prompt
		ALLEGRO_ALIGN_CENTER,
		player
	);

	al_flip_display();
}
*/

static void draw(void) {
	al_clear_to_color(al_map_rgb(0, 0, 0));
	// TODO-GC-scoring: Draw scoreboard, something your may need is sprinf();
	/*
		al_draw_text(...);
	*/

	al_draw_textf(
		largeFont,
		al_map_rgb(255, 255, 0),
		SCREEN_W / 2, 
		SCREEN_H - 70,
		ALLEGRO_ALIGN_CENTER,
		"%d", game_main_Score
	);
	// done

	draw_map(basic_map);

	pacman_draw(pman);
	if (game_over) {
		draw_gameover_status();
		if (show_scoreboard) {
			Score* scores = open_scorefile();
			float target_y = (SCREEN_H - (SCREEN_H / 3.5)) / 2; // Middle of the screen
			float current_y = SCREEN_H;
			while (animation && current_y > target_y) {
				current_y -= 12;
				draw_scoreboard(scores, current_y);
				al_flip_display();
				al_rest(0.05);
				if (current_y <= target_y){
					animation = false;
					printf("animation done\n");
					break;
				}
			}
			draw_scoreboard(scores, target_y);
		}
		return;
	}

	// no drawing below when game over
	for (int i = 0; i < GHOST_NUM; i++)
		ghost_draw(ghosts[i]);

	//debugging mode
	if (debug_mode) {
		draw_hitboxes();
	}

	if (game_pause) {
		draw_pause_status();
	}

	/*
	while (!input_name) {
		showing_name();
	}
	*/
}

static void draw_hitboxes(void) {
	RecArea pmanHB = getDrawArea((object *)pman, GAME_TICK_CD);
	al_draw_rectangle(
		pmanHB.x, pmanHB.y,
		pmanHB.x + pmanHB.w, pmanHB.y + pmanHB.h,
		al_map_rgb_f(1.0, 0.0, 0.0), 2
	);

	for (int i = 0; i < GHOST_NUM; i++) {
		RecArea ghostHB = getDrawArea((object *)ghosts[i], GAME_TICK_CD);
		al_draw_rectangle(
			ghostHB.x, ghostHB.y,
			ghostHB.x + ghostHB.w, ghostHB.y + ghostHB.h,
			al_map_rgb_f(1.0, 0.0, 0.0), 2
		);
	}

}

static void printinfo(void) {
	game_log("pacman:\n");
	game_log("coord: %d, %d\n", pman->objData.Coord.x, pman->objData.Coord.y);
	game_log("PreMove: %d\n", pman->objData.preMove);
	game_log("NextTryMove: %d\n", pman->objData.nextTryMove);
	game_log("Speed: %f\n", pman->speed);
}

static void destroy(void) {
	// TODO-GC-memory: free map array, Pacman and ghosts
	for (int i = 0; i < basic_map->row_num; i++) {
		free(basic_map->map[i]);
	}
	free(basic_map->map);
	free(basic_map);

	pacman_destroy(pman);

	for (int i = 0; i < GHOST_NUM; i++) {
		ghost_destory(ghosts[i]);
	}

	free(ghosts);
	al_destroy_timer(power_up_timer);
}

static void on_key_down(int key_code) {
	switch (key_code){
		// TODO-HACKATHON 1-1: Use allegro pre-defined enum ALLEGRO_KEY_<KEYNAME> to controll pacman movement
		// we provided you a function `pacman_NextMove` to set the pacman's next move direction.
		
		case ALLEGRO_KEY_W:
			pacman_NextMove(pman, UP);
			break;
		case ALLEGRO_KEY_A:
			pacman_NextMove(pman, LEFT);
			break;
		case ALLEGRO_KEY_S:
			if (cheat_mode && ctrl_pressed) {
				printf("ghost stop moving\n");
				ghost_stop = !ghost_stop;
				ctrl_pressed = false;
			}
			else
				pacman_NextMove(pman, DOWN);
			break;
		case ALLEGRO_KEY_D:
			pacman_NextMove(pman, RIGHT);
			break;
		case ALLEGRO_KEY_C:
			cheat_mode = !cheat_mode;
			if (cheat_mode)
				printf("cheat mode on\n");
			else 
				printf("cheat mode off\n");
			break;
		case ALLEGRO_KEY_G:
			debug_mode = !debug_mode;
			break;

		//new add, press esc to pause/resume the game 
		case ALLEGRO_KEY_ESCAPE:
			game_pause = !game_pause;
			if (game_pause)
				printf("game is pause\n");
			else
				printf("game is resume\n");
			break;

		// new add, press b to return to menu 
		case ALLEGRO_KEY_B:
			game_change_scene(scene_menu_create());
			game_pause = false;
			dead_animation_done = false;
			printf("back to menu\n");
			break;

		// in cheat mode, ghost will back to room when k is pressed
		case ALLEGRO_KEY_K:
			ghost_back = !ghost_back;
			for (int i = 0; i < GHOST_NUM; i++)
				ghosts[i]->status = GO_IN;
			break;

		case (ALLEGRO_KEY_LCTRL):
			printf("ctrl pressed\n");
			ctrl_pressed = true;
			break;

		case (ALLEGRO_KEY_RCTRL):
			printf("ctrl pressed\n");
			ctrl_pressed = true;
			break;

		case (ALLEGRO_KEY_L):
			if (cheat_mode && ctrl_pressed) {
				printf("pacman can cross wall\n");
				pacman_crosswall = !pacman_crosswall;
				ctrl_pressed = false;
			}
			break;

		// show scoreboard
		case (ALLEGRO_KEY_Z):
			show_scoreboard = !show_scoreboard;
			if (show_scoreboard) {
				animation = true;
				printf("showing scoreboard\n");
			}
			else
				printf("not showing scoreboard\n");
			break;
	
	default:
		break;
	}
}

static void on_mouse_down(int btn, int x, int y, int dz) {

	// nothing here

}

static void render_init_screen(void) {
	al_clear_to_color(al_map_rgb(0, 0, 0));

	draw_map(basic_map);
	pacman_draw(pman);
	for (int i = 0; i < GHOST_NUM; i++) {
		ghost_draw(ghosts[i]);
	}

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 0),
		400, 400,
		ALLEGRO_ALIGN_CENTER,
		"READY!"
	);

	al_flip_display();
	al_rest(2.0);

}
// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

// The only function that is shared across files.
Scene scene_main_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Start";
	scene.initialize = &init;
	scene.update = &update;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_down = &on_mouse_down;
	// TODO-IF: Register more event callback functions such as keyboard, mouse, ...
	game_log("Start scene created");
	return scene;
}