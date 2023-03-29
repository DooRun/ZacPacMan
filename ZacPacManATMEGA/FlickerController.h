
/*
* 
* 
*/

#ifndef FlickerController_h
#define FlickerController_h

#include "Arduino.h"

class FlickerController {

    private:
      double initial_flicker_on_threshold;
      double initial_flicker_off_threshold;
      double tau_on;
      double tau_off;
    
      boolean *current_states;
      int *pins;
      int number_of_pins;
      boolean is_starting_up;
      long startup_time_duration_ms;
      long flicker_check_period_ms;
      long turn_on_inflection_time;
      long current_flicker_start_time;

    public:
      FlickerController(int *pins_in, int number_of_pins);
      void setup_controller();
      double get_threshold(boolean is_turn_on);
      void set_state(boolean state, int pin);
      void do_flicker(boolean end_in_on_state);
      void randomly_adjust_all_states(boolean end_in_on_state);
  
};

#endif
