#include "settings_eeprom.h"
#include <EEPROM.h>
#include <cstdio>

settings_struct settings;

void get_settings_eeprom(){
	EEPROM.begin(sizeof(settings));
	EEPROM.get(0, settings);
	if(settings.MEMORY_INITIALIZED != 1){
		settings.MEMORY_INITIALIZED = 1;
		sprintf(settings.SSID, "");
		sprintf(settings.PASSWORD, "");
		sprintf(settings.SSID_AP, "");
		sprintf(settings.PASSWORD_AP, "");
		settings.pid_enabled = 0;
		settings.Kp = 0.5;
		settings.Ki = 0.03;
		settings.Kd = 0.4;
		settings.temp_high = 0;
		settings.pwm_preset[0] = 20;
		settings.pwm_preset[1] = 25;
		settings.pwm_preset[2] = 30;
		settings.pwm_preset[3] = 35;
		settings.pwm_preset[4] = 40;
		settings.pwm_preset[5] = 45;
		settings.pwm_preset[6] = 50;
		settings.pwm_preset[7] = 55;
		settings.pwm_preset[8] = 60;
		settings.pwm_preset[9] = 65;
		settings.pwm_preset[10] = 70;
		settings.pwm_preset[11] = 75;
		settings.pwm_preset[12] = 80;
		settings.pwm_preset[13] = 85;
		settings.pwm_preset[14] = 90;
		settings.pwm_preset[15] = 95;
		settings.pwm_preset[16] = 100;
		EEPROM.put(0, settings); 
		EEPROM.commit();
	}
}

void store_settings_eeprom(){
	EEPROM.put(0, settings); 
	EEPROM.commit();
}
