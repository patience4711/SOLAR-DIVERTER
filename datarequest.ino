//{ check systax
// this function requests data from the p1meter dongle
//#if defined SG110X_HA || defined U8G2_HA  // we are polling a 1phase meter

bool Poll() {

  polled = false;
  float testVar = 0;

  // the api is always /api/v1/data so we dont know  3phase or 1phase
  // we can determine this when the json contains active_power_l3_w

  String payLoad = httpGETRequest();
  if ( payLoad == "fail" ) { 
      consoleOut("poll failed, discard");
      failCount ++;
      eventSend(1);
      writeDisplay();
      return false;
  }
  // we'l have a string like {"smr_version":50,"meter_model":"XS210","wifi_ssid":"hansiart","wifi_strength":108,"total_power_import_t1_kwh":1477.206,"total_power_import_t2_kwh":1992.399,"total_power_export_t1_kwh":610.769,"total_power_export_t2_kwh":357.748,"active_power_w":0,"active_power_l1_w":83}
  // we must serialize it to a json but first make a char of it
  //char json[1024];
  int   ArrayLength  = payLoad.length()+1;
  char json[ArrayLength+1];
  payLoad.toCharArray(json,ArrayLength);
  // now we should have a char with the json

   JsonDocument jsonDoc;
   // Deserialize the JSON document
    DeserializationError error = deserializeJson(jsonDoc, json); // Deserialize the JSON document
    if (error)    
    {
       if(diagNose)  Serial.println("http no valid json ");
       strcpy(timeStamp, "not polled");
       writeDisplay(); // write powerBalance to the display
       return false;
    }
    consoleOut("http received valid json and deserialized it ");
    if (!jsonDoc["active_power_w"].isNull()  && !jsonDoc["active_power_l3_w"].isNull() )
    {       
          consoleOut("polled a 3 phase meter");
          threePhase= true;
          powerBalance = round0(active_power_w);
          //read the other phases,don't need to check if the values are present
          power_l1 = round0(jsonDoc["active_power_l1_w"]);          
          power_l2 = round0(jsonDoc["active_power_l2_w"]);
          power_l3 = round0(jsonDoc["active_power_l3_w"]);
          current_l1 = round1(jsonDoc["active_current_l1_a"]);
          current_l2 = round1(jsonDoc["active_current_l2_a"]);
          current_l3 = round1(jsonDoc["active_current_l3_a"]);
          consoleOut("power_l1 = ");     Serial.println(power_l1, 0);
          consoleOut("power_l2 = ");     Serial.println(power_l2, 0);
          consoleOut("power_l3 = ");     Serial.println(power_l3, 0);
          consoleOut("current_l1 = ");   Serial.println(current_l1, 0);         
          consoleOut("current_l2 = ");   Serial.println(current_l2, 0);
          consoleOut("current_l3 = ");   Serial.println(current_l3, 0);          
          consoleOut("powerBalance = "); Serial.println(powerBalance, 0);
    
    }  else 
    
     if (!jsonDoc["active_power_w"].isNull() && !jsonDoc["active_power_l1_w"].isNull() )
    {
          consoleOut("polled a 1 phase meter ");
          threePhase = false;
          power_l1 = round0(jsonDoc["active_power_l1_w"]); //always this one 
          active_power_w = round0(jsonDoc["active_power_w"]);
          powerBalance = round0(active_power_w - power_l1);
          consoleOut("power_l1 = " + String(power_l1));
          consoleOut("powerBalance = " + String(powerBalance));
    } else {
     // if we are here, no usefull payload was found
      polled = false;
      consoleOut("invalid payload");
      eventSend(1);
      writeDisplay();
      return false; 
    }
    
    // if we are here we did a valid poll and have the values
      sprintf( timeStamp, "%02d/%02d %02d:%02d", day(), month(), hour(), minute() );
      polled = true;

      if(dimmerEnabled == 1) adjustDimmer();
      eventSend(1); // inform the webbpage that there is new data
      writeDisplay(); // write powerBalance to the display
      #ifdef TEST
      if(Mqtt_Format != 0) sendTestMqtt();
      #else
      if(Mqtt_Format != 0) sendMqtt();
      #endif
      return true;
}    
    

    
//} 
//#endif
//#ifdef SSD1306_JO
// bool Poll_3f() {
//   polled = false;
//   float testVar = 0;
//  // bool 3Phase = false; 
//   // the api is always /api/v1/data so we dont know  3phase or 1phase
//   // we can determine this when the json contains active_power_l3_w
//   //StaticJsonDocument<1024> jsonDoc;  
//   String payLoad = httpGETRequest();
//   if ( payLoad == "fail" ) { 
//       consoleOut("poll failed, discard");
//       failCount ++;
//       eventSend(1);
//       writeDisplay();
//       return false;
//   }
//   // we'l have a string like {"smr_version":50,"meter_model":"XS210","wifi_ssid":"hansiart","wifi_strength":108,"total_power_import_t1_kwh":1477.206,"total_power_import_t2_kwh":1992.399,"total_power_export_t1_kwh":610.769,"total_power_export_t2_kwh":357.748,"active_power_w":0,"active_power_l1_w":83}
//   // we must serialize it to a json but first make a char of it
//   //char json[1024];
//   int   ArrayLength  = payLoad.length()+1;
//   char json[ArrayLength+1];
//   payLoad.toCharArray(json,ArrayLength);
//   // now we should have a char with the json

