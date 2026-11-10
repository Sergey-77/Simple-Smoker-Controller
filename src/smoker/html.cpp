#include "html.h"
#include "heater_control.h"
#include "settings_eeprom.h"
#include "temperature_chart.h"

extern chart temperature_chart;
const String page_html::css = 
#include "css.h"
    ;

const String page_html::end =
      "</html>"
    ;

float page_html::extractValueFromString(int position, int size, String str){
  char temp[5];
  temp[4]='\0';
  int str_pos;
  for(int i=0; i<4; i++){
    str_pos = position + i;
    if(str_pos >= size){temp[i]='\0';break;}
    temp[i] = str[str_pos];
    if(!(std::isdigit(temp[i]) || temp[i]=='.')){temp[i]='\0';break;}
  }
  return atof(temp);
}

MainPage::MainPage(){
  html_body1 = css + R""(<div class="header">Smoker Controller</div>
<div class="column_c">
<div class="column_a">
<div style="height: 185px"></div>
<div>
<form action="">
<label for="temperature">Smoker Temperature (&#176F):</label>
<form method="post">
<select name="temperature" id="temperature" onchange='if(this.value != 0) { this.form.submit(); }'>
<option value="125">125</option>
<option value="150">150</option>
<option value="175">175</option>
<option value="200">200</option>
<option value="225">225</option>
<option value="250">250</option>
<option value="275">275</option>
<option value="300">300</option>
<option value="325">325</option>
<option value="350">350</option>
<option value="375">375</option>
<option value="400">400</option>
<option value="425">425</option>
<option value="450">450</option>
<option value="475">475</option>
<option value="500">500</option>
<option value="525">525</option>
</select>
<span id="thermocouple"></span>
<output id="pwm">00</output>%
</form>
</div>
<div>
<form action="">
<label for="cooking_time_h">Cooking Time: </label>
<select name="cooking_time_h" id="cooking_time_h" onchange='this.form.submit()'>
<option value= "0">0</option>
<option value= "1">1</option>
<option value= "2">2</option>
<option value= "3">3</option>
<option value= "4">4</option>
<option value= "5">5</option>
<option value= "6">6</option>
<option value= "7">7</option>
<option value= "8">8</option>
<option value= "9">9</option>
<option value= "10">10</option>
<option value= "11">11</option>
<option value= "12">12</option>
<option value= "13">13</option>
<option value= "14">14</option>
<option value= "15">15</option>
<option value= "16">16</option>
<option value= "17">17</option>
<option value= "18">18</option>
<option value= "19">19</option>
<option value= "20">20</option>
<option value= "21">21</option>
<option value= "22">22</option>
<option value= "23">23</option>
<option value= "24">24</option>
</select> :
<select name="cooking_time_m" id="cooking_time_m" onchange='this.form.submit()'>
<option value ="0">0</option>
<option value ="5">5</option>
<option value ="10">10</option>
<option value ="15">15</option>
<option value ="20">20</option>
<option value ="25">25</option>
<option value ="30">30</option>
<option value ="35">35</option>
<option value ="40">40</option>
<option value ="45">45</option>
<option value ="50">50</option>
<option value ="55">55</option>
</select>
</form>
</div>
<div>
Elapsed time:  <output id="elapsed_time_h">0</output> : <output id="elapsed_time_m">00</output>
</div>
</div>
<form action="clean_cycle" method="post">
<input type="submit" value="Clean Cycle" class="Button">
</form>
<form action="setup" method="post">
<input type="submit" value="Settings" class="Button">
</form>
<form action="start_?S" method="post">
<input type="submit" id="start_button" class="Button" value="St?rt" style="color:#90EE90">
</form>
<span id="error"></span>
</div>
<div style="position: absolute; top:94px; left: 50%;transform: translateX(-50%);">
<table style="width:394px; font-size: 11px; font-family: sans-serif; border-collapse: collapse;">
<tr>
<td style="position: relative; width:34px; height:150px; padding:0;">
<span style="position: absolute; top:0px; right:2px;">400&deg;F</span>
<span style="position: absolute; top:45px; right:2px;">300&deg;F</span>
<span style="position: absolute; top:95px; right:2px;">200&deg;F</span>
<span style="position: absolute; top:140px; right:2px;">100&deg;F</span>
</td>
<td style="vertical-align: top; text-align: left;  padding: 0;"><canvas id="myCanvas" width="720" height="300" style="border:1px solid grey; width:360px; height:150px; background-color: white; border-radius: 8px;"></canvas></td>
</tr>
<tr>
<td></td>
<td style="position: relative; width:360px; height:12px; padding:0; vertical-align: top;">
<span style="position: absolute; left: 0%; transform: translateX(0%);">60</span> 
<span style="position: absolute; left: 25%; transform: translateX(-50%);">45</span> 
<span style="position: absolute; left: 50%; transform: translateX(-50%);">30</span> 
<span style="position: absolute; left: 75%; transform: translateX(-50%);">15</span>
</td>	
</tr>
</table>
</div>
<script>
selection=[["temperature","?te"],["cooking_time_h","00"],["cooking_time_m","00"],["elapsed_time_h","00"],["elapsed_time_m","00"],["pwm","000"]];for(let i=0;i<6;i++){document.getElementById(selection[i][0]).value=selection[i][1];}
const c = document.getElementById("myCanvas");
const ctx = c.getContext("2d");
ctx.transform(1, 0, 0, -1, 0, c.height);
var blob = new Blob(["function timedCount() {postMessage(0);setTimeout('timedCount()',5000);}timedCount();"]);
var blobURL = window.URL.createObjectURL(blob);
var w;
function fetchData() {
fetch('/data.html')
.then(
function(response){
if(response.status !=200){
return;
}
response.text().then(function (text) {
var data = text.split(",");
document.getElementById("temperature").value = data[0].toString();
document.getElementById("cooking_time_h").value = data[1].toString();
document.getElementById("cooking_time_m").value = data[2].toString();
b = document.getElementById("start_button");
if(data[3]==1){
b.value = "Stop";
b.style.color = "#FF5733";
}
else{
b.value = "Start";
b.style.color = "lightgreen";
}
document.getElementById("elapsed_time_h").value = data[4].toString();
document.getElementById("elapsed_time_m").value = data[5] > 9 ? data[5].toString() : '0' + data[5].toString();
if(data[6] != -127)document.getElementById("thermocouple").innerHTML = data[6] > 99 ? data[6].toString() : '0' + data[6].toString();
document.getElementById("pwm").innerHTML = data[7] > 9 ? data[7].toString() : '0' + data[7].toString();
ctx.clearRect(0, 0, c.width, c.height);
grid_array = [
[0, 99.5, 718, 99.5],
[0, 199.5, 718, 199.5],
[179.5, 0, 179.5, 300],
[359.5, 0, 359.5, 300],
[539.5, 0, 539.5, 300]
];
ctx.lineWidth = 1;
ctx.strokeStyle = "lightgray";
for (let i = 0; i < 5; i++){
ctx.stroke();
ctx.beginPath();
ctx.moveTo(grid_array[i][0], grid_array[i][1]);
ctx.lineTo(grid_array[i][2], grid_array[i][3]);
ctx.stroke();
}
ctx.lineWidth = 2;
ctx.strokeStyle = "red";
let prev_y = data[8];
let prev_x = 0;
for (let i = 0; i < data.length - 8; i++) {
let x = i*2;
let y =data[8+i];
ctx.beginPath();
ctx.moveTo(prev_x-0.5, prev_y-0.5);
ctx.lineTo(x-0.5, y-0.5);
ctx.stroke();
prev_y = y;
prev_x = x;
}
});
},
function(error) {
document.getElementById("elapsed_time_h").value = '_';
document.getElementById("elapsed_time_m").value = '_';
})
}
function startWorker() {
if(typeof(Worker) !== "undefined") {
if(typeof(w) == "undefined") {
w = new Worker(blobURL);
}w.onmessage = function(event) {
fetchData();
};
} 
else {
document.getElementById("error").innerHTML = "Sorry, your browser does not support Web Workers...";
}
}
startWorker();
</script>)"" + end;
  start_validation_position = html_body1.indexOf("?S");
  start_button_text_position = html_body1.indexOf("?rt");
  start_select_position = html_body1.indexOf("?te");
}

String MainPage::html(){
  String smoker_temp_setpoint_str = String(smoker_temp_setpoint);
  String cooking_time_hours_str = String(cooking_time_hours);
  String cooking_time_minutes_str = String(cooking_time_minutes);
  String elapsed_time_h_str = String(elapsed_time_h);
  String elapsed_time_m_str = String(elapsed_time_m);
  String pwm_duty_cycle_str = String(pwm_duty_cycle);
      html_body1[start_select_position] = smoker_temp_setpoint_str[0];
      html_body1[start_select_position+1] = smoker_temp_setpoint_str[1];
      html_body1[start_select_position+2] = smoker_temp_setpoint_str[2];
      if(cooking_time_hours<10){
        html_body1[start_select_position+24] = ' ';
        html_body1[start_select_position+25] = '"';
        html_body1[start_select_position+26] = cooking_time_hours_str[0];
      }else{
        html_body1[start_select_position+24] = '"';
        html_body1[start_select_position+25] = cooking_time_hours_str[0];
        html_body1[start_select_position+26] = cooking_time_hours_str[1];
      }
      if(cooking_time_minutes<10){
        html_body1[start_select_position+48] = ' ';
        html_body1[start_select_position+49] = '"';
        html_body1[start_select_position+50] = cooking_time_minutes_str[0];
      }else{
        html_body1[start_select_position+48] = '"';
        html_body1[start_select_position+49] = cooking_time_minutes_str[0];
        html_body1[start_select_position+50] = cooking_time_minutes_str[1];
      }
      if(elapsed_time_h<10){
        html_body1[start_select_position+72] = ' ';
        html_body1[start_select_position+73] = '"';
        html_body1[start_select_position+74] = elapsed_time_h_str[0];
      }else{
        html_body1[start_select_position+72] = '"';
        html_body1[start_select_position+73] = elapsed_time_h_str[0];
        html_body1[start_select_position+74] = elapsed_time_h_str[1];
      }
      if(elapsed_time_m<10){
        html_body1[start_select_position+97] = '0';
        html_body1[start_select_position+98] = elapsed_time_m_str[0];
      }else{
        html_body1[start_select_position+97] = elapsed_time_m_str[0];
        html_body1[start_select_position+98] = elapsed_time_m_str[1];
      }
      if(pwm_duty_cycle==100){
        html_body1[start_select_position+109] = '"';
        html_body1[start_select_position+110] = '1';
        html_body1[start_select_position+111] = '0';
        html_body1[start_select_position+112] = '0';
      }else if(pwm_duty_cycle<10){
        html_body1[start_select_position+109] = ' ';
        html_body1[start_select_position+110] = '"';
        html_body1[start_select_position+111] = '0';
        html_body1[start_select_position+112] = pwm_duty_cycle_str[0];
      }else{
        html_body1[start_select_position+109] = ' ';
        html_body1[start_select_position+110] = '"';
        html_body1[start_select_position+111] = pwm_duty_cycle_str[0];
        html_body1[start_select_position+112] = pwm_duty_cycle_str[1];
      }
      char str_start_validation[2];
      sprintf(str_start_validation, "%02d", start_button_validation);
      html_body1[start_validation_position] = str_start_validation[0];
      html_body1[start_validation_position+1] = str_start_validation[1];
      if(run == 1){
        html_body1[start_button_text_position] = 'o';
        html_body1[start_button_text_position+1] = 'p';
        html_body1[start_button_text_position+2] = '"';
        html_body1[start_button_text_position+3] = ' ';
        html_body1[start_button_text_position+19] = 'F';
        html_body1[start_button_text_position+20] = 'F';
        html_body1[start_button_text_position+21] = '5';
        html_body1[start_button_text_position+22] = '7';
        html_body1[start_button_text_position+23] = '3';
        html_body1[start_button_text_position+24] = '3';
      }else{
        html_body1[start_button_text_position] = 'a';
        html_body1[start_button_text_position+1] = 'r';
        html_body1[start_button_text_position+2] = 't';
        html_body1[start_button_text_position+3] = '"';
        html_body1[start_button_text_position+19] = '9';
        html_body1[start_button_text_position+20] = '0';
        html_body1[start_button_text_position+21] = 'E';
        html_body1[start_button_text_position+22] = 'E';
        html_body1[start_button_text_position+23] = '9';
        html_body1[start_button_text_position+24] = '0';
      }
      return html_body1;  
}
      

void MainPage::extractAndSetTemperature(String request){
      int index = request.indexOf("temperature=");
      if(index > 0){
        int temp_i = extractValueFromString((index + 12), request.length(), request);
        if(119 < temp_i && temp_i < 576){
          smoker_temp_setpoint = temp_i;
        }
      }
}

void MainPage::extractAndSetTimer(String request){
      int cooking_time;
      int index = request.indexOf("cooking_time_h=");
      if(index > 0){
        cooking_time = extractValueFromString((index + 15), request.length(), request);
        if(0 <= cooking_time && cooking_time < 25){
          cooking_time_hours = cooking_time;
        }
      }
      index = request.indexOf("cooking_time_m=");
      if(index > 0){
        cooking_time = extractValueFromString((index + 15), request.length(), request);
        if(0 <= cooking_time && cooking_time < 60){
          cooking_time_minutes = cooking_time;
        }
      }
}

String SettingsPage::html(){
  String smoker_temp_settings_str = String(smoker_temp_settings);
  String html_body = R""(<div class="header">Smoker Settings</div>
<div class="column_c">
<div class="column_a">
<div>
<form action="">
<label for="setings_temp">Smoker Temperature (&#176F):</label>
<form method="post">
<select name="setings_temp" id="setings_temp" onchange='if(this.value != 0) { this.form.submit(); }'>
<option value="125">125</option>
<option value="150">150</option>
<option value="175">175</option>
<option value="200">200</option>
<option value="225">225</option>
<option value="250">250</option>
<option value="275">275</option>
<option value="300">300</option>
<option value="325">325</option>
<option value="350">350</option>
<option value="375">375</option>
<option value="400">400</option>
<option value="425">425</option>
<option value="450">450</option>
<option value="475">475</option>
<option value="500">500</option>
<option value="525">525</option>
</select>
</form>
</div>
<div>
<form action="">
<label for="duty_cycle">Heater Output (%):</label>
<input type="number" id="duty_cycle" name="duty_cycle" min="00" max="100" step="1" value=0000 onchange='if(this.value != 0) { this.form.submit(); }'>
</form>
</div>
<div>
<form action="">
<label for="PID">Enable PID:</label>
<input type="hidden" name="pid_checkbox">
<input type="checkbox" id="pid" name="pid" value="1" onchange='{ this.form.submit(); }'>
</form>
<form action="">
<label for="proportional">P:</label>
<input type="number" id="roportional" name="proportional" min="0" max="99" step="0.01" value=1111 onchange='if(this.value != 0) { this.form.submit(); }'>
</form>
<form action="">
<label for="integral">I:</label>
<input type="number" id="integral" name="integral" min="0" max="99" step="0.01" value=2222 onchange='if(this.value != 0) { this.form.submit(); }'>
</form>
<form action="">
<label for="derivative">D:</label>
<input type="number" id="derivative" name="derivative" min="0" max="99" step="0.01" value=3333 onchange='if(this.value != 0) { this.form.submit(); }'>
</form>
</div>
<div>
<form action="">
<label for="integral_w">Integral Enable Window: (&#176F)</label>
<input type="number" id="integral_w" name="integral_w" min="0" max="99" step="1" value=?i onchange='{ this.form.submit();}'>
</form>
</div>
<div>
<form action="">
<label for="over_temp">Over Temp Cutoff(&#176F):</label>
<input type="number" id="over_temp" name="over_temp" min="0" max="99" step="1" value=4444 onchange='if(this.value != 0) { this.form.submit(); }'>
</form>
</div>
</div>
<form action="wifi" method="post">
<input type="submit" value="WIFI settings" class="Button">
</form>
<form action="smoker" method="post">
<input type="submit" value="Back" class="Button">
</form>
</div>)"";
      html_body.replace("<option value=\"" + smoker_temp_settings_str +"\"", "<option value=\"" + smoker_temp_settings_str + "\" selected");
      int index = (smoker_temp_settings - 125)/25;
      if(index < 0 || index > 16)index = 0;
      html_body.replace("0000", String(settings.pwm_preset[index]));
      html_body.replace("1111", String(settings.Kp));
      html_body.replace("2222", String(settings.Ki));
      html_body.replace("3333", String(settings.Kd));
      html_body.replace("4444", String(settings.temp_high));
      html_body.replace("?i", String(settings.integral_window));
      if(settings.pid_enabled)html_body.replace("pid\" value=\"1\"", "pid\" value=\"1\" checked");
      return css + html_body + end;
}

void SettingsPage::extractAndSetParameters(String request){
      int temp_i;
      int index = request.indexOf("setings_temp=");
      if(index > 0){
        temp_i = extractValueFromString((index + 13), request.length(), request);
        if(119 < temp_i && temp_i < 576){
          smoker_temp_settings = temp_i;
        }
      }
      else if (request.indexOf("pid_checkbox") != -1){
        if (request.indexOf("pid=1") != -1)settings.pid_enabled = 1;
        else{
          settings.pid_enabled = 0;
          pwm_on_phase = get_pwm_duty_cycle_length();
        }
      }
      else if ((index = request.indexOf("proportional")) != -1){
        float temp_f = extractValueFromString((index + 13), request.length(), request);
        if(0 <= temp_f && temp_f < 10){
          settings.Kp = temp_f;
        }
      }
      else if ((index = request.indexOf("integral_w")) != -1){
        temp_i = extractValueFromString((index + 11), request.length(), request);
        if(0 <= temp_i && temp_i < 100){
          settings.integral_window = temp_i;
        }
      }
      else if ((index = request.indexOf("integral")) != -1){
        float temp_f = extractValueFromString((index + 9), request.length(), request);
        if(0 <= temp_f && temp_f < 10){
          settings.Ki = temp_f;
        }
      }    
      else if ((index = request.indexOf("derivative")) != -1){
        float temp_f = extractValueFromString((index + 11), request.length(), request);
        if(0 <= temp_f && temp_f < 10){
          settings.Kd = temp_f;
        }
      }
      else if ((index = request.indexOf("over_temp")) != -1){
        temp_i = extractValueFromString((index + 10), request.length(), request);
        if(0 <= temp_i && temp_i < 100){
          settings.temp_high = temp_i;
        }
      }
      else if ((index = request.indexOf("duty_cycle")) != -1){
        temp_i = extractValueFromString((index + 11), request.length(), request);
        if(0 <= temp_i && temp_i < 101){
          int index = (smoker_temp_settings - 125)/25;
          if(index < 0 || index > 16)index = 0;
          settings.pwm_preset[index] = temp_i;
          if((smoker_temp_settings == smoker_temp_setpoint) && !settings.pid_enabled)pwm_on_phase = get_pwm_duty_cycle_length();
        }
      }
      else smoker_temp_settings = smoker_temp_setpoint;
}

String WifiPage::html(){
    char PASSWORD[9];
    if(settings.PASSWORD[0]=='\0')sprintf(PASSWORD, "");
    else sprintf(PASSWORD, "********");
    String html_body = R""(<div class="header">WIFI Settings</div>
<div class="column_c">
<div class="column_a">
<div>
<form action="">
<label for="ssid_ap">SSID for AP:</label><br>
<input type="text" id="ssid_ap" value=")"" + String(settings.SSID_AP) + R""(" name="ssid_ap" accept-charset="utf-8" onchange='if(this.value != 0) { this.form.submit(); }'>
</form>
</div>
<div>
<form action="">
<label for="pass_ap">Password for AP:</label><br>
<input type="text" id="pass_ap" value=")"" + String(settings.PASSWORD_AP) + R""(" name="pass_ap" onchange='if(this.value != 0) { this.form.submit(); }'>
</form>
</div>
<div>
<form action="">
<label for="ssid_ap">Client SSID:</label><br>
<input type="text" id="ssid_client" value=")"" + String(settings.SSID) + R""(" name="ssid_client" onchange='if(this.value != 0) { this.form.submit(); }'>
</form>
</div>
<div>
<form action="">
<label for="ssid_ap">Client Password:</label><br>
<input type="password" id="pass_client" value=")"" + PASSWORD + R""(" name="pass_client" onchange='if(this.value != 0) { this.form.submit(); }'>
</form>
</div>
</div>
<form action="wifi_save" method="post">
<input type="submit" value="Save" class="Button">
</form>
<form action="setup" method="post">
<input type="submit" value="Back" class="Button">
</form>
</div>)"";
    return css + html_body + end;
}
String WifiPage::urlDecode(String eString) {
	    String ret;
	    char ch;
	    int i, j;
	    for (i=0; i<eString.length(); i++) {
		    if (int(eString[i])==37) {
		    	sscanf(eString.substring(i+1,i+3).c_str(), "%x", &j);
	    		ch=static_cast<char>(j);
	    		ret+=ch;
	    		i=i+2;
	    	} else {
		    	ret+=eString[i];
		    }
    	}
	    return (ret);
}


void WifiPage::process_request(String request){
      int index;
      index = request.indexOf("ssid_ap=");
      if(index > 0){
        String temp_str = urlDecode(request.substring(index+8));
        int len = temp_str.length()-8; //minus  HTTP/1.1
        if(len < 64){
          temp_str.toCharArray(settings.SSID_AP, len);
        }
      }
      index = request.indexOf("pass_ap=");
      if(index > 0){
        String temp_str = urlDecode(request.substring(index+8));
        int len = temp_str.length()-8; //minus  HTTP/1.1
        if(len < 64){
          temp_str.toCharArray(settings.PASSWORD_AP, len);
        }
      }
      index = request.indexOf("ssid_client=");
      if(index > 0){
        String temp_str = urlDecode(request.substring(index+12));
        int len = temp_str.length()-8; //minus  HTTP/1.1
        if(len < 64){
          temp_str.toCharArray(settings.SSID, len);
        }
      }
      index = request.indexOf("pass_client=");
      if(index > 0){
        String temp_str = urlDecode(request.substring(index+12));
        int len = temp_str.length()-8; //minus  HTTP/1.1
        if(len < 64){
          temp_str.toCharArray(settings.PASSWORD, len);
        }
      }
      index = request.indexOf("wifi_save");
      if(index > 0){
        store_settings_eeprom();
      }
}

String Data::html(){
    String html_body = 
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html\r\n\r\n"
"" + String(smoker_temp_setpoint) + "," + String(cooking_time_hours) + "," + String(cooking_time_minutes) + "," + run + "," + elapsed_time_h + "," + elapsed_time_m + "," + int(thermocouple) + "," + pwm_duty_cycle + "," + temperature_chart.get_data() + ""
      ;
      return html_body;
}
