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
  uint8_t name[3];
  uint16_t score;
} highscore;

uint8_t is_highscore(uint16_t score);
void update_highscores(uint8_t name[3], uint16_t score);
void show_highscores(uint8_t pos);

#endif /* LEADERBOARD_H_ */
