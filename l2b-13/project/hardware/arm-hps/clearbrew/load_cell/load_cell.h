#ifndef LOAD_CELL_H__
#define LOAD_CELL_H__


volatile unsigned int * LOAD_CELL_ptr;
double load_cell_zero;
double load_cell_slope;


void init_load_cell(void);

void load_cell_tare(void);

double load_cell_get_weight(void);

#endif