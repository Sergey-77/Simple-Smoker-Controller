#include "smoker.h"

unsigned long startTime;
unsigned long elapsedTime;
unsigned long millis_buffer;
unsigned long pwm_on_phase;
unsigned long pwm_start;
unsigned long temp_update_time;
unsigned long pid_update_time;
int smoker_temp_setpoint_tmp;
int cooking_time_hours_tmp;
int cooking_time_minutes_tmp;
int cleaning_cycle_tmp;
int smoker_temp_setpoint = 250;
int cooking_time_hours = 2;
int cooking_time_minutes = 0;
int elapsed_time_h;
int elapsed_time_m;
int smoker_temp_settings;
int run = 0;
int running;
int start_button_validation = 0;
float thermocouple;
int pwm_duty_cycle;
