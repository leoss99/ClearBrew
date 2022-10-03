#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <intelfpgaup/KEY.h>
#include <intelfpgaup/video.h>

#include "fonts.h"
#include "screens.h"
#include "../logic/clearbrew_logic.h"
#include "../logic/cJSON.h"
#include "../touch_control/touch.h"
#include "../load_cell/load_cell.h"




/*
 *	Here lies the original logo for our project.
 *	It was quickly abandoned, since drawing pictures with boxes is not fun
*/
void draw_logo () {
	video_erase(); // clears all text
	video_clear(); // clears all graphics
	
	// White background
	video_box(0, 0, screen_x - 1, screen_y - 1, video_WHITE);
	
	// C
	video_box(50,40,140,70,video_CYAN);
	video_box(50,40,80,200,video_CYAN);
	video_box(50,170,140,200,video_CYAN);
	
	// B
	video_box(180,40,250,70,video_CYAN); // top
	video_box(180,40,210,200,video_CYAN); // left side
	video_box(180,170,270,200,video_CYAN); // bottom
	video_box(180,100,250,140,video_CYAN); // middle
	video_box(240,60,270,110,video_CYAN); // top right
	video_box(240,130,270,200,video_CYAN); // bottom right
}


/*
 *	Initialize the bounds for all of the buttons
 *	These are based on the screen resolution
*/
void init_screens() {

	// LEFT HALF
	BUTTON_LEFT_HALF_TL = (Point){10,10};
	BUTTON_LEFT_HALF_BR = (Point){screen_x/2 - 10, screen_y - 10};

	// TOP RIGHT HALF
	BUTTON_TOP_RIGHT_HALF_TL = (Point){screen_x/2 + 10, 10};
	BUTTON_TOP_RIGHT_HALF_BR = (Point){screen_x - 10, screen_y/2 - 10};

	// BOTTOM RIGHT HALF
	BUTTON_BOTTOM_RIGHT_HALF_TL = (Point){screen_x/2 + 10, screen_y/2 + 10};
	BUTTON_BOTTOM_RIGHT_HALF_BR = (Point){screen_x - 10, screen_y - 10};

	// TOP RIGHT THIRD
	BUTTON_TOP_RIGHT_THIRD_TL = (Point){2*screen_x/3+5,10};
	BUTTON_TOP_RIGHT_THIRD_BR = (Point){screen_x-10,screen_y/3-5};

	// MIDDLE RIGHT THIRD
	BUTTON_MID_RIGHT_THIRD_TL = (Point){2*screen_x/3+5,screen_y/3+5};
	BUTTON_MID_RIGHT_THIRD_BR = (Point){screen_x-10,2*screen_y/3-5};

	// BOTTOM RIGHT THIRD
	BUTTON_BOTTOM_RIGHT_THIRD_TL = (Point){2*screen_x/3+5,2*screen_y/3+5};
	BUTTON_BOTTOM_RIGHT_THIRD_BR = (Point){screen_x-10,screen_y-10};


	// TOP LEFT THIRD
	BUTTON_TOP_LEFT_THIRD_TL = (Point){10,10};
	BUTTON_TOP_LEFT_THIRD_BR = (Point){2*screen_x/3-5,screen_y/3-5};

	// MIDDLE LEFT THIRD
	BUTTON_MID_LEFT_THIRD_TL = (Point){10,screen_y/3+5};
	BUTTON_MID_LEFT_THIRD_BR = (Point){2*screen_x/3-5,2*screen_y/3-5};

	// BOTTOM LEFT THIRD
	BUTTON_BOTTOM_LEFT_THIRD_TL = (Point){10,2*screen_y/3+5};
	BUTTON_BOTTOM_LEFT_THIRD_BR = (Point){2*screen_x/3-5,screen_y-10};
}


// Draws the splash screen
void draw_splash_screen() {
        // White background
        video_box(0, 0, screen_x - 1, screen_y - 1, video_WHITE);

        video_box(50,100,screen_x-51,screen_y-101,video_CYAN);

        write_word(55, 102, "ClearBrew", video_CYAN, video_ORANGE, GIANT);
		
		// Swap the front and back buffers
		video_show();
}

// Draw the done screen when a user finishes a recipe
void draw_done_screen() {
	// Simply draw a green screen with black text in the middle
	draw_button(0,0,screen_x-1,screen_y-1,video_GREEN,"All Done!", 9);
	// Swap the front and back buffers
	video_show();
}

