// NodeMCU ESP8266 Board
/*
    Press flash button for 3 seconds after blue light blink at power up or reset to get Access Point WIFI.
    If button was pressed for 10 seconds default "smoker" AP WIFI will be started. No password needed.
    IP for AP will be 192.168.4.1
*/

/*
The code needs to be added to OneWire.h for Arduino Nano ESP32 support

#elif defined(ARDUINO_ARCH_ESP32)
#define PIN_TO_BASEREG(pin) ((volatile uint32_t *)((digitalPinToGPIONumber(pin)>31)?1:0))
#define PIN_TO_BITMASK(pin) (1 << (digitalPinToGPIONumber(pin)&31))
#define IO_REG_TYPE uint32_t
#define IO_REG_ASM
#define DIRECT_READ(base, mask) ((*((volatile uint32_t *)((base)?GPIO_IN1_REG:GPIO_IN_REG)) & (mask)) ? 1 : 0) // GPIO_IN_ADDRESS
#define DIRECT_MODE_INPUT(base, mask) (*((volatile uint32_t *)((base)?GPIO_ENABLE1_W1TC_REG:GPIO_ENABLE_W1TC_REG)) = (mask)) // GPIO_ENABLE_W1TC_ADDRESS
#define DIRECT_MODE_OUTPUT(base, mask) (*((volatile uint32_t *)((base)?GPIO_ENABLE1_W1TS_REG:GPIO_ENABLE_W1TS_REG)) = (mask)) // GPIO_ENABLE_W1TS_ADDRESS
#define DIRECT_WRITE_LOW(base, mask) (*((volatile uint32_t *)((base)?GPIO_OUT1_W1TC_REG:GPIO_OUT_W1TC_REG)) = (mask)) // GPIO_OUT_W1TC_ADDRESS
#define DIRECT_WRITE_HIGH(base, mask) (*((volatile uint32_t *)((base)?GPIO_OUT1_W1TS_REG:GPIO_OUT_W1TS_REG)) = (mask)) // GPIO_OUT_W1TS_ADDRESS

*/


#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #define LED D4        // LED on ESP8266
  #define HEATER_PIN D1
  #define BUTTON_PIN 0  // flash button
#elif defined(ESP32)
  #include <WiFi.h>
  #define LED LED_BUILTIN
  #define HEATER_PIN D3
  #define BUTTON_PIN D2
#endif

//#include "smoker.h"
#include "settings_eeprom.h"
#include "html.h"
#include "heater_control.h"

#include "OneWire.h"
#include "DallasTemperature.h"

// Data wire conection from thermocouple module
#define ONE_WIRE_BUS D7

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
WiFiServer server(80);

main_page main_page;
settings_page settings_page;
wifi_page wifi_page;
data data;

#define PWM_LENGTH 32000

void led_on(){
  #if defined(ESP8266)
  digitalWrite(LED, LOW);
  #else
  digitalWrite(LED, HIGH);
  #endif
}

void led_off(){
  #if defined(ESP8266)
  digitalWrite(LED, HIGH);
  #else
  digitalWrite(LED, LOW);
  #endif  
}

