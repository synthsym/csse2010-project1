/*
 * lives.h
 *
 *  Created on: 20/05/2014
 *      Author: adam
 */

#ifndef LIVES_H_
#define LIVES_H_

#include <stdint.h>

void init_lives(void);
void add_life(void);
void remove_life(void);
uint8_t get_lives(void);
void display_lives(void);

#endif /* LIVES_H_ */
