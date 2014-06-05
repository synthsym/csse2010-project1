/*
 * timing.c
 *
 *  Created on: 05/06/2014
 *      Author: adam
 */

#include "timing.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t count = 0;
volatile uint8_t remain = 0;
uint8_t ssd_data[10] = {63,6,91,79,102,109,125,7,127,111};
volatile uint8_t ssd_cc = 0;

void init_timer(void) {
  TCCR1B = (1 << WGM12); //CTC Mode, Timer is off
  OCR1A = 9999; //one tick per second
  TIMSK1 = (1 << OCIE1A); //trigger interrupt on compare match

  DDRC = 0xff;
  DDRA |= (1 << PINA2);
}

void start_timer(uint8_t time) {
  TCCR1B |= (0 << CS11); //stop the counter
  TCNT1 = 0; // reset the counter
  TCCR1B |= (1 << CS11); //start the counter, 64 prescale
  remain = time;
  display_time();
}

uint8_t timer_finished(void) {
  return (remain == 0);
}

void display_time(void) {
  PORTC = 0; //remove the last number
  if(ssd_cc) {
    if(remain > 9) {
      //display the tens digit
      PORTC = ssd_data[(remain/10)];
      PORTA |= (1 << PINA2);
    }
  } else {
    //display the ones digit
    PORTC = ssd_data[(remain%10)];
    PORTA &= ~4;
  }
  ssd_cc = 1 ^ ssd_cc;
}

ISR(TIMER1_COMPA_vect) {
  if(remain == 0) {
    TCCR1B |= (0 << CS11) | (0 << CS10); // turn off the timer, we don't want any overflow errors
    PORTC = 0;
  } else {
    count++;
    if(count == 1000) {
      remain--;
      count = 0;
    }
    display_time();
  }
}
