
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "../address_map_arm.h"
#include "../touch_control/touch.h"
#include "load_cell.h"

// Initialize the load cell
void init_load_cell(void) {
	// By calibrating against another scale, we calculated these values
	// Somewhere in this repo is the excel file for this...
	load_cell_zero = -150.94;
	load_cell_slope = 0.035;
	
	// Set virtual address pointer to I/O port
	LOAD_CELL_ptr = (unsigned int *) (LW_virtual + LOAD_CELL_BASE);
}

// Tare (set the weight to 0) the load cell
void load_cell_tare(void) {
	
	// Given a signal from the load cell, we can calculate the weight using the formula:
	// weight = slope * signal + zero
	
	// So, to tare the load cell, we must set the zero value as follows:
	// 0 = slope * signal + zero
	// zero = -slope * signal
	
	load_cell_zero = -(double)(*LOAD_CELL_ptr) * load_cell_slope;
}

// Return the weight in grams
double load_cell_get_weight(void) {
	
	// Given a signal from the load cell, we can calculate the weight using the formula:
	// weight = slope * signal + zero
	return load_cell_slope * (double)(*LOAD_CELL_ptr) + load_cell_zero;
}
