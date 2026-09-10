#include "heater_control.h"
#include "settings_eeprom.h"
#include <cmath>

int heater_controll_pid(int setpoint, float temperature, int first_run){
	int pwm;
	static int preheated;
	static float errPrev, errSum;
	float errSum2;
	int index = (setpoint - 125)/25;
  if(index < 0 || index > 16)index = 0;
  int offset = settings.pwm_preset[index];
	float error = setpoint - temperature;
	if(first_run){
		errPrev = error;
		preheated = 0;
		errSum = errSum2 = 0;
	}
	float delta = error - errPrev;
	if((abs(delta) < 1 && temperature > setpoint - 15) || preheated){
		errSum2 = settings.Ki * error;
		preheated = 1;
	}else{
		errSum2 = 0;
	}
	errPrev = error;
	pwm = offset + settings.Kp * error + errSum + errSum2 + settings.Kd * delta;
	if(pwm > 100){
		pwm = 100;
		if(errSum2 > 0){
			errSum2 = 0;
		}
	}
	if(pwm < 0){
		pwm = 0;
		if(errSum2 < 0){
			errSum2 = 0;
		}
	}
	errSum += errSum2;
	if(pwm < 5){
		pwm = 0;
	}
	if(temperature > 600 || temperature < 0) pwm = 0;
	if(settings.temp_high > 0 && temperature > setpoint + settings.temp_high) pwm = 0;
	return pwm;
}

/*
type_heater_presets heater_presets[17] = {
	{125,19.35},
	{150,25.00}, //not in simple smoker
	{175,29.03},
	{200,35.00}, //not in simple smoker
	{225,38.71},
	{250,45.16},
	{275,50.00},
	{300,54.84},
	{325,59.38},
	{350,64.52},
	{375,68.75},
	{400,74.19},
	{425,78.13},
	{450,83.87},
	{475,90.32},
	{500,93.55},
	{525,100.00}
};
*/