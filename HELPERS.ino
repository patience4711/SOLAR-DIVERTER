
bool checkTime() {
  int currentTimeMin = hour() * 60 + minute();

  int setonTimeMin = ((switchOn[0]-'0')*10 + (switchOn[1]-'0')) * 60
                   + ((switchOn[3]-'0')*10 + (switchOn[4]-'0'));

  int setoffTimeMin = ((switchOff[0]-'0')*10 + (switchOff[1]-'0')) * 60
                    + ((switchOff[3]-'0')*10 + (switchOff[4]-'0'));

  bool active;
  if (setoffTimeMin > setonTimeMin) {
    // normal case, e.g. 08:00 → 20:00
    active = (currentTimeMin >= setonTimeMin && currentTimeMin < setoffTimeMin);
  } else {
    // overnight case, e.g. 17:30 → 06:16
    active = (currentTimeMin >= setonTimeMin || currentTimeMin < setoffTimeMin);
  }

  return active;
}

void delayedReset() {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  SPIFFS.end();       // Recommended before formatting
  SPIFFS.format();
  ESP.restart();
}

void flashErase(AsyncWebServerRequest *request) {
// call this from the portal to erase the wifi and the flash
     confirm();
     request->send(200, "text/html", toSend);
     resetTicker.once(3.0, delayedReset); // 1-second delay
}


// function to show debug info
  // function to 
void consoleOut(String toLog) {
 switch (diagNose) 
    { case 0:  
          break;
      case 1:       
          delay(100); // otherwise the socket cannot keep up
          ws.textAll(toLog);
          break;
      case 2: 
          Serial.println(toLog);
          break;
      case 3:
           toLog += "\n";
           debugLog += toLog;   
    }
}

// flash the led
void ledblink(int i, int wacht) {
  for(int x=0; x<i; x++) {
    digitalWrite(led_onb, LED_AAN);
    delay(wacht);
    digitalWrite(led_onb, LED_UIT);
    delay(wacht);
   }
}


