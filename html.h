#ifndef HTML_
#define HTML_

#include <Arduino.h>
#include "smoker.h"

class page_html {
  public:
    static const String css;
    static const String end;
};

class main_page: public page_html{
  public:
    String html();
	  void extractAndSetTemperature(String request);
	  void extractAndSetTimer(String request);
};

class settings_page: public page_html{
  public:
    String html();
	  void extractAndSetParameters(String request);
};

class wifi_page: public page_html{
  public:
    String html();
    void process_request(String request);
  private:
    String urlDecode(String eString);
};

class data {
  public:
   String html();
};
#endif