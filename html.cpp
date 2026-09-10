#include "html.h"
#include "heater_control.h"
#include "settings_eeprom.h"

const String page_html::css = 
#include "css.h"
    ;

const String page_html::end =
      "</html>"
    ;

String main_page::html(){
       String html_body = R""(

<div class="header">Smoker Controller</div>
	<div class="column_c">
	<div class="column_a">
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
        <span id="pwm">00</span>
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

		<form action="start_00" method="post">
        <input type="submit" id="start_button" value="Start" class="Button" style="color:#90EE90">
      	</form>
		<span id="error"></span>
</div>
  <script>
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
                    document.getElementById("pwm").innerHTML = data[7] > 99 ? data[7].toString() : '0' + data[7].toString();
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
    </script>
      )"";
      
      html_body.replace("<option value=\"" + String(smoker_temp_setpoint) +"\"", "<option value=\"" + String(smoker_temp_setpoint) + "\" selected");
      html_body.replace("<option value= \"" + String(cooking_time_hours) +"\"", "<option value= \"" + String(cooking_time_hours) + "\" selected");
      html_body.replace("<option value =\"" + String(cooking_time_minutes) +"\"", "<option value =\"" + String(cooking_time_minutes) + "\" selected");
      html_body.replace(">H<", ">" + String(elapsed_time_h) + "<");
      html_body.replace(">M<", ">" + String(elapsed_time_m) + "<");
      char str_buffer[3];
      sprintf(str_buffer, "%02d", start_button_validation);
      html_body.replace("start_00", "start_" + String(str_buffer));
      if(run == 1){
        html_body.replace("Start\" class", "Stop\" class");
        html_body.replace("90EE90", "FF5733");
      }
      return css + html_body + end; 
}

void main_page::extractAndSetTemperature(String request){
      int temp_i;
      char temp[4];
      temp[3] = '\0';
      int index = request.indexOf("temperature=");
      if(index > 0){
        temp[0] = request[index + 12];
        temp[1] = request[index + 13];
        temp[2] = request[index + 14];
        temp_i = atoi(temp);
        if(119 < temp_i && temp_i < 576){
          smoker_temp_setpoint = temp_i;
        }
      }
}

void main_page::extractAndSetTimer(String request){
      int cooking_time;
      char temp[3];
      temp[2] = '\0';
      int index = request.indexOf("cooking_time_h=");
      if(index > 0){
        temp[0] = request[index + 15];
        temp[1] = request[index + 16];
        if(temp[1] == '&')temp[1]=' ';
        cooking_time = atoi(temp);
        if(0 <= cooking_time && cooking_time < 25){
          cooking_time_hours = cooking_time;
        }
      }
      index = request.indexOf("cooking_time_m=");
      if(index > 0){
        temp[0] = request[index + 15];
        temp[1] = request[index + 16];
        cooking_time = atoi(temp);
        if(0 <= cooking_time && cooking_time < 60){
          cooking_time_minutes = cooking_time;
        }
      }
}

String settings_page::html(){
    String html_body = R""(
<div class="header">Smoker Settings</div>
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
				<label for="duty_cycle">Duty Cycle (%):</label>
				<input type="number" id="duty_cycle" name="duty_cycle" min="00" max="100" step="1" value=DDD onchange='if(this.value != 0) { this.form.submit(); }'>
			</form>
    </div>
    <div>
			<form action="">
				<label for="cycle_length">Cycle Length (sec):</label>
				<input type="number" id="cycle_length" name="cycle_length" min="0" max="99" step="1" value=32 onchange='if(this.value != 0) { this.form.submit(); }'>
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
</div>
      )"";
      html_body.replace("<option value=\"" + String(smoker_temp_settings) +"\"", "<option value=\"" + String(smoker_temp_settings) + "\" selected");
      int index = (smoker_temp_settings - 125)/25;
      if(index < 0 || index > 16)index = 0;
      html_body.replace("DDD", String(settings.pwm_preset[index]));
      html_body.replace("1111", String(settings.Kp));
      html_body.replace("2222", String(settings.Ki));
      html_body.replace("3333", String(settings.Kd));
      html_body.replace("4444", String(settings.temp_high));
      if(settings.pid_enabled)html_body.replace("pid\" value=\"1\"", "pid\" value=\"1\" checked");
      return css + html_body + end;
}

void settings_page::extractAndSetParameters(String request){
      int temp_i;
      char temp[5];
      temp[3] = '\0';
      int index = request.indexOf("setings_temp=");
      if(index > 0){
        temp[0] = request[index + 13];
        temp[1] = request[index + 14];
        temp[2] = request[index + 15];
        temp_i = atoi(temp);
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
        temp[4] = '\0';
        temp[0] = request[index + 13];
        temp[1] = request[index + 14];
        temp[2] = request[index + 15];
        temp[3] = request[index + 16];
        float temp_f = atof(temp);
        if(0 <= temp_f && temp_f < 10){
          settings.Kp = temp_f;
        }
      }
      else if ((index = request.indexOf("integral")) != -1){
        temp[4] = '\0';
        temp[0] = request[index + 9];
        temp[1] = request[index + 10];
        temp[2] = request[index + 11];
        temp[3] = request[index + 12];
        float temp_f = atof(temp);
        if(0 <= temp_f && temp_f < 10){
          settings.Ki = temp_f;
        }
      }    
      else if ((index = request.indexOf("derivative")) != -1){
        temp[4] = '\0';
        temp[0] = request[index + 11];
        temp[1] = request[index + 12];
        temp[2] = request[index + 13];
        temp[3] = request[index + 14];
        float temp_f = atof(temp);
        if(0 <= temp_f && temp_f < 10){
          settings.Kd = temp_f;
        }
      }
      else if ((index = request.indexOf("over_temp")) != -1){
        temp[4] = '\0';
        temp[0] = request[index + 10];
        temp[1] = request[index + 11];
        temp[2] = request[index + 12];
        temp[3] = request[index + 13];
        temp_i = atoi(temp);
        if(0 <= temp_i && temp_i < 100){
          settings.temp_high = temp_i;
        }
      }
      else if ((index = request.indexOf("duty_cycle")) != -1){
        temp[4] = '\0';
        temp[0] = request[index + 11];
        temp[1] = request[index + 12];
        temp[2] = request[index + 13];
        temp[3] = request[index + 14];
        temp_i = atoi(temp);
        if(0 <= temp_i && temp_i < 101){
          int index = (smoker_temp_settings - 125)/25;
          if(index < 0 || index > 16)index = 0;
          settings.pwm_preset[index] = temp_i;
          if((smoker_temp_settings == smoker_temp_setpoint) &! settings.pid_enabled)pwm_on_phase = get_pwm_duty_cycle_length();
        }
      }
      else smoker_temp_settings = smoker_temp_setpoint;
}

String wifi_page::html(){
    char PASSWORD[9];
    if(settings.PASSWORD[0]=='\0')sprintf(PASSWORD, "");
    else sprintf(PASSWORD, "********");
    String html_body = R""(
<div class="header">WIFI Settings</div>
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
</div>
  )"";
    return css + html_body + end;
}
String wifi_page::urlDecode(String eString) {
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


void wifi_page::process_request(String request){
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

String data::html(){
    String html_body = 
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html\r\n\r\n"
"" + String(smoker_temp_setpoint) + "," + String(cooking_time_hours) + "," + String(cooking_time_minutes) + "," + run + "," + elapsed_time_h + "," + elapsed_time_m + "," + int(thermocouple) + "," + pwm_duty_cycle + ""
      ;
      return html_body;
}
