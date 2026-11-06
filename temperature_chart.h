#ifndef CHART_
#define CHART_
#include <Arduino.h>

class chart {
  public:
    chart(int size);
    ~chart();
    void add_data_point(int);
    String get_data();
  private:
    int data_size;
    int position;
    int* data;
};
#endif