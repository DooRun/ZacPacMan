
#include "Arduino.h"
#include "FlickerController.h"

FlickerController::FlickerController(int *pins_in, int number_of_pins_in) {
  pins = pins_in;
  number_of_pins = number_of_pins_in;
  current_states = new boolean[number_of_pins];
}

void FlickerController::setup_controller() {
  
  // Set the random seed
  int random_seed = 0;
  for (int i = 0; i < 5; i++) {
    random_seed += analogRead(A0);
  }
  randomSeed(analogRead(0));

  // Set pinmodes
  for (int i = 0; i < number_of_pins; i++) {
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], LOW);
  }

  // Setup constants that aren't exposed to the constructor (but could be)
  startup_time_duration_ms = 7000;
  flicker_check_period_ms = 25;  // was 12
  initial_flicker_on_threshold = 0.07;
  initial_flicker_off_threshold = 1;
  tau_on = 0.00215;
  tau_off = 0.00080;

}

void FlickerController::do_flicker(boolean end_in_on_state) {

  // Setup time variables here
  current_flicker_start_time = millis();
  long startup_time_end_ms =  current_flicker_start_time + startup_time_duration_ms;
  turn_on_inflection_time = startup_time_end_ms - (startup_time_duration_ms / 2);
  long last_switch_time = 0;
  

  while(true) {
  
    long current_time_ms = millis();

    // If we are done starting up, then turn things on and mark startup as complete
    if (current_time_ms > startup_time_end_ms) {
      for (int i = 0; i < number_of_pins; i++) {
        digitalWrite(pins[i], end_in_on_state);
      }
      break;
    }
  
    // Uncomment these if you want to see thresholds printed
    // Serial.print(get_current_flicker_on_threshold());
    // Serial.print(",");
    // Serial.println(get_current_flicker_off_threshold());
    
    // We are still in startup. See if it's time to test for a state switch
    if (current_time_ms - last_switch_time > flicker_check_period_ms) {
      last_switch_time = current_time_ms;
      randomly_adjust_all_states(end_in_on_state);
      
    }
  }
}

void FlickerController::randomly_adjust_all_states(boolean end_in_on_state) {
  for (int i = 0; i < number_of_pins; i++)
  {
    double random_seed = random(1, 1000) / 1000.0;
    // Probably need to change the line below to make flicker off work
    if (current_states[i]) {
      // It is already on
      if (random_seed < get_threshold(!end_in_on_state)) {
        set_state(false, i);
        tone(13, 9000); delay(15); noTone(13);
      }
    } else {
      // It is already off
      if (random_seed < get_threshold(end_in_on_state)) {
        set_state(true, i);
        tone(13, 9000); delay(15); noTone(13);
      }
    }
  }
}

double FlickerController::get_threshold(boolean is_turn_on) {
  long current_time = millis();
  if (is_turn_on) {
    long argument = current_time - turn_on_inflection_time;
    return initial_flicker_on_threshold + (1 - initial_flicker_on_threshold) / (1 + exp(-argument * tau_on));
  } else {
    return initial_flicker_off_threshold * exp(-(current_time - current_flicker_start_time) * tau_off);
  }
}


void FlickerController::set_state(boolean state, int pin) {
  digitalWrite(pins[pin], state);
  current_states[pin] = state;
}