//    JsonDocument jsonDoc;
//    //  DynamicJsonDocument jsonDoc(1024);       // We use json library to parse the payload                         
//    //  The function deserializeJson() parses a JSON input and puts the result in a JsonDocument.
//    // Deserialize the JSON document
//     DeserializationError error = deserializeJson(jsonDoc, json); // Deserialize the JSON document
//     if (error)    
//     {
//        if(diagNose)  Serial.println("http no valid json ");
//        strcpy(timeStamp, "not polled");
//        writeDisplay(); // write powerBalance to the display
//        return false;
//     }
//       meterType = 2;
//     if (!jsonDoc["active_power_w"].isNull()  && !jsonDoc["active_power_l3_w"].isNull() )
//     //if( jsonDoc.containsKey("active_power_w") && jsonDoc.containsKey("active_power_l3_w") )
//       {
//           consoleOut("found a 3phase meter ");
    
      
//           //now set the timestamp
//           sprintf( timeStamp, "%02d/%02d %02d:%02d", day(), month(), hour(), minute() );
//           if(diagNose) { 
//           //Serial.print("active_power_w = ");    Serial.println(active_power_w, 0);
//           Serial.print("power_l1 = ");     Serial.println(power_l1, 0);
//           Serial.print("power_l2 = ");     Serial.println(power_l2, 0);
//           Serial.print("power_l3 = ");     Serial.println(power_l3, 0);
//           Serial.print("current_l1 = ");   Serial.println(current_l1, 0);         
//           Serial.print("current_l2 = ");   Serial.println(current_l2, 0);
//           Serial.print("current_l3 = ");   Serial.println(current_l3, 0);          
//           Serial.print("powerBalance = "); Serial.println(powerBalance, 0);

          
          
//           }
//           polled = true;
// //          // now check if the data fits the metertype
// //          if(meterType == 1 && 3Phase == true) {
// //          ConsoleOut("mismatch of meterType 1 and jsondata");
// //          polled = false;  
// //          } else if(meterType == 2 && 3Phase == false) {
// //          ConsoleOut("mismatch of meterType 2 and jsondata")
// //          polled = false;
// //          }
//           eventSend(1); // inform the webbpage that there is new data

//           writeDisplay(); // write powerBalance to the display
//           if(Mqtt_Format != 0) sendMqtt();
//           return true;
// // if we are here, there is an unuseable payload
//       polled = false;
//       consoleOut("invalid payload");
//       eventSend(1);
//       writeDisplay();
//       return false;     
// } 
// //#endif


String httpGETRequest() {
  HTTPClient http;
  String payload;
  String serverName = "http://" + String(reqApi);
  consoleOut("the servername = " + serverName);
  http.begin(serverName);
  int httpResponseCode = http.GET();
  if (httpResponseCode > 0) {
  if(diagNose) Serial.printf("[HTTP] GET... code: %d\n", httpResponseCode);   
  // file found at server
      if (httpResponseCode == HTTP_CODE_OK) { // code 200
         payload = http.getString();
         //consoleOut("payload = " + payload);
      }
    } else {
      if(diagNose) Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
      payload = "fail";    
    }

    http.end();
    consoleOut("datarequest payload = " + payload );
    return payload;
  }
//}  
