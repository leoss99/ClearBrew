#ifndef CLEARBREW_LOGIC_H__
#define CLEARBREW_LOGIC_H__

// __________ GLOBAL VARIABLES __________

// JSON representing the list of available recipes
struct cJSON * recipes_json;
// JSON representing the current recipe
struct cJSON * current_recipe;
// JSON representing the current recipe
struct cJSON * current_step;
// enum representing the current selected screen
int current_screen;
// The time at which the timer was started, in seconds
time_t start_time;


// Function prototypes
// See clearbrew_logic.c for function specifications

// For fetching recipes from API
int fetch_recipes();

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);


// For determining actions to take when screen is touched
int is_button_pressed(Point press, Point button_tl, Point button_br);

void get_action(Point p, int screen);

void get_action_menu(Point p);

void get_action_timer(Point p);

void get_action_scale(Point p);

void get_action_recipe(Point p);

void get_action_step(Point p);


// For performing actions
void action_goto_menu();

void action_goto_timer();

void action_goto_scale();

void action_goto_recipe();

void action_goto_first_recipe();

void action_goto_second_recipe();

void action_goto_third_recipe();

void action_goto_next_step();

void action_reset_timer();


#endif