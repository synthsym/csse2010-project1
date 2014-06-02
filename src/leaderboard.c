/*
 * leaderboard.c
 *
 *  Created on: 26/05/2014
 *      Author: adam
 */

#include "leaderboard.h"
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include "terminalio.h"
#include <stdio.h>

uint8_t is_highscore(uint16_t score) {
  highscore scores[LIST_SIZE];
  eeprom_read_block(scores, 0, sizeof(highscore[LIST_SIZE]));

  for(uint8_t i = 0; i < LIST_SIZE; i++) {
    //if the score is higher or equal, or is empty
    if(score >= scores[i].score || scores[i].score == -1) {
      return 1;
    }
  }

  return 0;
}

void update_highscores(uint8_t name[3], uint16_t score) {
  highscore new_score;
  for(uint8_t i = 0; i < 3; i++) {
    new_score.name[i] = *name;
    name++;
  }
  new_score.score = score;
  highscore new_scores[LIST_SIZE];
  highscore scores[LIST_SIZE];

  eeprom_read_block(scores, 0, sizeof(scores));

  //insert the new score into the scores array
  uint8_t i;
  for(i = 0; i < LIST_SIZE; i++) {
    // if the score is equal or higher, or the score is empty
    if(scores[i].score <= new_score.score || scores[i].score == -1) {
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

void show_highscores(uint8_t pos) {
  highscore scores[LIST_SIZE];
  eeprom_read_block(scores, 0, sizeof(scores));

  move_cursor(10, pos);
  printf_P(PSTR("HIGHSCORES"));
  uint8_t line = pos+2;

  for(uint8_t i = 0; i < LIST_SIZE; i++) {
    if(scores[i].score != -1) {
      line++;
      move_cursor(10, line);
      printf_P(PSTR("%c%c%c  %i"), scores[i].name[0], scores[i].name[1], scores[i].name[2], scores[i].score);
    }
  }
}
