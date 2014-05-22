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
  DDRA |= 0xF0;

  lives = 3;
}

void add_life(void) {
  // cap at max 3 lives
  if(lives < 5) {
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

uint8_t is_supa_dead(void) {
  return (lives == 0);
}

void display_lives(void) {
  // remove previous output
  PORTA &= 0x0F;

  // set the required bits
  for(uint8_t i = 7; i > 7-lives; i--) {
    PORTA |= (1 << i);
  }
}
