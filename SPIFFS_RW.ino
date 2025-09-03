// ******************   read and write spiffs   *************************


void SPIFFS_read() {
          if (!SPIFFS.begin(true)) {
          consoleOut("could not mount file system");
          return;
       }
       consoleOut("reading SPIFFS");

       if( file_open_for_read("/wificonfig.json") ) {
                Serial.println("\nread wificonfig\n");
          } else {
             Serial.println("wificonfig.json not opened\n");
          }
       
       if( file_open_for_read("/basisconfig.json") ) {     
             Serial.println("\nread basisconfig\n");
          } else {
          Serial.println("basisconfig.json not opened\n");
        } 
       if( file_open_for_read("/mqttconfig.json") ) {     
             Serial.println("\nread mqttconfig\n");
          } else {
          Serial.println("mqttconfig.json not opened");
        }   

       if( file_open_for_read("/dimmerconfig.json") ) {     
             Serial.println("\nread dimmerconfig\n");
          } else {
          Serial.println("dimmerconfig.json not opened");
        } 
 // einde spiffs read 3 bestanden

}  

// writeStruct(bestand, month); // alles opslaan in SPIFFS

//void writeStruct( String whichfile, int mnd) {
//      File configFile = SPIFFS.open(whichfile, "w");
//      // f.i. monthly_vals0.str
//        if (!configFile)
//           {
//             Serial.print(F("Failed open for write : ")); Serial.println(whichfile);            
//           } 
//           
//              Serial.print(F("Opened for write....")); Serial.println(whichfile);
//              configFile.write( (unsigned char *)&MVALS[mnd], sizeof(MVALS[mnd]) );
//              configFile.close();
//           
//}
//
//bool readStruct(String whichfile ,int mnd) {
//      //input = //mvalues_0.str
//      File configFile = SPIFFS.open(whichfile, "r");
//
//      Serial.print(F( "readStruct mnd = ")); Serial.println( String(mnd) );  
//        if (!configFile)
//           {
//              Serial.print(F("Failed to open for read")); Serial.println(whichfile);
//              return false;           
//           } 
//              Serial.print(F("reading ")); Serial.println(whichfile);
//              configFile.read( (unsigned char *)&MVALS[mnd], sizeof(MVALS[mnd]) );
//              configFile.close();
//              return true;
// }



// **************************************************************************** 
//             save data in SPIFFS                         *  
// ****************************************************************************

void wifiConfigsave() {
   consoleOut("saving wificonfigjson");

    JsonDocument doc;
    JsonObject root = doc.to<JsonObject>();   
//    json["ip"] = static_ip;
    root["pswd"] = pswd;
    root["longi"] = longi;
    root["lati"] = lati;
    root["gmtOffset"] = gmtOffset;
    root["dts"] = dts;
    Serial.println("spiffs save securityLevel = " + String(securityLevel));
    root["securityLevel"] = securityLevel;
    File configFile = SPIFFS.open("/wificonfig.json", "w");
    if (!configFile) Serial.println ("open wificonfig.json failed");
    if(diagNose){ 
      serializeJson(root, Serial);
      Serial.println("");     
    }
    Serial.println(F("")); 
    serializeJson(root, configFile);
    configFile.close();
}


void basisConfigsave() {
    consoleOut("saving basis config");
    JsonDocument doc;
    JsonObject root = doc.to<JsonObject>();
    root["leftMargin"] = leftMargin;
    root["reqApi"] = reqApi;
    //root["tresHold"] = tresHold;
    root["pollFreq"] = pollFreq;
    root["Polling"] = Polling;
    root["diagNose"] = diagNose;    
    File configFile = SPIFFS.open("/basisconfig.json", "w");
    if (!configFile) {
    Serial.println("open basisconfig failed");
    }
    if(diagNose){ 
      serializeJson(root, Serial);
      Serial.println("");     
    }
    serializeJson(root, configFile);
    configFile.close();
    }

