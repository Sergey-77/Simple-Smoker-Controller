#ifndef SMOKER_
#define SMOKER_

extern unsigned long startTime;
extern unsigned long elapsedTime;
extern unsigned long millis_buffer;
extern unsigned long pwm_on_phase;
extern unsigned long pwm_start;
extern unsigned long temp_update_time;
extern unsigned long pid_update_time;
extern int smoker_temp_setpoint_tmp;
extern int cooking_time_hours_tmp;
extern int cooking_time_minutes_tmp;
extern int cleaning_cycle_tmp;
extern int smoker_temp_setpoint;
extern int cooking_time_hours;
extern int cooking_time_minutes;
extern int elapsed_time_h;
extern int elapsed_time_m;
extern int smoker_temp_settings;
extern int run;
extern int running;
extern int start_button_validation;
extern float thermocouple;
extern int pwm_duty_cycle;

unsigned long get_pwm_duty_cycle_length();

#endif