/*
 *	Draw a button
 *
 *	x_1,y_1:	Coordinates of the top left corner
 *	x_2,y_2:	Coordinates of the bottom right corner
 *	colour:		The colour of the button (text colour is always black)
 *	text:		The text to be displayed
 *	textlen:	The length of text; if desired, use strlen(text)
*/
void draw_button(int x_1, int y_1, int x_2, int y_2, int colour, const char * text, int textlen) {
	
	// Draw the base of the button
	video_box (x_1, y_1, x_2, y_2, colour);
	
	// Determine position of centered text:
	// Large font size is 16x27 per character
	int x = (x_1 + x_2)/2 - textlen * 8;
	int y = (y_1 + y_2)/2 - 13;
	
	write_word(x, y, text, video_WHITE, video_BLACK, LARGE);
}

// Draw the menu screen
void draw_menu_screen() {
	
	// Clear the screen
	video_box(0,0,screen_x-1,screen_y-1, video_WHITE);
	
	// Draw recipes button
	draw_button(BUTTON_LEFT_HALF_TL.x, BUTTON_LEFT_HALF_TL.y,
				BUTTON_LEFT_HALF_BR.x, BUTTON_LEFT_HALF_BR.y,
				video_CYAN, "Recipes", 7);
	
	// Draw the timer button
	draw_button(BUTTON_TOP_RIGHT_HALF_TL.x, BUTTON_TOP_RIGHT_HALF_TL.y,
				BUTTON_TOP_RIGHT_HALF_BR.x, BUTTON_TOP_RIGHT_HALF_BR.y,
				video_GREEN, "Timer", 5);
	
	// Draw the scale button
	draw_button(BUTTON_BOTTOM_RIGHT_HALF_TL.x, BUTTON_BOTTOM_RIGHT_HALF_TL.y,
				BUTTON_BOTTOM_RIGHT_HALF_BR.x, BUTTON_BOTTOM_RIGHT_HALF_BR.y,
				video_YELLOW, "Scale", 5);
	
	// Show the drawn objects
	video_show();
}


// Draw the timer screen
void draw_timer_screen() {
	
	// White background
	video_box(0,0,screen_x-1,screen_y-1, video_WHITE);
	
	// Draw the timer box
	video_box(10,10,2*screen_x/3-5,screen_y-10, video_CYAN);
	
	// Draw the Reset button
	draw_button(BUTTON_MID_RIGHT_THIRD_TL.x,BUTTON_MID_RIGHT_THIRD_TL.y,
				BUTTON_MID_RIGHT_THIRD_BR.x,BUTTON_MID_RIGHT_THIRD_BR.y,
				video_GREEN,"Reset", 5);
	
	// Draw the back button
	draw_button(BUTTON_BOTTOM_RIGHT_THIRD_TL.x,BUTTON_BOTTOM_RIGHT_THIRD_TL.y,
				BUTTON_BOTTOM_RIGHT_THIRD_BR.x,BUTTON_BOTTOM_RIGHT_THIRD_BR.y,
				video_RED,"Back", 4);
	
	// Initially, draw the timer as 0
	draw_standalone_timer(0);
}

// Draws the time on the timer screen, given the time in seconds
void draw_standalone_timer(int time) {
	
	// Time cannot be negative; stop timer at 0
	if (time < 0) time = 0;
	
	// Draw a box over the previous time
	video_box(10,10,2*screen_x/3-5,screen_y-10, video_CYAN);
	
	// Get minutes and seconds
	int seconds = time % 60;
	int minutes = time / 60;
	
	// Print {minutes : seconds}
	char timer_buffer[5];
	sprintf (timer_buffer, "%d:%02d", minutes, seconds);
	write_word(30, 100, timer_buffer, video_WHITE, video_BLACK, MASSIVE);

	// Show the drawn objects
	video_show();
}


// Draws the scale screen
void draw_weigh_screen() {
	
	// White background
	video_box(0,0,screen_x-1,screen_y-1, video_WHITE);
	
	// Draw the weight box
	video_box(10,10,2*screen_x/3-5,screen_y-10, video_CYAN);
	
	// Draw the Tare box
	draw_button(BUTTON_MID_RIGHT_THIRD_TL.x,BUTTON_MID_RIGHT_THIRD_TL.y,
				BUTTON_MID_RIGHT_THIRD_BR.x,BUTTON_MID_RIGHT_THIRD_BR.y,
				video_GREEN,"Tare", 4);
	
	// Draw the back button
	draw_button(BUTTON_BOTTOM_RIGHT_THIRD_TL.x,BUTTON_BOTTOM_RIGHT_THIRD_TL.y,
				BUTTON_BOTTOM_RIGHT_THIRD_BR.x,BUTTON_BOTTOM_RIGHT_THIRD_BR.y,
				video_RED,"Back", 4);
	
	// Draw the weight, reading from the load cell
	draw_standalone_weight();
}

