/*
 * FroggerProject.c
 *
 * Main file
 *
 * Author: Peter Sutton. Modified by <YOUR NAME HERE>
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>

#include "ledmatrix.h"
#include "scrolling_char_display.h"
#include "buttons.h"
#include "serialio.h"
#include "terminalio.h"
#include "score.h"
#include "lives.h"
#include "joystick.h"
#include "timer0.h"
#include "game.h"

#define F_CPU 8000000L
#include <util/delay.h>

// Function prototypes - these are defined below (after main()) in the order
// given here
void initialise_hardware(void);
void splash_screen(void);
void new_game(void);
void play_game(void);
void next_level(void);
void handle_game_over(void);

// Variables
static uint8_t level;
static uint8_t game_over;

// ASCII code for Escape character
#define ESCAPE_CHAR 27

/////////////////////////////// main //////////////////////////////////
int main(void) {
	// Setup hardware and call backs. This will turn on 
	// interrupts.
	initialise_hardware();
	
	// Show the splash screen message. Returns when display
	// is complete
	splash_screen();
	
	while(1) {
    new_game();
    while(!game_over) {
      if(is_supa_dead()) {
        handle_game_over();
      } else {
        next_level();
        play_game();
      }
    }
	}
}

void initialise_hardware(void) {
	ledmatrix_setup();
	init_button_interrupts();
	// Setup serial port for 19200 baud communication with no echo
	// of incoming characters
	init_serial_stdio(19200,0);
	
	init_timer0();
	
	init_joystick();

	// Turn on global interrupts
	sei();
}

void splash_screen(void) {
	// Clear terminal screen and output a message
	clear_terminal();
	move_cursor(10,10);
	printf_P(PSTR("Frogger"));
	move_cursor(10,12);
	printf_P(PSTR("CSSE2010/7201 project by Adam Jacobus"));
	
	// Output the scrolling message to the LED matrix
	// and wait for a push button to be pushed.
	ledmatrix_clear();
	while(1) {
	  set_text_colour(COLOUR_ORANGE);
		set_scrolling_display_text("FROGGER");
		// Scroll the message until it has scrolled off the 
		// display or a button is pushed
		while(scroll_display()) {
			_delay_ms(150);
			if(button_pushed() != -1) {
				return;
			}
		}

		set_text_colour(COLOUR_GREEN);
		set_scrolling_display_text("43585746");
		// Scroll the message until it has scrolled off the
    // display or a button is pushed
    while(scroll_display()) {
      _delay_ms(150);
      if(button_pushed() != -1) {
        return;
      }
    }
	}
}

void new_game(void) {
	// Set initial level
	level = 0;
	game_over = 0;

	// Clear the serial terminal
	clear_terminal();
	
	// Initialise the score
	init_score();
	
  // give three lives
  init_lives();

	// Clear a button push or serial input if any are waiting
	// (The cast to void means the return value is ignored.)
	(void)button_pushed();
	clear_serial_input_buffer();
}

void play_game(void) {
	uint32_t current_time;
	int8_t button;
	int8_t joystick;
	char serial_input, escape_sequence_char;
	uint8_t characters_into_escape_sequence = 0;
	uint8_t game_paused = 0;
	
	// Get the current time and remember this as the last time the vehicles
	// and logs were moved.
	current_time = get_clock_ticks();
	
	init_game();
	clear_terminal();

	// We play the game while the frog is alive and we haven't filled up the 
	// far riverbank
	while(!is_supa_dead() && !is_riverbank_full()) {
		if(is_frog_alive() && frog_has_reached_riverbank()) {
			// Frog reached the other side successfully but the
			// riverbank isn't full, put a new frog at the start
			put_frog_at_start();
		}
		
		// remove a life if the frog dies
		if(!is_frog_alive()) {
		  remove_life();
		  put_frog_at_start();
		}

		// Check for input - which could be a button push or serial input.
		// Serial input may be part of an escape sequence, e.g. ESC [ D
		// is a left cursor key press. At most one of the following three
		// variables will be set to a value other than -1 if input is available.
		// (We don't initalise button to -1 since button_pushed() will return -1
		// if no button pushes are waiting to be returned.)
		// Button pushes take priority over serial input. If there are both then
		// we'll retrieve the serial input the next time through this loop
		serial_input = -1;
		escape_sequence_char = -1;
		button = button_pushed();
		
		if(button == -1) {
			// No push button was pushed, see if there is any serial input
			if(serial_input_available()) {
				// Serial data was available - read the data from standard input
				serial_input = fgetc(stdin);
				// Check if the character is part of an escape sequence
				if(characters_into_escape_sequence == 0 && serial_input == ESCAPE_CHAR) {
					// We've hit the first character in an escape sequence (escape)
					characters_into_escape_sequence++;
					serial_input = -1; // Don't further process this character
				} else if(characters_into_escape_sequence == 1 && serial_input == '[') {
					// We've hit the second character in an escape sequence
					characters_into_escape_sequence++;
					serial_input = -1; // Don't further process this character
				} else if(characters_into_escape_sequence == 2) {
					// Third (and last) character in the escape sequence
					escape_sequence_char = serial_input;
					serial_input = -1;  // Don't further process this character - we
										// deal with it as part of the escape sequence
					characters_into_escape_sequence = 0;
				} else {
					// Character was not part of an escape sequence (or we received
					// an invalid second character in the sequence). We'll process 
					// the data in the serial_input variable.
					characters_into_escape_sequence = 0;
				}
			}
		}
		
		joystick = joystick_direction();

    // Process the input only if the game isn't paused.
    if(!game_paused) {
      if(button==3 || escape_sequence_char=='D' || serial_input=='L' || serial_input=='l' || joystick==3) {
        // Attempt to move left
        move_frog_left();
      } else if(button==2 || escape_sequence_char=='A' || serial_input=='U' || serial_input=='u' || joystick==2) {
        // Attempt to move forward
        move_frog_forward();
      } else if(button==1 || escape_sequence_char=='B' || serial_input=='D' || serial_input=='d' || joystick==1) {
        // Attempt to move down
        move_frog_backward();
      } else if(button==0 || escape_sequence_char=='C' || serial_input=='R' || serial_input=='r' || joystick==0) {
        // Attempt to move right
        move_frog_right();
      }
      // else - invalid input or we're part way through an escape sequence -
      // do nothing
    }

		// Deal with the pause game state
		if(serial_input == 'p' || serial_input == 'P') {
      if(game_paused) {
        game_paused = 0;
        clear_terminal();
      } else {
        game_paused = 1;
        move_cursor(10,14);
        printf_P(PSTR("Game Paused!"));

      }
    }
		// else - invalid input or we're part way through an escape sequence -
		// do nothing
		
		current_time = get_clock_ticks();
		if(is_frog_alive() && !game_paused) {
		  if(current_time % (1000 + (50 * level)) == 0) {
        scroll_log_channel(1, 1);
		  }

		  if(current_time % (1100 + (50 * level)) == 0) {
		    scroll_lane(1, -1);
		  }

		  if(current_time % (1200 + (50 * level)) == 0) {
		    scroll_lane(0, 1);
		  }

		  if(current_time % (800 + (50 * level)) == 0) {
		    scroll_lane(2, 1);
		  }

		  if(current_time % (600 + (50 * level)) == 0) {
		    scroll_log_channel(0, -1);
		  }
		}

		// display the number of lives
		display_lives();
	}

	// We get here if the frog is supa-dead (has no more lives) or the riverbank is full
	// The game is over.
}

void next_level() {
  level++;
  if(level > 1) {
    add_life();
  }

  clear_terminal();
  move_cursor(10,15);
  printf_P(PSTR("Level %i"), level);
  move_cursor(10,16);
  printf_P(PSTR("Score: %i"), get_score());

  ledmatrix_clear();
  set_text_colour(COLOUR_RED);
  char level_txt[8];
  sprintf(level_txt, "LEVEL %i", level);
  set_scrolling_display_text(level_txt);

  while(scroll_display()) {
    _delay_ms(150);
    if(button_pushed() != -1) {
      return;
    }
  }
}

void handle_game_over() {
  game_over = 1;

  clear_terminal();
	move_cursor(10,14);
	printf_P(PSTR("GAME OVER"));
	move_cursor(10,15);
	printf_P(PSTR("SCORE %i"), get_score());
	move_cursor(10,16);
	printf_P(PSTR("Press a button to start again"));
	
	ledmatrix_clear();
	set_text_colour(COLOUR_GREEN);
	while(1) {
    set_scrolling_display_text("GAME OVER");
    // Scroll the message until it has scrolled off the
    // display or a button is pushed
    while(scroll_display()) {
      _delay_ms(150);
      if(button_pushed() != -1) {
        return;
      }
    }
  }
}
