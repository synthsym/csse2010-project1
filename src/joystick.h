/*
 * joystick.h
 *
 *  Created on: 24/05/2014
 *      Author: adam
 */

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <stdint.h>

void init_joystick(void);
void measure_joystick(void);
int8_t joystick_direction(void);

#endif /* JOYSTICK_H_ */
