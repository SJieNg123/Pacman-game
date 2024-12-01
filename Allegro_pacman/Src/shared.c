// [shared.c]
// you should define the shared variable declared in the header here.

#include "shared.h"
#include "utility.h"
#include "game.h"
// #include "scene_menu.h"

ALLEGRO_FONT* font_pirulen_32;
ALLEGRO_FONT* font_pirulen_24;
ALLEGRO_SAMPLE* themeMusic1 = NULL;
ALLEGRO_SAMPLE* themeMusic2 = NULL;
ALLEGRO_SAMPLE* PACMAN_MOVESOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_DEATH_SOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_POWERUP_SOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_EATGHOST_SOUND = NULL;
ALLEGRO_FONT* menuFont = NULL;
ALLEGRO_FONT* largeFont = NULL;

int fontSize = 30;
int largeSize = 50;
float music_volume = 0.5;
float effect_volume = 0.5;
bool gameDone = false;
bool oriBGM = true;
bool ezmode = true;
bool bgmMute = false;

/*
	A way to accelerate is load assets once.
	And delete them at the end.
	This method does provide you better management of memory.
	
*/

void shared_init(void) {
	menuFont = load_font("Assets/Minecraft.ttf", fontSize);
	largeFont = load_font("Assets/Minecraft.ttf", largeSize);
	themeMusic1 = load_audio("Assets/Music/original_theme.ogg");
	themeMusic2 = load_audio("Assets/Music/capybara-song.ogg");
	PACMAN_MOVESOUND = load_audio("Assets/Music/pacman-chomp.ogg");
	PACMAN_DEATH_SOUND = load_audio("Assets/Music/pacman_death.ogg");
	PACMAN_POWERUP_SOUND = load_audio("Assets/Music/pacman_eatfruit.ogg");
	PACMAN_EATGHOST_SOUND = load_audio("Assets/Music/pacman_eatghost.ogg");
}

void shared_destroy(void) {
	al_destroy_font(menuFont);
	al_destroy_font(largeFont);
	al_destroy_sample(themeMusic1);
	al_destroy_sample(themeMusic2);
	al_destroy_sample(PACMAN_MOVESOUND);
	al_destroy_sample(PACMAN_DEATH_SOUND);
	al_destroy_sample(PACMAN_POWERUP_SOUND);
	al_destroy_sample(PACMAN_EATGHOST_SOUND);
}