/*

	This file is used to handle the logic behind the ClearBrew GUI.
	See clearbrew_logic.h for function prototypes

*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <curl/curl.h>

#include "../display/screens.h"
#include "../touch_control/touch.h"
#include "../load_cell/load_cell.h"
#include "clearbrew_logic.h"
#include "cJSON.h"


// Keep a buffer for reading the recipes to
// NOTE FOR OFFLINE TESTING: this should be commented when not using curl
char recipes[4500];


/*
 *	Fetch the recipes from the backend and parse the received data.
 *	Stores the parsed JSON in recipes_json
*/
int fetch_recipes() {
	
	printf("Fetching recipes\n");
	
	
	// ########## FOR OFFLINE TESTING ##########
	// char * recipes = "[{\"_id\": \"624f1c9a01a99964a10916ce\", \"estimatedTime\": 600.0, \"name\": \"Simple Arabica Pour-over\", \"numSteps\": 11.0, \"rating\": 4.5, \"steps\": [{\"description\": \"Weigh 10 grams of coffee grounds\", \"stepType\": \"weigh\", \"targetValue\": 10.0}, {\"description\": \"Weigh 170 grams of water\", \"stepType\": \"weigh\", \"targetValue\": 170.0}, {\"description\": \"Boil the water\", \"stepType\": \"action\", \"targetValue\": 0.0}, {\"description\": \"Grind the coffee\", \"stepType\": \"action\", \"targetValue\": 0.0}, {\"description\": \"Place and wet the filter\", \"stepType\": \"action\", \"targetValue\": 0.0}, {\"description\": \"Place the coffee gorunds in the filter\", \"stepType\": \"action\", \"targetValue\": 0.0}, {\"description\": \"Pour 2 times as much water as coffee into the grounds\", \"stepType\": \"action\", \"targetValue\": 0.0}, {\"stepType\": \"time\", \"targetValue\": 35.0, \"description\": \"Pour the water\"}, {\"description\": \"Over the next 3 mins, continue to pour evenly over the grounds\", \"stepType\": \"action\", \"targetValue\": 0.0}, {\"stepType\": \"time\", \"targetValue\": 180.0, \"description\": \"Continue pouring\"}, {\"description\": \"Done!\", \"stepType\": \"action\", \"targetValue\": 0.0}], \"brewer\": \"Chemex\", \"origin\": \"Ecuador\", \"species\": \"Arabica\", \"roast\": \"Light\", \"user\": \"624e91caf2886b6691301b2a\"}, {\"_id\": \"624f1cb301a99964a10916cf\", \"estimatedTime\": 50400.0, \"name\": \"Cold-Brew Coffee\", \"numSteps\": 8.0, \"rating\": 4.0, \"steps\": [{\"description\": \"Weigh 30 grams of coffee grounds\", \"stepType\": \"weigh\", \"targetValue\": 30.0}, {\"description\": \"Weigh 120 grams of water\", \"stepType\": \"weigh\", \"targetValue\": 120.0}, {\"description\": \"Boil the water\", \"stepType\": \"action\", \"targetValue\": 0.0}, {\"description\": \"Grind the coffee to a medium coarse consistency\", \"stepType\": \"action\", \"targetValue\": 0.0}, {\"description\": \"Add the coffee grounds and water to a glass storage jar\", \"stepType\": \"action\", \"targetValue\": 0.0}, {\"stepType\": \"time\", \"targetValue\": 50400.0, \"description\": \"Refrigerate\"}, {\"description\": \"Strain the coffee through a fiine-mesh sieve\", \"stepType\": \"action\", \"targetValue\": 0.0}, {\"description\": \"Pour the strained coffee into a majon jar and refrigerate\", \"stepType\": \"action\", \"targetValue\": 0.0}, {\"description\": \"Done!\", \"stepType\": \"action\", \"targetValue\": 0.0}], \"brewer\": \"V60\", \"origin\": \"Brazil\", \"species\": \"Liberica\", \"roast\": \"Extra Dark\", \"user\": \"624e91caf2886b6691301b2a\"}, {\"_id\": \"624f1cce01a99964a10916d0\", \"estimatedTime\": 300.0, \"name\": \"Vanilla Lattee\", \"numSteps\": 15.0, \"rating\": 4.2, \"steps\": [{\"description\": \"Weigh 10 grams of coffee grounds\", \"stepType\": \"weigh\", \"targetValue\": 10.0}, {\"description\": \"Weigh 170 grams of water\", \"stepType\": \"weigh\", \"targetValue\": 170.0}, {\"description\": \"Boil the water\", \"stepType\": \"action\", \"targetValue\": 0.0}, {\"description\": \"Grind the coffee\", \"stepType\": \"time\", \"targetValue\": 60.0}, {\"description\": \"Place and wet the filter\", \"stepType\": \"action\", \"targetValue\": 0.0}, {\"description\": \"Place the coffee grounds in the filter\", \"stepType\": \"action\", \"targetValue\": 0.0}, {\"description\": \"Pour 2 times as much water as coffee into the grounds\", \"stepType\": \"time\", \"targetValue\": 35.0}, {\"description\": \"Pour evenly over the grounds\", \"stepType\": \"time\", \"targetValue\": 180.0}, {\"description\": \"Measure 1/2 cup of milk\", \"stepType\": \"action\", \"targetValue\": 0.0}, {\"description\": \"Measure 1//4 teaspoon of vanilla\", \"stepType\": \"action\", \"targetValue\": 0.0}, {\"description\": \"Combine milk and vanilla in a sealed container\", \"stepType\": \"action\", \"targetValue\": 0.0}, {\"description\": \"Heat and froth the milk using a milk frother\", \"stepType\": \"action\", \"targetValue\": 0.0}, {\"description\": \"Pour coffee and milk together\", \"stepType\": \"action\", \"targetValue\": 0.0}, {\"description\": \"Done!\", \"stepType\": \"action\", \"targetValue\": 0.0}], \"brewer\": \"Medium\", \"origin\": \"Columbia\", \"species\": \"Robusta\", \"roast\": \"Medium\", \"user\": \"624e91caf2886b6691301b2a\"}]";
	
	
	// ########## FOR FETCHING RECIPES FROM API ##########
	// Code taken from https://curl.se/libcurl/c/http-post.html and https://curl.se/libcurl/c/curl_easy_setopt.html
	// Also used https://stackoverflow.com/questions/2329571/c-libcurl-get-output-into-a-string
	
	// Create a curl handle
	CURL *curl;
	CURLcode res;

	/* In windows, this will init the winsock stuff */
	curl_global_init(CURL_GLOBAL_ALL);
	

	/* get a curl handle */
	curl = curl_easy_init();

	if(curl) {

		// Set the output to write
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		
		// Set the URL that we want to GET from
		curl_easy_setopt(curl, CURLOPT_URL, "https://zlofv343ec.execute-api.us-east-1.amazonaws.com/proxy_test/recipe/all");
		
		// Specify the HTTP verb as GET
		curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		
		/* Check for errors */
		if(res != CURLE_OK)
		  fprintf(stderr, "curl_easy_perform() failed: %s\n",
				  curl_easy_strerror(res));

		/* always cleanup */
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
	
	
	// Parse the new recipes
	cJSON * new_recipes_json = cJSON_Parse(recipes);
	
	
	// Check that recipes were parsed well
	if (new_recipes_json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        return 0;
    }
	
	// Print to console if we have fetched the recipes
	printf ("Success fetching recipes\n");
	printf ("Recipes fetched: %p\n", (void *) new_recipes_json);
	
	// Clear the old recpies
	cJSON_Delete(recipes_json);
	
	// Save the new recipes to a global variable
	recipes_json = new_recipes_json;
	
	
	return 1;
}

/*
 *	A helper function for use with curl.
 *	
 *	It is passed to curl_easy_setopt with CURLOPT_WRITEFUNCTION in order to change the location
 *	to which the HTTP GET data is printed. When used, data from the HTTP GET is printed to the
 *	global buffer `recipes`.
 *
 *	NOTE FOR OFFLINE TESTING: when not using curl, this function should be commented.
 *
*/
size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
	sprintf (recipes, "%s", (const char *)buffer);
	return size * nmemb;
}


