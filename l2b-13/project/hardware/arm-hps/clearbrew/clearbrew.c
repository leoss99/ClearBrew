#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <intelfpgaup/KEY.h>
#include <intelfpgaup/video.h>

#include "display/fonts.h"
#include "display/screens.h"
#include "logic/clearbrew_logic.h"
#include "touch_control/touch.h"
#include "load_cell/load_cell.h"
#include "address_map_arm.h"


int char_x, char_y;


volatile sig_atomic_t stop;
void catchSIGINT(int signum){
    stop = 1;
}


/*
 *      This is the main program that runs the clearbrew GUI and logic.
 *		It was modified from the draw_lines tutorial program, available on the ARM HPS from the Intel Monitor Program.
 */
int main(int argc, char *argv[]) {
	
	time_t t;       // used to seed the rand() function

	// catch SIGINT from ^C, instead of having it abruptly close this program
	signal(SIGINT, catchSIGINT);

	srand ((unsigned) time(&t));    // seed the rand function
	// Open the Key and video device drivers
	if (!KEY_open ( ) || !video_open ( ))
			return -1;

	video_read (&screen_x, &screen_y, &char_x, &char_y);  // get screen & text size
	video_erase ( );  // erase any text on the screen

	// Clear both buffers
	video_clear ( );
	video_show ( );
	video_clear ( );

	// Draw the splash screen
	draw_splash_screen();

	// Fetch the list of recipes from backend
	int err = fetch_recipes();

	// If there was an error fetching recipes, stop the program
	if (!err) stop = 1;


	// Initialize the touch controller
	printf("initializing touch\n");
	int fd = -1;
	init_touch(fd);

	// Initialize the load cell
	printf("initializng load cell\n");
	init_load_cell();

	// Initialize the screens and button placements
	printf("initializing screens\n");
	init_screens();

	// Wait so that the user can enjoy the splash screen
	sleep(2);
	
	// Draw the menu screen
	draw_menu_screen();
	current_screen = MENU_SCREEN;
	
	// We will track the time of the last screen touch to avoid mistaken double-clicks
	time_t debounce = time(NULL);

	// Loop until the program is stopped
	// The program stops when there is a SIGINT (^C)
	while (!stop) {
		// Wait for touch report
		Point p = get_press_timeout(10000);
		
		// If the touch report did not time out and the last touch was more than 1 second ago, perform the action
		if (p.x != -1 && p.y != -1 && (time(NULL) - debounce > 1)) {
			printf("Press received: (%d, %d)\n", p.x, p.y);

			// Perform the action associated with this touch location and screen
			get_action(p, current_screen);
			// Debounce to avoid mistaken double-clicks
			debounce = time(NULL);
		}
		
		// Check if the display needs to be updated
		if (current_screen == SCALE_SCREEN) {
			// On the standalone scale screen, update the displayed weight
			draw_standalone_weight();
			
		} else if (current_screen == TIMER_SCREEN) {
			// On the standalone timer screen, draw the time, counting up from start time
			draw_standalone_timer((int)(time(NULL) - start_time));
			
		} else if (current_screen == STEP_SCREEN) {
			// Get the type of step for this screen
			const char * type = cJSON_GetObjectItemCaseSensitive(current_step, "stepType")->valuestring;
			
			if (!strcmp(type, "time")) {
				// If this is a time step, draw the updated time, counting down from step_time
				int step_time = cJSON_GetObjectItemCaseSensitive(current_step, "targetValue")->valueint;
				draw_step_timer(step_time - (int)(time(NULL) - start_time));
				
			} else if (!strcmp(type, "weigh")){
				// If this is a weigh step, draw the weight
				draw_step_weight();
			}
			
			// If this was an action step, there is nothing to draw
		}
	}
	
	// The program is stopping. Let's clean up
	
	// Clear and show the screen
	video_clear();
	video_show();
	
	// Close the video and key drivers
	video_close ( );
	KEY_close ( );
	
	// Close the load cell and touch connections
	unmap_physical(LW_virtual,LW_BRIDGE_SPAN);
	close_physical(fd);
	
	printf ("\nExiting clearbrew program\n");
	return 0;
}