void blink_ip(int ip){
  int ii;
  if (ip<10)ii = 2;
  else if (ip<100)ii = 1;
  for(; ii < 3; ii++){
    int i, zero;
    if(ii == 0 and ip>99){
      i = ip / 100;
      ip -= (i*100);
    }
    else if (ii == 1){
      if (ip>9){
        i = ip / 10;
        ip -= (i*10);
      }else zero = 1;
    }
    else if (ii == 2){
      if (ip){
         i = ip;
        ip = 0;
      }
      else zero =1;
    }
    for (;i>0;i--){
      led_on();
      delay(50);
      led_off();
      delay(500);
    }
     if (zero){
      led_on();
      delay(800);
      led_off();
      delay(500);
      zero = 0;
    }
    delay(2000);
  }
}
unsigned long get_pwm_duty_cycle_length(){
  int index = (smoker_temp_setpoint - 125)/25;
  if(index < 0 || index > 16)index = 0;
  int duty_cycle_preset = settings.pwm_preset[index];
  pwm_duty_cycle = duty_cycle_preset;
  return PWM_LENGTH * duty_cycle_preset / 100;
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  pinMode(HEATER_PIN, OUTPUT);
  led_off();
  get_settings_eeprom();
  delay(500);
  int button_pressed = 0;
  while(digitalRead(BUTTON_PIN) == LOW){
    delay(1000);
    button_pressed++;
  }
  if (button_pressed > 5) {
    WiFi.softAP("smoker");
  }
  else if (button_pressed > 0) {
    WiFi.softAP(settings.SSID_AP, settings.PASSWORD_AP);
  }
  else if(settings.SSID[0] != '\0'){
    WiFi.hostname("smoker");
    WiFi.mode(WIFI_STA);
    WiFi.begin (settings.SSID, settings.PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    // LED blink last three digits of IP 
    IPAddress ip = WiFi.localIP();
    blink_ip(ip[3]);
  }
  
  else if (settings.SSID_AP[0] != '\0') {
    WiFi.softAP(settings.SSID_AP, settings.PASSWORD_AP);
  }
  else{
    WiFi.softAP("smoker");
  } 
  server.begin(); 
  pwm_on_phase = get_pwm_duty_cycle_length();
  sensors.begin();
}

void loop() {
  WiFiClient client = server.accept();
  if (client) {
  String request = client.readStringUntil('\r');
  if (request.indexOf("temperature=") != -1){
    main_page.extractAndSetTemperature(request);
    pwm_on_phase = get_pwm_duty_cycle_length();
    client.print(main_page.html());
  }
  else if (request.indexOf("cooking_time_h=") != -1){
    main_page.extractAndSetTimer(request); 
    client.print(main_page.html());
  }
  else if (request.indexOf("setup") != -1){
    settings_page.extractAndSetParameters(request);
    client.print(settings_page.html());
  }
  else if ((request.indexOf("clean_cycle") != -1) && running == 0){
    if(cleaning_cycle_tmp){
      smoker_temp_setpoint = smoker_temp_setpoint_tmp;
      cooking_time_hours = cooking_time_hours_tmp;
      cooking_time_minutes = cooking_time_minutes_tmp;
    }
    else{
      smoker_temp_setpoint_tmp = smoker_temp_setpoint;
      cooking_time_hours_tmp = cooking_time_hours;
      cooking_time_minutes_tmp = cooking_time_minutes;
      smoker_temp_setpoint = 525;
      cooking_time_hours = 0;
      cooking_time_minutes = 30;
    }
    cleaning_cycle_tmp ^= 1;
    pwm_on_phase = get_pwm_duty_cycle_length();
    client.print(main_page.html());
  }
  else if (request.indexOf("start") != -1){
    int index = request.indexOf("start");
    char validation[3];
    validation[2] = '\0';
    validation[0] = request[index + 6];
    validation[1] = request[index + 7];
    if(start_button_validation == atoi(validation)){
      start_button_validation++;
      if(start_button_validation>99)start_button_validation=0;
      run ^=1;
      if(run == 0){
        running = 0;
      }
    }
    client.print(main_page.html());
  }
  else if (request.indexOf("GET /data.html") != -1){
    client.print(data.html());
  }
  else if (request.indexOf("wifi") != -1){
    wifi_page.process_request(request);
    client.print(wifi_page.html());
  }
  else {
    client.print(main_page.html());
  }
  }
  if(run == 1 and running == 1 ){
    elapsedTime = (millis() - startTime);
    elapsed_time_h = elapsedTime/3600000;
    elapsed_time_m = (elapsedTime - (elapsed_time_h * 3600000))/60000;
    if(elapsed_time_h > cooking_time_hours ){
      run = 0;
      running = 0;
    }
    if(elapsed_time_h == cooking_time_hours and elapsed_time_m >= cooking_time_minutes){
      run = 0;
      running = 0;
    }
  }
  if(run == 1 and running == 0 ){
    startTime = millis();
    running = 1;
    if(settings.pid_enabled){
      heater_controll_pid(smoker_temp_setpoint, thermocouple, 1);
    }
  }
  if(running){
    millis_buffer = millis() - pwm_start;
    if(millis_buffer > PWM_LENGTH){
      pwm_start = millis();
      millis_buffer = 0;
    }
    if(millis_buffer < pwm_on_phase){
      led_on();
      digitalWrite(HEATER_PIN, HIGH);
    }
    else{
      led_off();
      digitalWrite(HEATER_PIN, LOW);
    }
  }
  else {
    led_off();
    digitalWrite(HEATER_PIN, LOW);
  }
  if(millis() > temp_update_time){
    sensors.requestTemperatures();
    thermocouple = sensors.getTempCByIndex(0)*1.8 + 32; //with celsius to fahrenheit conversion
    if(isnan(thermocouple))thermocouple = -127;
    temp_update_time = millis() + 5000;
  }
  if(settings.pid_enabled && running && (millis() > pid_update_time)){
    pwm_duty_cycle = heater_controll_pid(smoker_temp_setpoint, thermocouple);
    pwm_on_phase = PWM_LENGTH * pwm_duty_cycle / 100;
    pid_update_time = millis() + 30000;
  }  
}

