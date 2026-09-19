#ifndef SETTINGS_EEPROM_
#define SETTINGS_EEPROM_

struct settings_struct{ 
		int MEMORY_INITIALIZED;
    char SSID[64] = "";
    char PASSWORD[64] = "";
		char SSID_AP[64] = "";
    char PASSWORD_AP[64] = "";
		int pid_enabled;
		float Kp;
		float Ki;
		float Kd;
		int temp_high;
		int pwm_preset[17];
};

extern settings_struct settings;

void get_settings_eeprom();
void store_settings_eeprom();

#endif