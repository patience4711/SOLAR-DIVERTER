// find out if the request comes from inside the network
//check if the first 9 characters of the router's ip ( 192.168.0 ) is in the url
bool checkRemote(String url) {
   if(securityLevel==0) return false; //no security so never remote
   if ( url.indexOf(WiFi.gatewayIP().toString().substring(0, securityLevel)) == -1 ) return true; else return false;
//return false;
}

// we come here when an unknown request is done

void handleNotFound(AsyncWebServerRequest *request) {
  
bool intern = false;
if(!checkRemote( request->client()->remoteIP().toString()) ) intern = true;

// **************************************************************************
//             R E S T R I C T E D   A R E A
// **************************************************************************
// access only from inside the network

if ( intern ) {    //DebugPrintln("the request comes from inside the network");
        String serverUrl = request->url().c_str();             
        if ( serverUrl.indexOf("/api/v1/state") > -1) {
             
             JsonDocument root; //(160);
             //float Power = active_power_w - active_power_l1_w;
             root["powerBalance"] = powerBalance;
             //Response = like {"powerBalance":-1234}
             //String reSponse="{/"powerBalance/":" + String(round0(Power)) + "}";
             AsyncResponseStream *response = request->beginResponseStream("application/json");
             serializeJson(root, * response);
             request->send(response);
        }

        if ( serverUrl.indexOf("/api/v1/poll") > -1) {
              if(Polling)
              {
                 request->send ( 200, "text/plain", "automatic polling, skipping" ); //zend bevestiging
                 return; 
              }
             if (request->hasParam("debug")) diagNose = 3;
             debugLog = "debug poll command\n";
             //String teZenden = F("polled power log ");
             //request->send ( 200, "text/plain", teZenden ); 
             actionFlag = 26; // takes care for the polling
             String term = "polling smart meter ";
             if(diagNose==3) term += "<br><br><a href=\"/readDebug\">view the log</a>";
             request->send ( 200, "text/html", term ); 
             return;
       } else
       
       if ( serverUrl.indexOf("readDebug") > -1) {
          debugLog += "length log: " + String(debugLog.length());
          request->send(200, "text/html", "<pre>" + debugLog + "</pre>");
          debugLog="";
          return;
      } else      
        if ( serverUrl.indexOf("/api/data") > -1) {
            char temp[13]={0};
            uint8_t remote = 0;
            JsonDocument root; //(160);
            if(checkRemote( request->client()->remoteIP().toString()) ) remote = 1; // for the menu link

               root["power_balance"] = powerBalance;
#ifdef SSD1306_JO
               root["power_l1"] = round0(power_l1);
               root["power_l2"] = round0(power_l2);
               root["power_l3"] = round0(power_l3);
#endif
             // {"active_power_w":5000,"active_power_l1w":5000,"power_balance":-5000}  length 66
             AsyncResponseStream *response = request->beginResponseStream("application/json");
   
             serializeJson(root, * response);
             request->send(response);
                 
        }
       // if we are here, no valid api was found    
       request->send ( 200, "text/plain", "ERROR this link is invalid, go back <--" );//send not found message
       }             

    else { // not intern
      //DebugPrint("\t\t\t\t unauthorized, not from inside the network : ");
      request->send ( 200, "text/plain", "ERROR you are not authorised, go back <--" );//send not found message
    }
}
