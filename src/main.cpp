#include <Arduino.h>

#include <WiFi.h>
#include <WebSocketsServer.h>
#include <WebServer.h>
#include <RBDdimmer.h>

#define outputPin  33 
#define zerocross  32 // for boards with CHANGEBLE input pins

dimmerLamp dimmer(outputPin, zerocross); //initialase port for dimmer for ESP8266, ESP32, Arduino due boards
//dimmerLamp dimmer(outputPin); //initialase port for dimmer for MEGA, Leonardo, UNO, Arduino M0, Arduino Zero

int outVal = 0;
static int preVal = 0;
const char* ssid = "BeInTech-Ventures";
const char* password = "BI15711588";

static void writeLED(bool);

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);


static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name = "viewport" content = "width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0">
<title>Akzhol Demo</title>
<style>
    input[type=range] {
      -webkit-appearance: none;
      margin: 0px 0;
      width: 65%;
    }

    input[type=range]:focus {
      outline: none;
    }

    input[type=range]::-webkit-slider-runnable-track {
      width: 100%;
      height: 18px;
      cursor: pointer;
      //animate: 0.2s;
      box-shadow: 1px 1px 1px #50555C;
      background: #50555C;
      border-radius: 14px;
      border: 0px solid #000000;
    }

    input[type=range]::-webkit-slider-thumb {
      box-shadow: 0px 0px 0px #000000;
      border: 0px solid #000000;
      height: 18px;
      width: 30px;
      border-radius: 12px;
      background: #529DE1;
      cursor: pointer;
      -webkit-appearance: none;
      margin-top: 0.5px;
    }

    input[type=range]:focus::-webkit-slider-runnable-track {
      background: #50555C;
    }

    input[type=range]::-moz-range-track {
      width: 100%;
      height: 19px;
      cursor: pointer;
      animate: 0.2s;
      box-shadow: 1px 1px 1px #50555C;
      background: #50555C;
      border-radius: 14px;
      border: 0px solid #000000;
    }

    input[type=range]::-moz-range-thumb {
      box-shadow: 0px 0px 0px #000000;
      border: 0px solid #000000;
      height: 18px;
      width: 40px;
      border-radius: 12px;
      background: #529DE1;
      cursor: pointer;
    }

    input[type=range]::-ms-track {
      width: 100%;
      height: 19px;
      cursor: pointer;
      animate: 0.2s;
      background: transparent;
      border-color: transparent;
      color: transparent;
    }

    input[type=range]::-ms-fill-lower {
      background: #50555C;
      border: 0px solid #000000;
      border-radius: 28px;
      box-shadow: 1px 1px 1px #50555C;
    }

    input[type=range]::-ms-fill-upper {
      background: #50555C;
      border: 0px solid #000000;
      border-radius: 28px;
      box-shadow: 1px 1px 1px #50555C;
    }

    input[type=range]::-ms-thumb {
      box-shadow: 0px 0px 0px #000000;
      border: 0px solid #000000;
      height: 18px;
      width: 40px;
      border-radius: 12px;
      background: #529DE1;
      cursor: pointer;
    }

    input[type=range]:focus::-ms-fill-lower {
      background: #50555C;
    }

    input[type=range]:focus::-ms-fill-upper {
      background: #50555C;
    }

    input[type="text"] {
      width: 250px;
      height: 35px;
    }

    input[type="password"] {
      width: 250px;
      height: 35px;
    }

    .onoffswitch {
      position: relative;
      width: 75px;
      -webkit-user-select: none;
      -moz-user-select: none;
      -ms-user-select: none;
    }

    .onoffswitch-checkbox {
      display: none;
    }

    .onoffswitch-label {
      display: block;
      overflow: hidden;
      cursor: pointer;
      height: 36px;
      padding: 0;
      line-height: 36px;
      border: 2px solid #E3E3E3;
      border-radius: 36px;
      background-color: #FFFFFF;
      transition: background-color 0.3s ease-in;
    }

    .onoffswitch-label:before {
      content: "";
      display: block;
      width: 36px;
      margin: 0px;
      background: #FFFFFF;
      position: absolute;
      top: 0;
      bottom: 0;
      right: 34px;
      border: 2px solid #E3E3E3;
      border-radius: 36px;
      transition: all 0.3s ease-in 0s;
    }

    .onoffswitch-checkbox:checked+.onoffswitch-label {
      background-color: #49E845;
    }

    .onoffswitch-checkbox:checked+.onoffswitch-label,
    .onoffswitch-checkbox:checked+.onoffswitch-label:before {
      border-color: #49E845;
    }

    .onoffswitch-checkbox:checked+.onoffswitch-label:before {
      right: 0px;
    }
  </style>
<script>
 var websock;

