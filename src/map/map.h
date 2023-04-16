#ifndef map_h
#define map_h

#include <stdio.h>
#include <stdbool.h>

int get_number_of_rows_in_map(void);
int get_number_of_columns_in_map(void);
int get_the_width_of_the_tiles(void);
int get_the_height_of_the_tiles(void);
int get_tile_information(int x, int y);
bool encounters_forbidden_tile(int x, int y);

#endif