// Draws the weight on the scale screen, given the weight in grams
void draw_standalone_weight() {
	// Draw a box over the previous weight
	video_box(10,10,2*screen_x/3-5,screen_y-10, video_CYAN);
	
	// Print {`weight`g}
	char buffer[5];
	sprintf (buffer, "%dg", (int)load_cell_get_weight());
	write_word(50, 100, buffer, video_WHITE, video_BLACK, MASSIVE);
	
	// Show the drawn objects
	video_show();
}


// Draws a recipe screen using the three input recipes
void draw_recipe_screen(){
	
	// Get 3 recipes to display, checking for null values
	// If recipes are null, do not display them because they do not exist
	cJSON * recipe_1 = recipes_json->child;
	cJSON * recipe_2 = NULL;
	cJSON * recipe_3 = NULL;
	const char * recipe_name_1;
	const char * recipe_name_2;
	const char * recipe_name_3;
	
	if (recipe_1 != NULL) {
		printf("recipe 1 is NOT null\n");
		
		// Get the first recipe name
		recipe_name_1 = cJSON_GetObjectItemCaseSensitive(recipe_1, "name")->valuestring;
		if (recipe_name_1 == NULL) {
			printf("recipe 1 name is null\n");
		} else {
			printf("recipe 1 name is NOT null\n");
		}

		// Get the second recipe and its name
		recipe_2 = recipe_1->next;
		recipe_name_2 = cJSON_GetObjectItemCaseSensitive(recipe_2, "name")->valuestring;
		if (recipe_2 != NULL) {
			// Get the third recipe and its name
			recipe_3 = recipe_2->next;
			recipe_name_3 = cJSON_GetObjectItemCaseSensitive(recipe_3, "name")->valuestring;
		}
	}
	
	

	// Draw the background
	video_box(0,0,screen_x-1,screen_y-1, video_WHITE);
	
	// Draw the first 3 recipes in the list
	// Truncate the recipe name to 10 characters if needed
	
	if (recipe_1 != NULL) {
		char shortName[11];
		int len = strlen(recipe_name_1);
		// truncate name to 10 chars
		if (len > 10) {
			len = 10;
		}
		shortName[len] = '\0';
		memcpy(shortName,recipe_name_1, len);
		
		
		draw_button(BUTTON_TOP_LEFT_THIRD_TL.x,BUTTON_TOP_LEFT_THIRD_TL.y,
					BUTTON_TOP_LEFT_THIRD_BR.x,BUTTON_TOP_LEFT_THIRD_BR.y,
					video_CYAN,shortName, len);
	}
	
	if (recipe_2 != NULL) {
		char shortName[11];
		int len = strlen(recipe_name_2);
		// truncate name to 10 chars
		if (len > 10) {
			len = 10;
		}
		shortName[len] = '\0';
		memcpy(shortName,recipe_name_2, len);
		
		draw_button(BUTTON_MID_LEFT_THIRD_TL.x,BUTTON_MID_LEFT_THIRD_TL.y,
					BUTTON_MID_LEFT_THIRD_BR.x,BUTTON_MID_LEFT_THIRD_BR.y,
					video_CYAN,shortName, len);
	}
	
	if (recipe_3 != NULL) {
		char shortName[11];
		int len = strlen(recipe_name_3);
		// truncate name to 10 chars
		if (len > 10) {
			len = 10;
		}
		shortName[len] = '\0';
		memcpy(shortName,recipe_name_3, len);
		
		
		draw_button(BUTTON_BOTTOM_LEFT_THIRD_TL.x,BUTTON_BOTTOM_LEFT_THIRD_TL.y,
					BUTTON_BOTTOM_LEFT_THIRD_BR.x,BUTTON_BOTTOM_LEFT_THIRD_BR.y,
					video_CYAN,shortName, len);
	}
	
	// Draw the back button
	draw_button(BUTTON_BOTTOM_RIGHT_THIRD_TL.x,BUTTON_BOTTOM_RIGHT_THIRD_TL.y,
				BUTTON_BOTTOM_RIGHT_THIRD_BR.x,BUTTON_BOTTOM_RIGHT_THIRD_BR.y,
				video_RED,"Back", 4);
	
	// Show the drawn objects
	video_show();
}