/*
 *	Given the location of a touch report and a screen, determines which button was pressed
 *	and what action to perform.
*/
void get_action(Point p, int screen) {
	switch (screen) {
		case SPLASH_SCREEN:
			// ignore touches in splash screen
			break;
		case MENU_SCREEN:
			get_action_menu(p);
			break;
		case SCALE_SCREEN:
			get_action_scale(p);
			break;
		case TIMER_SCREEN:
			get_action_timer(p);
			break;
		case RECIPE_SCREEN:
			get_action_recipe(p);
			break;
		case STEP_SCREEN:
			get_action_step(p);
			break;
		default:
			break;
	}
}

/*
 *	Given the location of a touch report, determines which button on the menus screen was pressed
 *	and performs the associated action
*/
void get_action_menu(Point p) {
	if (is_button_pressed(p,BUTTON_LEFT_HALF_TL,BUTTON_LEFT_HALF_BR) == 1) {
		// Recipes button - left half
		printf("Recipes button pressed\n");
		action_goto_recipe();

	} else if (is_button_pressed(p,BUTTON_TOP_RIGHT_HALF_TL,BUTTON_TOP_RIGHT_HALF_BR) == 1){
		// Timer button - top right
		printf("Timer button pressed\n");
		action_goto_timer();

	} else if (is_button_pressed(p,BUTTON_BOTTOM_RIGHT_HALF_TL,BUTTON_BOTTOM_RIGHT_HALF_BR) == 1){
		// Scale button - bottom right
		printf("Scale button pressed\n");
		action_goto_scale();
	}
}