void mqttConfigsave() {
    consoleOut("saving mqtt config.json");
    JsonDocument doc;
    JsonObject root = doc.to<JsonObject>();

    root["Mqtt_Broker"] = Mqtt_Broker;
    root["Mqtt_Port"] = Mqtt_Port;    
    root["powerIdx"] = powerIdx;
    root["absorbIdx"] = absorbIdx;
    root["Mqtt_outTopic"] = Mqtt_outTopic;
    root["Mqtt_Username"] = Mqtt_Username;
    root["Mqtt_Password"] = Mqtt_Password;
    root["Mqtt_Clientid"] = Mqtt_Clientid;    
    root["Mqtt_Format"] = Mqtt_Format;    
    File configFile = SPIFFS.open("/mqttconfig.json", "w");
    if (!configFile) {
    Serial.println("open mqttconfig failed");
    }
    if(diagNose){ 
      serializeJson(root, Serial);
      Serial.println("");     
    }
    serializeJson(root, configFile);
    configFile.close();
}

void dimmerConfigsave() {
    consoleOut("saving dimmer config.json");
    JsonDocument doc;
    JsonObject root = doc.to<JsonObject>();
    root["dimmerEnabled"] = dimmerEnabled;
    root["caliBration"] = caliBration;
    root["elementNumber"] = elementNumber;
    root["elementPower"] = elementPower;
    root["switchOn"] = switchOn;    
    root["switchOff"] = switchOff;
    root["nightHeat"] = nightHeat;
   
    File configFile = SPIFFS.open("/dimmerconfig.json", "w");
    if (!configFile) {
    Serial.println("open dimmerconfig failed");
    }
    if(diagNose){ 
      serializeJson(root, Serial);
      Serial.println("");     
    }
    serializeJson(root, configFile);
    configFile.close();

}

bool file_open_for_read(String bestand) {
      //Serial.print("we are in file_open_for_read, bestand = "); Serial.println(bestand); 
       JsonDocument doc;
        File configFile = SPIFFS.open(bestand, "r");
        if (configFile) 
        {
            DeserializationError error = deserializeJson(doc, configFile);
            configFile.close();
            if (error) {
                Serial.print(F("Failed to parse config file: "));
                Serial.println(error.c_str());
                // Continue with fallback values
                } else {
                // no error so we can print the file
                    serializeJson(doc, Serial);  // always print
                }
        } else {
            Serial.print(F("Cannot open config file: "));
            Serial.println(bestand);
            // Continue with empty doc -> all fallbacks will be used
        }            
       
      if (bestand == "/wificonfig.json") {
                strcpy(pswd, doc["pswd"] | "0000");
                longi = doc["longi"] | 5.432;
                lati = doc["lati"] | 51.743;                      
                strcpy(gmtOffset, doc["gmtOffset"] | "+120");
                dts = doc["dts"].as<bool>() | true;
                securityLevel = doc["securityLevel"] | 6;
                //Serial.println("spiffs securityLevel = " + String(securityLevel));
      }

      if (bestand == "/basisconfig.json") {
                leftMargin = doc["leftMargin"] | 10 ;
                strcpy(reqApi, doc["reqApi"] | "n/a");
                //tresHold = doc["tresHold"].as<int>() | 2000; 
                //pollFreq = doc["pollFreq"].as<int>() | 300 ;
                //tresHold = doc["tresHold"] | 2000; 
                pollFreq = doc["pollFreq"] | 300 ;                
                Polling = doc["Polling"].as<bool>() | false;
                diagNose = doc["diagNose"] | 0;
              //consoleOut("read userPwd = " + String(userPwd));      
        }            

        if (bestand == "/mqttconfig.json"){
                strcpy(Mqtt_Broker,   doc["Mqtt_Broker"]   | "192.168.0.100");
                Mqtt_Port =   doc["Mqtt_Port"]             | 1883;  
                strcpy(Mqtt_outTopic, doc["Mqtt_outTopic"] | "domoticz/in");         
                strcpy(Mqtt_Username, doc["Mqtt_Username"] | "n/a");
                strcpy(Mqtt_Password, doc["Mqtt_Password"] | "n/a");
                Mqtt_Format = doc["Mqtt_Format"]           | 0;
                powerIdx = doc["powerIdx"]                 | 123; 
                absorbIdx = doc["absorbIdx"]              | 123;       
         }
        if (bestand == "/dimmerconfig.json"){
                dimmerEnabled = doc["dimmerEnabled"] | 0;
                elementNumber = doc["elementNumber"]  | 1;
                elementPower = doc["elementPower"]  | 100;
                nightHeat = doc["nightHeat"].as<bool>() | false;
                strcpy(switchOn,   doc["switchOn"] | "00:00");
                strcpy(switchOff,   doc["switchOff"] | "00:00");
                caliBration = doc["caliBration"] | 0;
        }

         return true;
 }
