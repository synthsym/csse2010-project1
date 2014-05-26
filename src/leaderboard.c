/*
 * leaderboard.c
 *
 *  Created on: 26/05/2014
 *      Author: adam
 */

#include "leaderboard.h"
#include <avr/eeprom.h>

uint8_t is_highscore(uint16_t score) {
  for(uint8_t i = 1; i <= LIST_SIZE; i++) {
    highscore temp = read_highscore(i);
    if(temp.valid) {
      if(score >= temp.score) {
        return 1;
      }
    }
  }

  return 0;
}

void update_highscores(char name[3], uint16_t score) {
  highscore new_score = {SIGNATURE_BYTE, name, score};

}

void show_highscores(void) {

}

void write_highscore(uint8_t pos, highscore score) {
  eeprom_update_block(score, pos-1, sizeof(highscore));
}

highscore read_highscore(uint8_t pos) {
  highscore temp;
  eeprom_read_block(temp, pos-1, sizeof(highscore));
}

