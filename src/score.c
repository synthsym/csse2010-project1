/*
 * score.c
 *
 * Written by Peter Sutton
 */

#include "score.h"
#include <stdio.h>
#include <avr/pgmspace.h>
#include "terminalio.h"

uint16_t score;

void init_score(void) {
	score = 0;
}

void add_to_score(uint16_t value) {
	score += value;
	move_cursor(10, 12);
	printf_P(PSTR("Score: %i"), get_score());
}

uint16_t get_score(void) {
	return score;
}