/*
 *	Given the location of a touch report, determines which button on the timer screen was pressed
 *	and performs the associated action
*/
void get_action_timer(Point p) {
	
	if (is_button_pressed(p, BUTTON_MID_RIGHT_THIRD_TL,BUTTON_MID_RIGHT_THIRD_BR) == 1) {
		// Reset button - middle right
		// Redrawing the timer screen will reset it
		action_goto_timer();
	} else if (is_button_pressed(p, BUTTON_BOTTOM_RIGHT_THIRD_TL,BUTTON_BOTTOM_RIGHT_THIRD_BR) == 1) {
		// Back button - bottom right
		action_goto_menu();
	}
}

/*
 *	Given the location of a touch report, determines which button on the scale screen was pressed
 *	and performs the associated action
*/
void get_action_scale(Point p) {
	
	if (is_button_pressed(p, BUTTON_MID_RIGHT_THIRD_TL,BUTTON_MID_RIGHT_THIRD_BR) == 1) {
		// Tare (zero) button - middle right
		load_cell_tare();
		// Redrawing the weigh screen resets the scale value
		draw_standalone_weight();
		
	} else if (is_button_pressed(p, BUTTON_BOTTOM_RIGHT_THIRD_TL,BUTTON_BOTTOM_RIGHT_THIRD_BR) == 1) {
		// Back button - bottom right
		action_goto_menu();
	}
}

/*
 *	Given the location of a touch report, determines which button on the recipe screen was pressed
 *	and performs the associated action
*/
void get_action_recipe(Point p) {
	if (is_button_pressed(p, BUTTON_TOP_LEFT_THIRD_TL,BUTTON_TOP_LEFT_THIRD_BR) == 1) {
		// First recipe button
		action_goto_first_recipe();
		
	} else if (is_button_pressed(p, BUTTON_MID_LEFT_THIRD_TL,BUTTON_MID_LEFT_THIRD_BR) == 1) {
		// Second recipe button
		action_goto_second_recipe();
		
	} else if (is_button_pressed(p, BUTTON_BOTTOM_LEFT_THIRD_TL,BUTTON_BOTTOM_LEFT_THIRD_BR) == 1) {
		// Third recipe button
		action_goto_third_recipe();
		
	} else if (is_button_pressed(p, BUTTON_BOTTOM_RIGHT_THIRD_TL,BUTTON_BOTTOM_RIGHT_THIRD_BR) == 1) {
		// Back button
		action_goto_menu();
	}
}