String getChipId(bool sec) {
    uint32_t chipId = 0;
    for(int i=0; i<17; i=i+8) {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  if(sec) return String(chipId); else return "ESP32C3-" + String(chipId);
}


char *split(char *str, const char *delim)
{
    char *p = strstr(str, delim);

    if (p == NULL)
        return NULL; // delimiter not found

    *p = '\0';                // terminate string after head
    return p + strlen(delim); // return tail substring
}


double round3(double value) {
   return (int)(value * 1000 + 0.5) / 1000.0;
}
double round2(double value) {
   return (int)(value * 100 + 0.5) / 100.0;
}
double round1(double value) {
   return (int)(value * 10 + 0.5) / 10.0;
}
double round0(double value) {
   return (int)(value + 0.5) ;
}

void empty_serial() { // remove any remaining data in serial buffer
  while(Serial.available()) {
    Serial.read();
    //uart_read_bytes(UART_NUM_2, &data, 1, 20 / portTICK_PERIOD_MS);
  }
}

void empty_Serial1() { // remove any remaining data in serial buffer
  while(Serial1.available()) {
    Serial1.read();
  }
}

void showDir() {
    ws.textAll("**** Content filesystem ****");
    char temp[50]={0};
    File root = SPIFFS.open("/");
    File file = root.openNextFile();
    while (file) {
          sprintf(temp, "%s  size %u<br>" , file.name(), file.size() );
          ws.textAll(String(temp));
          delay(100);
          file = root.openNextFile();
    }
 }

 // all actions called by the webinterface should run outside the async webserver environment
// otherwise crashes will occure.
    void test_actionFlag() {
/*
 * 10 = reboot
 * 11 = flush wifi and start ap
 * 24 = reconnect mosquitto
 * 25 = recalculate time
 * 26 = poll the meter
 * 126 = serial read test
 * 46 = printout the files
 * 49 = test mosquitto

 */
  
  // ******************   reboot  *************************  
     if (actionFlag == 10) { // reboot
     delay(2000); // give the server the time to send the confirm
     //DebugPrintln("rebooting");
     //write_eeprom();
     ESP.restart();
     } 
  // ******************  reset the nework data and reboot in AP *************************
    if (actionFlag == 11 || value == 11) { // 
     //DebugPrintln("erasing the wifi credentials, value = " + String(value) + "  actionFlag = " + String(actionFlag));
     delay(1000); //reserve time to release the button
     flush_wifi();
     WiFi.disconnect(true);
     //Serial.println(F("wifi disconnected"));
     ESP.restart();
  }  


    // ********* reconnect mosquitto 
    if (actionFlag == 24) {
        actionFlag = 0; //reset the actionflag
        MQTT_Client.disconnect() ;
       //reset the mqttserver
        MQTT_Client.setServer ( Mqtt_Broker, Mqtt_Port );
        MQTT_Client.setCallback ( MQTT_Receive_Callback ) ;
        if (Mqtt_Format != 0) mqttConnect(); // reconnect mqtt after change of settings
    }    

//    if (actionFlag == 23) {
//      actionFlag = 0; //reset the actionflag
//      empty_serial();
//      unsigned long nu = millis();
//      consoleLog("nu before = " + String(nu) );
//      waitSerialAvailable(5); // recalculate time after change of settings
//      nu = millis();
//      consoleLog("nu after = " + String(nu) );
//      if(Serial.available()) consoleLog("there is data available");
//    }    
// *************  get and recalculate time *****************    
    if (actionFlag == 25) {
      actionFlag = 0; //reset the actionflag
      getTijd(); // recalculate time after change of settings
    }
// *************  poll the meter ***************** 
    if (actionFlag == 26) { 
      actionFlag = 0; //reset the actionflag
      Poll(); // get data
      //sendMqtt();
      //sendMqtt(); //gas
      if(diagNose == 3) diagNose = 0;
    }    
// ************* dimmer setting ******************
    if(actionFlag == 27)
    {
     actionFlag = 0;
       if (dimmerEnabled == 0) {
          detachInterrupt(digitalPinToInterrupt(ZC_PIN)); // prevent interrupts during the isr
          digitalWrite(TRIAC1_PIN, LOW);
          digitalWrite(TRIAC2_PIN, LOW);
          duty_ch1 = 0.0f;
          duty_ch2 = 0.0f;
          } 
        if(dimmerEnabled == 1) attachInterrupt(digitalPinToInterrupt(ZC_PIN), onZeroCross, RISING);
        if(dimmerEnabled == 2) {
           detachInterrupt(digitalPinToInterrupt(ZC_PIN)); // prevent interrupts during the isr
           digitalWrite(TRIAC1_PIN, HIGH);
           digitalWrite(TRIAC2_PIN, HIGH);
           duty_ch1 = 1.0f;
           duty_ch2 = 1.0f;
        } 
    }


//// *************  serial read test ***************** 
//    if (actionFlag == 126) { 
//      actionFlag = 0; //reset the actionflag
//      consoleOut("running read_test");
//      read_test(); // read 100 chars in teleGram
//    }

//    // decode the testfile
//    if (actionFlag == 28) { //
//      actionFlag = 0; //reset the actionflag
//      if(!testTelegram) {
//        consoleLog("telegram is modified, reboot if you want to test");
//        return;
//      }
      // if we have te telegram read from spiffs we can decode it
//      polled=true;
//      //we need the readCRC so we extract it from the file
//         int len = strlen(teleGram);
//         strncpy(readCRC, teleGram + len-4, 4); 
//         consoleLog("readCRC = " + String(readCRC) );
//         decodeTelegram();
//         sendMqtt(false);
//         sendMqtt(true);
//    }

// test the reception of a telegram at boot
// the teleGram is saved in SPIFFS

//    if (actionFlag == 30) { 
//      actionFlag = 0; //reset the actionflag
//      if (SPIFFS.exists("/testFile.txt")) SPIFFS.remove("/testFile.txt");      
//      read_into_array(); // 
//      // save the outcome to the files
//      testFilesave(); // an existing file is not overwritten
//
//    }    
// * * * * * * show files in spiffs * * * * * 
    if (actionFlag == 46) { //triggered by the console printout-spiffs
        consoleOut("print the files");
        actionFlag = 0; //reset the actionflag
        showDir();
        //printFiles();
        //SPIFFS_read(); 
    }
// ****************  read spiffs and show content ********************
    if (actionFlag == 47) { //triggered by serial printout-spiffs
        consoleOut("print the files");
        actionFlag = 0; //reset the actionflag
        //showDir();
        //printFiles();
        SPIFFS_read();
        // print monthly values files
        //printFiles();
   }

    // ** test mosquitto *********************
    if (actionFlag == 49) { //triggered by console testmqtt
        actionFlag = 0; //reset the actionflag
        ledblink(1,100);
        // always first drop the existing connection
        MQTT_Client.disconnect();
        ws.textAll("drop connection and reconnect");
        delay(100);
        char Mqtt_send[26] = {0};
       
        if(mqttConnect() ) {
        String toMQTT=""; // if we are connected we do this
        
        strcpy( Mqtt_send , Mqtt_outTopic);
        
        //if(Mqtt_send[strlen(Mqtt_send -1)] == '/') strcat(Mqtt_send, String(Inv_Prop[0].invIdx).c_str());
        toMQTT = "{\"test\":\"" + String(Mqtt_send) + "\"}";
        
        if(Mqtt_Format == 5) toMQTT = "field1=12.3&field4=44.4&status=MQTTPUBLISH";
        
        if( MQTT_Client.publish (Mqtt_outTopic, toMQTT.c_str() ) ) {
            ws.textAll("sent mqtt message : " + toMQTT);
        } else {
            ws.textAll("sending mqtt message failed : " + toMQTT);    
        }
      } 
     // the not connected message is displayed by mqttConnect()
    }


} // end test actionflag
