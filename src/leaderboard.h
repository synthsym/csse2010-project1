/*
 * leaderboard.h
 *
 *  Created on: 26/05/2014
 *      Author: adam
 */

#ifndef LEADERBOARD_H_
#define LEADERBOARD_H_

#include <stdint.h>

#define SIGNATURE_BYTE 0b110011100
#define LIST_SIZE 5

typedef struct {
  uint8_t valid;
  char* name;
  uint16_t score;
} highscore;

void write_highscore(highscore);
highscore read_highscore(uint8_t position);

#endif /* LEADERBOARD_H_ */