/*
 *	Given the location of a touch report, determines which button on a step screen was pressed
 *	and performs the associated action
*/
void get_action_step(Point p) {
	
	if (is_button_pressed(p, BUTTON_TOP_RIGHT_THIRD_TL,BUTTON_TOP_RIGHT_THIRD_BR) == 1) {
		// Next button - top right
		action_goto_next_step();
		
	} else if (is_button_pressed(p, BUTTON_MID_RIGHT_THIRD_TL,BUTTON_MID_RIGHT_THIRD_BR) == 1) {
		
		// Check the type of step, and perform the appropriate action
		const char * type = cJSON_GetObjectItemCaseSensitive(current_step, "stepType")->valuestring;
		
		if (!strcmp(type, "time")) {
			// Time step: reset the timer
			action_reset_timer();
			
		} else if (!strcmp(type, "weigh")){
			// Weigh step: tare the scale
			load_cell_tare();
			// Display the updated weight
			draw_step_weight();
			
		} else {
			// There is no button for action steps; do nothing
			draw_step_action();
		}
	} else if (is_button_pressed(p, BUTTON_BOTTOM_RIGHT_THIRD_TL,BUTTON_BOTTOM_RIGHT_THIRD_BR) == 1) {
		// This button is hidden, but we keep it as an emergency "return to menu" button for demos
		// It is in the bottom right corner
		action_goto_menu();
	}
}
/*
 *	Checks if press is within the button bounds, given top-left and bottom-right corners.
 *
 *	Returns 1 if within bounds, -1 if out of bounds.
*/
int is_button_pressed(Point press, Point button_tl, Point button_br) {
	if (press.x < button_br.x && press.x > button_tl.x &&
		press.y < button_br.y && press.y > button_tl.y) {
		return 1;
	} else {
		return -1;
	}
}


/*
 *	Go to the menu screen
 */
void action_goto_menu() {
	draw_menu_screen();
	current_screen = MENU_SCREEN;
}

/*
 *	Go to the standalone timer screen
 */
void action_goto_timer() {
	// Set the start time of the timer
	start_time = time(NULL);
	
	draw_timer_screen();
	draw_timer_screen();
	current_screen = TIMER_SCREEN;
}

/*
 *	Go to the standalone scale screen
 */
void action_goto_scale() {
	load_cell_tare();
	draw_weigh_screen();
	draw_weigh_screen();
	current_screen = SCALE_SCREEN;
}

/*
 *	Go to the recipes screen
 */
void action_goto_recipe() {
	draw_recipe_screen();
	current_screen = RECIPE_SCREEN;
}

/*
 *	Go to the first recipe's first step
 */
void action_goto_first_recipe() {
	printf("First recipe pressed\n");
	
	// Go to first recipe, first step
	current_recipe = recipes_json->child;
	current_step = cJSON_GetObjectItemCaseSensitive(current_recipe, "steps")->child;
	
	if (current_step == NULL) printf("current step is null\n");
	
	// Draw current step twice to fill both buffers
	draw_step_screen(current_step);
	draw_step_screen(current_step);
	current_screen = STEP_SCREEN;
}

/*
 *	Go to the second recipe's first step
 */
void action_goto_second_recipe() {
	printf("second recipe pressed");
	
	// pick 2nd recipe
	current_recipe = recipes_json->child->next;
	current_step = cJSON_GetObjectItemCaseSensitive(current_recipe, "steps")->child;
	
	// Draw current step twice to fill both buffers
	draw_step_screen(current_step);
	draw_step_screen(current_step);
	current_screen = STEP_SCREEN;
}

/*
 *	Go to the third recipe's first step
 */
void action_goto_third_recipe() {
	printf("third recipe pressed");
	
	// pick 3rd recipe
	current_recipe = recipes_json->child->next->next;
	current_step = cJSON_GetObjectItemCaseSensitive(current_recipe, "steps")->child;
	
	// Draw current step twice to fill both buffers
	draw_step_screen(current_step);
	draw_step_screen(current_step);
	current_screen = STEP_SCREEN;
}

/*
 *	Go to the next step in the current recipe
 */
void action_goto_next_step() {

	if (current_step == NULL) {
		printf("current step is null\n");
	}
	
	// Get the next step
	current_step = current_step->next;

	// If the current step is null, there are no more steps to show
	if (current_step == NULL) {
		
		// Draw a nice screen to tell the user to enjoy
		draw_done_screen();
		sleep(2);
		
		// Return to the menu when done
		action_goto_menu();
		
	} else {
		// Draw current step twice to fill both buffers
		draw_step_screen(current_step);
		draw_step_screen(current_step);
		current_screen = STEP_SCREEN;
	}
}

void action_reset_timer() {
	// Redraw the timer step screen, which restarts the time
	draw_step_screen(current_step);
	draw_step_screen(current_step);
}
