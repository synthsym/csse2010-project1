/*
 * timing.h
 *
 *  Created on: 05/06/2014
 *      Author: adam
 */

#ifndef TIMING_H_
#define TIMING_H_

#include <stdint.h>

void init_timer(void);
void start_timer(uint8_t time);
uint8_t timer_finished(void);
void display_time(void);

#endif /* TIMING_H_ */
