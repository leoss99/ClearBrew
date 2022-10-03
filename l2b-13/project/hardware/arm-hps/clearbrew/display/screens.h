#ifndef SCREENS_H__
#define SCREENS_H__

#include "../logic/cJSON.h"
#include "../touch_control/touch.h"


// Constants representing each screen
#define SPLASH_SCREEN 0
#define MENU_SCREEN 1
#define SCALE_SCREEN 2
#define TIMER_SCREEN 3
#define RECIPE_SCREEN 4
#define STEP_SCREEN 5

// -------------------- Points for buttons --------------------

// LEFT HALF
Point BUTTON_LEFT_HALF_TL;
Point BUTTON_LEFT_HALF_BR;

// TOP RIGHT HALF
Point BUTTON_TOP_RIGHT_HALF_TL;
Point BUTTON_TOP_RIGHT_HALF_BR;

// BOTTOM RIGHT HALF
Point BUTTON_BOTTOM_RIGHT_HALF_TL;
Point BUTTON_BOTTOM_RIGHT_HALF_BR;

// TOP RIGHT THIRD
Point BUTTON_TOP_RIGHT_THIRD_TL;
Point BUTTON_TOP_RIGHT_THIRD_BR;

// MIDDLE RIGHT THIRD
Point BUTTON_MID_RIGHT_THIRD_TL;
Point BUTTON_MID_RIGHT_THIRD_BR;

// BOTTOM RIGHT THIRD
Point BUTTON_BOTTOM_RIGHT_THIRD_TL;
Point BUTTON_BOTTOM_RIGHT_THIRD_BR;

// TOP LEFT THIRD
Point BUTTON_TOP_LEFT_THIRD_TL;
Point BUTTON_TOP_LEFT_THIRD_BR;

// MIDDLE LEFT THIRD
Point BUTTON_MID_LEFT_THIRD_TL;
Point BUTTON_MID_LEFT_THIRD_BR;

// BOTTOM LEFT THIRD
Point BUTTON_BOTTOM_LEFT_THIRD_TL;
Point BUTTON_BOTTOM_LEFT_THIRD_BR;



// -------------------- End Points for buttons --------------------


// Dimensions of screen
int screen_x, screen_y;

void init_screens();

// Draw a box with "Clearbrew in the middle" -- deprecated
void draw_logo ();

// Draws the splash screen
void draw_splash_screen();

// Screen that shows after finishing a recipe
void draw_done_screen();

// Helper function for drawing a button
void draw_button(int x_1, int y_1, int x_2, int y_2, int colour, const char * text, int textlen);

// Draws the menu screen, where the user selects recipe, time, or weigh
void draw_menu_screen();


// Draws the timer screen
void draw_timer_screen();

// Draws the time on the timer screen, given the time in seconds
void draw_standalone_timer(int time);


// Draws the timer screen
void draw_weigh_screen();

// Draws the weight on the weigh screen, given the weight in grams
void draw_standalone_weight();


// Draws a recipe screen using the three input recipes
void draw_recipe_screen();

// Draws the screen for an individual step
void draw_step_screen(struct cJSON * step);


// Draws the weight on a weigh step, given the weight in grams
void draw_step_weight();

void draw_step_action();


// Draws the time on a time step, given the time in seconds
void draw_step_timer(int time);


#endif