// TODO-HACKATHON 3-9: Create scene_settings.h and scene_settings.c.
// No need to do anything for this part. We've already done it for
// you, so this 2 files is like the default scene template.
#include "scene_settings.h"
#include <stdlib.h>
#include <string.h>
// #include <cstdio>
// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/* Define your static vars / function prototypes below. */

// TODO-IF: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.

static void draw(void) {
	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2,
		SCREEN_H - 500,
		ALLEGRO_ALIGN_CENTER,
		"PRESS <1> To change bgm"
	);

	if (oriBGM) {
		al_draw_text(
			menuFont,
			al_map_rgb(255, 255, 0),
			SCREEN_W / 2 - 70,
			SCREEN_H - 450,
			ALLEGRO_ALIGN_CENTER,
			"BGM 1"
		);

		al_draw_text(
			menuFont,
			al_map_rgb(255, 255, 255),
			SCREEN_W / 2 + 70,
			SCREEN_H - 450,
			ALLEGRO_ALIGN_CENTER,
			"BGM 2"
		);
	}

	else {
		al_draw_text(
			menuFont,
			al_map_rgb(255, 255, 255),
			SCREEN_W / 2 - 70,
			SCREEN_H - 450,
			ALLEGRO_ALIGN_CENTER,
			"BGM 1"
		);

		al_draw_text(
			menuFont,
			al_map_rgb(255, 255, 0),
			SCREEN_W / 2 + 70,
			SCREEN_H - 450,
			ALLEGRO_ALIGN_CENTER,
			"BGM 2"
		);
	}

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2,
		SCREEN_H - 400,
		ALLEGRO_ALIGN_CENTER,
		"PRESS <2> To mute bgm"
	);

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2,
		SCREEN_H - 350,
		ALLEGRO_ALIGN_CENTER,
		"BGM"
	);

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 0),
		SCREEN_W / 2 - 63,
		SCREEN_H - 350,
		ALLEGRO_ALIGN_CENTER,
		"<   >"
	);

	if (bgmMute)
		al_draw_text(
			menuFont,
			al_map_rgb(255, 255, 255),
			SCREEN_W / 2 - 62,
			SCREEN_H - 350,
			ALLEGRO_ALIGN_CENTER,
			"X"
		);

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2,
		SCREEN_H - 300,
		ALLEGRO_ALIGN_CENTER,
		"PRESS <3> To change gamemode"
	);

	if(ezmode){
		al_draw_text(
			menuFont,
			al_map_rgb(255, 255, 0),
			SCREEN_W / 2 - 30,
			SCREEN_H - 250,
			ALLEGRO_ALIGN_RIGHT,
			"EZMODE"
		);

		al_draw_text(
			menuFont,
			al_map_rgb(255, 255, 255),
			SCREEN_W / 2 + 30,
			SCREEN_H - 250,
			ALLEGRO_ALIGN_LEFT,
			"HARDMODE"
		);
	}

	else {
		al_draw_text(
			menuFont,
			al_map_rgb(255, 255, 255),
			SCREEN_W / 2 - 30,
			SCREEN_H - 250,
			ALLEGRO_ALIGN_RIGHT,
			"EZMODE"
		);

		al_draw_text(
			menuFont,
			al_map_rgb(255, 255, 0),
			SCREEN_W / 2 + 30,
			SCREEN_H - 250,
			ALLEGRO_ALIGN_LEFT,
			"HARDMODE"
		);
	}

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W/2,
		SCREEN_H - 200,
		ALLEGRO_ALIGN_CENTER,
		"PRESS <4> Back to menu"
	);

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2,
		SCREEN_H - 150,
		ALLEGRO_ALIGN_CENTER,
		"PRESS <5> to Exit game"
	);
}

static void on_key_down(int keycode) {
	switch (keycode) {
	case ALLEGRO_KEY_1:
		oriBGM = !oriBGM;
		printf("bgm changed\n");
		break;

	case ALLEGRO_KEY_2:
		bgmMute = !bgmMute;
		if(bgmMute)
			printf("bgm muted\n");
		break;

	case ALLEGRO_KEY_3:
		ezmode = !ezmode;
		if (!ezmode)
			printf("hardmode\n");
		break;

	case ALLEGRO_KEY_4:
		game_change_scene(scene_menu_create());
		break;

	case ALLEGRO_KEY_5:
		gameDone = true;
		break;

		default:
			break;
	}
}

// The only function that is shared across files.
Scene scene_settings_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Settings";
	scene.draw = &draw;
	scene.on_key_down = &on_key_down;
	// TODO-IF: Register more event callback functions such as keyboard, mouse, ...
	game_log("Settings scene created");
	return scene;
}

// TODO-Graphical_Widget:
// Just suggestions, you can create your own usage.
	// Selecting BGM:
	// 1. Declare global variables for storing the BGM. (see `shared.h`, `shared.c`)
	// 2. Load the BGM in `shared.c`.
	// 3. Create dropdown menu for selecting BGM in setting scene, involving `scene_settings.c` and `scene_setting.h.
	// 4. Switch and play the BGM if the corresponding option is selected.

	// Adjusting Volume:
	// 1. Declare global variables for storing the volume. (see `shared.h`, `shared.c`)
	// 2. Create a slider for adjusting volume in setting scene, involving `scene_settings.c` and `scene_setting.h.
		// 2.1. You may use checkbox to switch between mute and unmute.
	// 3. Adjust the volume and play when the button is pressed.

	// Selecting Map:
	// 1. Preload the map to `shared.c`.
	// 2. Create buttons(or dropdown menu) for selecting map in setting scene, involving `scene_settings.c` and `scene_setting.h.
		// 2.1. For player experience, you may also create another scene between menu scene and game scene for selecting map.
	// 3. Create buttons(or dropdown menu) for selecting map in setting scene, involving `scene_settings.c` and `scene_setting.h.
	// 4. Switch and draw the map if the corresponding option is selected.
		// 4.1. Suggestions: You may use `al_draw_bitmap` to draw the map for previewing. 
							// But the map is too large to be drawn in original size. 
							// You might want to modify the function to allow scaling.