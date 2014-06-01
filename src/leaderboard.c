/*
 * leaderboard.c
 *
 *  Created on: 26/05/2014
 *      Author: adam
 */

#include "leaderboard.h"
#include <avr/eeprom.h>

uint8_t is_highscore(uint16_t score) {
  highscore scores[LIST_SIZE];
  eeprom_read_block(scores, 0, sizeof(highscore[LIST_SIZE]));
  for(uint8_t i = 0; i < LIST_SIZE; i++) {
    if(score >= temp[i].score) {
      return 1;
    }
  }

  return 0;
}

void update_highscores(char name[3], uint16_t score) {
  highscore new_score = {name, score};
  highscore new_scores[LIST_SIZE];
  highscore scores[LIST_SIZE];

  eeprom_read_block(scores, 0, sizeof(highscore[LIST_SIZE]));

  //insert the new score when we find a score that is less or equal
  uint8_t i;
  for(i = 0; i < LIST_SIZE; i++) {
    if(scores[i].score <= new_score.score) {
      new_scores[i] = new_score;
      break;
    } else {
      new_scores[i] = scores[i];
    }
  }

  // fill the last positions with stuff from the old leaderboard
  for(uint8_t j = i; j < LIST_SIZE-1; j++) {
    new_scores[j+1] = scores[j];
  }

  // update the eeprom
  eeprom_update_block(new_scores, 0, sizeof(new_scores));
}

void show_highscores(void) {

}
