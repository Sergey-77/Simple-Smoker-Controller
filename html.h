#ifndef HTML_
#define HTML_

#include <Arduino.h>
#include "smoker.h"

class page_html {
  public:
    static const String css;
    static const String end;
    float extractValueFromString(int position, int max_index, String str);
};

class MainPage: public page_html{
  public:
    MainPage();
    String html();
	  void extractAndSetTemperature(String request);
	  void extractAndSetTimer(String request);
  private:
    String html_body1;
    int start_validation_position;
    int start_button_text_position;
    int start_select_position;
};

class SettingsPage: public page_html{
  public:
    String html();
	  void extractAndSetParameters(String request);
};

class WifiPage: public page_html{
  public:
    String html();
    void process_request(String request);
  private:
    String urlDecode(String eString);
};

class Data {
  public:
   String html();
};
#endif