#include "myWifi.h"
myWifi myServer;

// #include "OLED.h"
// OLED myOled;

int pinLED2 = 2;
int pinLED4 = 4;
int pinLED5 = 5;

#include <ArduinoJson.h>

/// Value 기준
int CO2_BASE = 800;
int VOC_BASE = 80;
int HUM_BASE = 70;
int TMP_BASE = 30;
int ILM_BASE = 3000;
int DUST_SUM_BASE = 50;

bool toggleDay = true;

void setup() {
  pinMode(pinLED2, OUTPUT);
  pinMode(pinLED4, OUTPUT);
  pinMode(pinLED5, OUTPUT);
  delay(1000);
  digitalWrite(pinLED2, HIGH);   
  digitalWrite(pinLED4, HIGH);   
  digitalWrite(pinLED5, HIGH);    
  delay(2000);
  digitalWrite(pinLED2, LOW);   
  digitalWrite(pinLED4, LOW);   
  digitalWrite(pinLED5, LOW);   

  myOled.Init();
  myOled.displaySensorData("Ready ...  ", "Hello Everyone !");

  Serial.begin(115200);
  
  bool _bConnected = false;
  //while(!_bConnected)
  { 
    _bConnected = myServer.tryConnect();   ///  wifi 연결
  }
  delay(3000);
}

void loop() {
  //myServer.getData();
  if(myServer.getWifiStatus() == WL_CONNECTED) {

    DynamicJsonDocument response = myServer.getAirAlarm("non");
    int dust1 = response["dust1"];
    int dust5 = response["dust5"];
    int dust10 = response["dust10"];
    int totalDust = dust1 + dust5 + dust10;
    int co2 = response["co2"];
    int tvoc = response["tvoc"];
    int illum = response["illu"];
    int temp = response["temp"];
    int mois = response["mois"];

    String msg = "";
    if(illum >= ILM_BASE)   /// 많이 어두워짐 : 알람 멈춤.
    {
      msg = "Light: " + String(illum) + "\n Stop the sensor";
      myOled.displaySensorData("Good night!", msg.c_str());
      toggleDay = false;
    } else {
      if(toggleDay == false) {    /// 어두워졌다가 밝아질때 1회
        msg = "Light: " + String(illum) + "\n Stop the sensor";
        myOled.displaySensorData("Good night!", msg.c_str());  
        toggleDay = true;
      }
      
      /// 주요 공기질 상태가 나빠질 경우만 알림.
      if(totalDust >= DUST_SUM_BASE) {        /// 미세먼지가 높아지면?
        msg = "Dust: " + String(totalDust) + "\n Open the Window";
        myOled.displaySensorData("Warning !", msg.c_str());    
        digitalWrite(pinLED2, HIGH);   
      }
      else if(co2 >= CO2_BASE) {
        msg = "CO2: " + String(co2) + "\n Open the Window";
        myOled.displaySensorData("Warning !", msg.c_str());
        digitalWrite(pinLED4, HIGH);   
      }
      else if(tvoc >= VOC_BASE) {
        msg = "VOC: " + String(tvoc) + "\n Open the Window";
        myOled.displaySensorData("Warning !", msg.c_str());        
        digitalWrite(pinLED5, HIGH) ;  
      }
      else {
        if(temp == 0 && mois == 0) {
          msg = "Check Main Sensor !!";
          myOled.displaySensorData("Erro or Invalid ", msg.c_str());
        } 
        else {
          msg = "T: " + String(temp) + "  H: " + String(mois) +"\n Clean air";
          myOled.displaySensorData("Nice Day !", msg.c_str());
          digitalWrite(pinLED2, LOW);   
          digitalWrite(pinLED4, LOW);   
          digitalWrite(pinLED5, LOW);   
        }
      }

    }
  } 
  else {
    // 와이파이접속 정리하고 다시 연결
     myOled.displaySensorData("Lost to Connect ", "Try Restart !!!! ");
     delay(10000);
  }

  //myOled.displaySensorData("Hyun's Air Alarm", "Open the Window !!");
  delay(3000);

}