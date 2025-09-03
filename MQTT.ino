bool mqttConnect() {   // 
/* this function checks if we are connected to the broker, if not connect anyway */  
    if( MQTT_Client.connected() ) {
       return true;
    }
    // we are here because w'r not connected. Signal with the LED
    ledblink(2,70);
    if(diagNose) consoleOut("mqtt connecting");
    if (Mqtt_Port == 0 ) { Mqtt_Port = 1883;}   // just in case ....
    uint8_t retry = 3;
    String intopic = "DIVERTER-" + String(getChipId(true)) + "/in";  
    // make a char of this string
    int arrayLength = intopic.length(); 
    char inTopic[arrayLength+1];
    intopic.toCharArray(inTopic, arrayLength+1);    
    //  char Mqtt_inTopic[11]={"ESP-POWERLOG/in"};
    consoleOut("intopic = " + String(inTopic));
    while (!MQTT_Client.connected()) {

      if ( MQTT_Client.connect( Mqtt_Clientid, Mqtt_Username, Mqtt_Password) )
      {
         //connected, so subscribe to inTopic (not for thingspeak)
        if(Mqtt_Format != 5 ) {
           if(  MQTT_Client.subscribe ( inTopic ) ) { 
               consoleOut("subscribed to " + String(inTopic ));
           }
        }
         if(diagNose) Serial.println("mqtt connected");
      
       return true;

    } else {
        //String term = "connection failed state: " + String(MQTT_Client.state());

        if (!--retry) break; // stop when tried 3 times
        delay(500);
    }
  }
  // if we are here , no connection was made.

  if(diagNose) Serial.println("mqtt connection failed");
  return false;
}

// *************************************************************************
//                   process received mqtt
// *************************************************************************

void MQTT_Receive_Callback(char *topic, byte *payload, unsigned int length)
{
   
//    String Payload = "";     // convert the payload to a String...
//    for (int i = 0; i < length; i++)
//    {
//        Payload += (char)payload[i]; // convert to char, needed???
//    }
    
   // ws.textAll("mqtt received " + Payload);

    JsonDocument  doc;       // We use json library to parse the payload                         
   //  The function deserializeJson() parses a JSON input and puts the result in a JsonDocument.
   // DeserializationError error = deserializeJson(doc, Payload); // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, payload); // Deserialize the JSON document
    if (error)            // Test if parsing succeeds.
    {
       if(diagNose)  ws.textAll("mqtt no valid json ");
        return;
    } 
    
    // We check the kind of command format received with MQTT
    //now we have a payload like {"poll",1}    
    if (!doc["poll"].isNull())
    //if( doc.containsKey("poll") )
    {
        //int inv = doc["poll"].as<int>(); 
        consoleOut( "got message containing \"poll\"");

        if(!Polling)
        {
             actionFlag = 26; // takes care for the polling
              return;
            } else {
               consoleOut("error, automatic polling is on");
              return;         
            }
        }
        else
        {
          consoleOut("polling = automatic, skipping");
        }
}


#ifdef TEST
    void sendTestMqtt() 
    {
        consoleOut("sending test mosquitto message");
        char toMQTT[512]={0};
        char Mqtt_send[26]={0}; 
        strcpy(Mqtt_send, Mqtt_outTopic);
        bool reTain = false;
       
        snprintf(toMQTT, sizeof(toMQTT), "{\"idx\":%d,\"nvalue\":0,\"svalue\":\"%.0f\"}" , powerIdx, powerBalance);
        consoleOut("sendTestMqtt toMQTT = " + String(toMQTT)); 
        if(mqttConnect() ) MQTT_Client.publish ( Mqtt_send, toMQTT, reTain );
        
        int consumed = (int)(duty_ch1 * elementPower) + (int)(duty_ch2 * elementPower);
        snprintf(toMQTT, sizeof(toMQTT), "{\"idx\":%d,\"nvalue\":0,\"svalue\":\"%d\"}" , absorbIdx, consumed);
        consoleOut("sendTestMqtt toMQTT = " + String(toMQTT));
        if(mqttConnect() ) MQTT_Client.publish ( Mqtt_send, toMQTT, reTain );
    }
#endif
void sendMqtt() {

  if(Mqtt_Format == 0) return;  
  float calibratedPower = elementPower * (1.0f + caliBration / 100.0f);
  int consumed = (int)(duty_ch1 * calibratedPower) + (int)(duty_ch2 * calibratedPower);
  char Mqtt_send[26]={0};  
  strcpy(Mqtt_send, Mqtt_outTopic);

  bool reTain = false;
  char pan[50]={0};
  char tail[40]={0};
  char toMQTT_1[112]={0};
  char toMQTT_2[112]={0};
// the json to p1 domoticz must be something like {"command":"udevice", "idx":1234, "svalue":"1234"}
 
   switch(Mqtt_Format)  { 
    case 1: 
        snprintf(toMQTT_1, sizeof(toMQTT_1), "{\"idx\":%d,\"nvalue\":0,\"svalue\":\"%.0f\"}" , powerIdx, powerBalance);
        snprintf(toMQTT_2, sizeof(toMQTT_2), "{\"idx\":%d,\"nvalue\":0,\"svalue\":\"%d\"}" , absorbIdx, consumed);
        break;
    }
   consoleOut("toMQTT_1 = " + String(toMQTT_1)); 
   consoleOut("toMQTT_2 = " + String(toMQTT_1));
   // mqttConnect() checks first if we are connected, if not we connect anyway
   if(mqttConnect() ) MQTT_Client.publish ( Mqtt_send, toMQTT_1, reTain );
   if(mqttConnect() ) MQTT_Client.publish ( Mqtt_send, toMQTT_2, reTain );
}
