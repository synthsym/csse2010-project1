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
  char name[3];
  uint16_t score;
} highscore;

uint8_t is_highscore(uint16_t score);
void update_highscores(char* name, uint16_t score);
void show_highscores(void);
void write_highscore(uint8_t pos, highscore score);
highscore read_highscore(uint8_t pos);

#endif /* LEADERBOARD_H_ */