function start() {
  websock = new WebSocket('ws://' + window.location.hostname + ':81/');
  websock.onopen = function(evt) { console.log('websock open'); };
  websock.onclose = function(evt) { console.log('websock close'); };
  websock.onerror = function(evt) { console.log(evt); };
  websock.onmessage = function(evt) {
    console.log(evt);
  };
}

    function Handle_Switch_0() {
      var toSend;
      if (document.getElementById('myonoffswitch_A').checked == 1) {
        toSend = "ledon";
      }
      else {
        toSend = "ledoff";
      }
      websock.send(toSend);
    };
    function showValueA(newValue) {
      document.getElementById("outputTextA").innerHTML = Math.round(newValue);
      websock.send("a" + newValue);
    }
  </script>
</head>
<body onload="javascript:start();">
  <center>
    <br>
    <h2 style="color:black; font-family:verdana;">AKZHOL DIMMER </h2>
    <input type="range" id="inputSlider_0" min="0" max="100" value="0" step="1" onmouseup="showValueA(this.value)" />
    <!-- ontouchend for touch devices -->
    <br><span id="outputTextA">0</span>
    <p></p>
    <div class="onoffswitch">
      <input type="checkbox" name="onoffswitch" onchange="Handle_Switch_0()" class="onoffswitch-checkbox"
        id="myonoffswitch_A">
      <label class="onoffswitch-label" for="myonoffswitch_A"></label>
    </div>
</body>
</html>
)rawliteral";


const int LEDPIN = 16;
bool LEDStatus;

const char LEDON[] = "ledon";
const char LEDOFF[] = "ledoff";

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
  Serial.printf("webSocketEvent(%d, %d, ...)\r\n", num, type);
  switch(type) {
    case WStype_DISCONNECTED:
    {
       Serial.printf("[%u] Disconnected!\r\n", num);
       break;
    }
    case WStype_CONNECTED:
    {
        IPAddress ip = webSocket.remoteIP(num);     
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\r\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        // Send the current LED status
        if (LEDStatus) {        
          webSocket.sendTXT(num, LEDON, strlen(LEDON));       
        }       
        else {        
          webSocket.sendTXT(num, LEDOFF, strlen(LEDOFF));     
        }     
      break;
    }
 
    case WStype_TEXT:
    {
      String text = String((char *) &payload[0]);     
      Serial.printf("[%u] get Text: %s\r\n", num, payload);    
      if (strcmp(LEDON, (const char *)payload) == 0) {      
  dimmer.setState(ON);
      }   
      else if (strcmp(LEDOFF, (const char *)payload) == 0) {    
  dimmer.setState(OFF);
      }        
      else if(text.startsWith("a"))    
        {    
          String val=(text.substring(text.indexOf("a")+1,text.length()));        
          int outVal = val.toInt();    
           if (preVal != outVal)
  {
    //Serial.print("lampValue -> ");
    //printSpace(dimmer.getPower());
    //Serial.print(dimmer.getPower());
    Serial.println("if");
   // dimmer.toggleSettings(preVal, outVal); //Name.toggleSettings(MIN, MAX);
  if(preVal<outVal){
        Serial.println("2if");
    while(preVal != outVal){
        Serial.println("loop");
     dimmer.setPower(preVal);
     preVal++;
     delay(80);
    }
  }

  else if(preVal>outVal){
    while(preVal != outVal){
     dimmer.setPower(preVal);
     preVal--;
     delay(80);
    }
  }
   //dimmer.setPower(outVal); // setPower(0-100%);     
        }   
      else {    
        Serial.println("Unknown command");  
      }      
      // send data to all connected clients      
      webSocket.broadcastTXT(payload, length);    
      break;
    }
    }  
    case WStype_BIN:    
    {     
      Serial.printf("[%u] get binary length: %u\r\n", num, length);   
      // hexdump(payload, length); 
      // echo data back to browser  
      webSocket.sendBIN(num, payload, length);     
      break;   
    }
           
    default:
    {
      Serial.printf("Invalid WStype [%d]\r\n", type);   
      break;
    }
  }
}



void handleRoot()
{
  server.send(200, "text/html", INDEX_HTML); 
}



void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

static void writeLED(bool LEDon)
{
  LEDStatus = LEDon;
  // Note inverted logic for Adafruit HUZZAH board
  if (LEDon) {
    digitalWrite(LEDPIN, 0);
  }
  else {
    digitalWrite(LEDPIN, 1);
  }
}
void printSpace(int val)
{
  if ((val / 100) == 0) Serial.print(" ");
  if ((val / 10) == 0) Serial.print(" ");
}


void setup()
{
  pinMode(LEDPIN, OUTPUT);
  dimmer.begin(NORMAL_MODE, ON); //dimmer initialisation: name.begin(MODE, STATE) 
  writeLED(false);
  Serial.begin(115200);
  //Serial.setDebugOutput(true);
  Serial.println();
  Serial.println();
  Serial.println();

  for(uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] BOOT WAIT %d...\r\n", t);
    Serial.flush();
    delay(500);
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop()
{
  webSocket.loop();
  server.handleClient();
  
/*
    Serial.print("lampValue -> ");
    printSpace(dimmer.getPower());
    Serial.print(dimmer.getPower());
    Serial.println("%");
  delay(1000);*/

}
