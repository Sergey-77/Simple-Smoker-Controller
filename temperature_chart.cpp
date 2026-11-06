#include "temperature_chart.h"

chart::chart(int size){
  position = 0;
  data_size = size;
  data = new int[data_size]();
}

chart::~chart(){
  delete[] data;
}

void chart::add_data_point(int temperature){
  position++;
  if (position > (data_size - 1))position = 0;
  data[position] = temperature;
}

String chart::get_data(){
  int position_tmp = position;
  String output_str;
  for(int i=0; i<data_size; i++){
    position_tmp++;
    if(position_tmp > (data_size - 1))position_tmp = 0;
    if(i == (data_size - 1)){
      output_str = output_str + String(data[position_tmp]);
    }
    else output_str = output_str + String(data[position_tmp]) + ',';
  }
  return output_str;
}