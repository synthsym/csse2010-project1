/*
 * lives.c
 *
 *  Created on: 20/05/2014
 *      Author: adam
 */

#include "lives.h"

uint8_t lives;

void init_lives(void) {
  // set ports for interacting with lives LEDS
  DDRA |= (1 << PINA7) | (1 << PINA6) | (1 << PINA5);

  lives = 3;
}

void add_life(void) {
  // cap at max 3 lives
  if(lives < 3) {
    lives++;
  }
}

void remove_life(void) {
  // cannot have negative lives
  if(lives > 0) {
    lives--;
  }
}

uint8_t get_lives(void) {
  return lives;
}

void display_lives(void) {
  // remove previous output
  PORTA &= 0xF0;

  // set the required bits
  for(int i = 0; i < lives; ++i) {
    PORTA |= (1 << i);
  }
}
