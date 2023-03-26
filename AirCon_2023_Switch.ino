#include "myWifi.h"
myWifi myServer;


#include "OLED.h"
OLED myOled;

#include <ArduinoJson.h>

void setup() {
  myOled.Init();

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
    int co2 = response["co2"];
    Serial.println(String(co2));

    String msg = "CO2: " + String(co2) + " Close Window";
    myOled.displaySensorData("Air Contition Alarm", msg.c_str());
    // if(response.co2 > 100)
    //   Serila.println()

  }

  //myOled.displaySensorData("Hyun's Air Alarm", "Open the Window !!");

  delay(3000);

  //myOled.drawMaple();

  delay(5000);
}