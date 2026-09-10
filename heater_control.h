#ifndef HEATER_CONTROL_
#define HEATER_CONTROL_

int heater_controll_pid(int setpoint, float temperature, int first_run = 0);

#endif