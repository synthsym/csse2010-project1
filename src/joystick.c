/*
 * joystick.c
 *
 *  Created on: 24/05/2014
 *      Author: adam
 */

#include "joystick.h"
#include <avr/io.h>
#include "timer0.h"

static uint16_t xvalue;
static uint16_t yvalue;
static uint32_t prev_t;
static uint8_t p_dir;

void init_joystick(void) {
  ADMUX = (1<<REFS0);
  ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1);
}

void measure_joystick(void) {
  // get joystick x data
  ADMUX &= ~1;
  ADCSRA |= (1<<ADSC);
  while(ADCSRA & (1<<ADSC)) {
    ; /* Wait until conversion finished */
  }
  xvalue = ADC;

  // get joystick y data
  ADMUX |= 1;
  ADCSRA |= (1<<ADSC);
  while(ADCSRA & (1<<ADSC)) {
    ; /* Wait until conversion finished */
  }
  yvalue = ADC;
}

int8_t joystick_direction(void) {
  /*
   *  3 - Left
   *  2 - Forward
   *  1 - Backward
   *  0 - Right
   *  -1 - Rest
   */
  uint8_t dir;
  uint32_t t;
  measure_joystick();

  if(xvalue < 100) {
    dir = 3;
  } else if(yvalue > 850) {
    dir = 2;
  } else if(yvalue < 150) {
    dir = 1;
  } else if(xvalue > 800) {
    dir = 0;
  } else {
    dir = -1;
  }

  if(dir >= 0) { //If a direction has been selected, not rest
    if (p_dir == dir) { //If the current direction is the same as the last direction
      t = get_clock_ticks();
      if(t < prev_t + 500) { // Has 500ms elapsed since last direction
        return -1;
      }
    }

    p_dir = dir;
    prev_t = get_clock_ticks();
    return dir;
  } else {
    return dir; // dir is -1
  }
}