// Draws the screen for an individual step
void draw_step_screen(struct cJSON * step) {
	// Clear the screen
	video_box(0,0,screen_x-1,screen_y-1, video_WHITE);
	
	// Get the description
	const char * description = cJSON_GetObjectItemCaseSensitive(current_step, "description")->valuestring;
	
	if (description != NULL) {
		
		// Truncate the description to 41 characters if needed
		// Ideally, we would split long descriptions into multi-line, but this was not achievable
		//	given our contrained time
		
		char shortDesc[41];
		int len = strlen(description);
		if (len > 40) {
			len = 40;
		}
		shortDesc[len] = '\0';
		memcpy(shortDesc,description, len);
		
		write_word(10, 10, shortDesc, video_WHITE, video_BLACK, SMALL);
	}
	
	
	
	// Draw timer/weight box in bottom left
	video_box(10,screen_y/2+5,2*screen_x/3-5,screen_y-10,video_CYAN);
	
	// Draw Next button in top right
	draw_button(BUTTON_TOP_RIGHT_THIRD_TL.x,BUTTON_TOP_RIGHT_THIRD_TL.y,
				BUTTON_TOP_RIGHT_THIRD_BR.x,BUTTON_TOP_RIGHT_THIRD_BR.y,
				video_GREEN,"Next", 4);
	
	
	// Draw Pause/Tare button
	// Check here if type is weight or time
	const char * type = cJSON_GetObjectItemCaseSensitive(current_step, "stepType")->valuestring;
	printf("type of step is: %s\n", type);
	
	if (!strcmp(type, "time")) {
		
		// In a time step, draw the reset button and timer
		
		draw_button(BUTTON_MID_RIGHT_THIRD_TL.x,BUTTON_MID_RIGHT_THIRD_TL.y,
				BUTTON_MID_RIGHT_THIRD_BR.x,BUTTON_MID_RIGHT_THIRD_BR.y,
				video_GREEN,"Reset", 5);
		
		// Draw a timer, counting down from the target time
		start_time = time(NULL);
		draw_step_timer(cJSON_GetObjectItemCaseSensitive(current_step, "targetValue")->valueint);
		
	} else if (!strcmp(type, "weigh")){

		// In a weigh step, draw the tare button and the weight

		draw_button(BUTTON_MID_RIGHT_THIRD_TL.x,BUTTON_MID_RIGHT_THIRD_TL.y,
				BUTTON_MID_RIGHT_THIRD_BR.x,BUTTON_MID_RIGHT_THIRD_BR.y,
				video_GREEN,"Tare", 4);
		
		draw_step_weight();
		
	} else {
		// In an action step, there are no extra buttons or values to show
		// Simply show the frame buffer
		video_show();
	}
}

// Draws the weight on a weigh step, given the weight in grams
void draw_step_weight() {
	// Get the current weight
	int weight = (int)load_cell_get_weight();
	// Get the target weight for this step
	int target_weight = cJSON_GetObjectItemCaseSensitive(current_step, "targetValue")->valueint;


	if (weight == target_weight) {
		// We have reached the target weight; show a DONE button
		video_box(10,screen_y/2+5,2*screen_x/3-5,screen_y-10,video_GREEN);
		write_word(25, 150, "Done", video_WHITE, video_BLACK, MASSIVE);
	} else {
		
		// cover the previous weight
		video_box(10,screen_y/2+5,2*screen_x/3-5,screen_y-10,video_CYAN);	
		
		// print {weight g}
		char buffer[5];
		sprintf (buffer, "%dg", weight);
		write_word(25, 150, buffer, video_WHITE, video_BLACK, MASSIVE);
		
	}
	
	// Display the drawn objects
	video_show();
}

void draw_step_action() {
	// Nothing to show
	video_box(10,screen_y/2+5,2*screen_x/3-5,screen_y-10,video_CYAN);
	// Display the drawn objects
	video_show();
}

// Draws the time on a time step, given the time in seconds
void draw_step_timer(int time) {
	
	if (time < 0) {
		// Time has reached 0; show a DONE button
		video_box(10,screen_y/2+5,2*screen_x/3-5,screen_y-10,video_GREEN);
		write_word(25, 150, "Done", video_WHITE, video_BLACK, MASSIVE);

	} else {
		// Cover the previous time
		video_box(10,screen_y/2+5,2*screen_x/3-5,screen_y-10,video_CYAN);
		
		// Get minutes and seconds
		int seconds = time % 60;
		int minutes = time / 60;
		
		// print {minutes : seconds}
		char timer_buffer[5];
		sprintf (timer_buffer, "%d:%02d", minutes, seconds);
		write_word(25, 150, timer_buffer, video_WHITE, video_BLACK, MASSIVE);
	}
	
	// Display the drawn objects
	video_show();
}