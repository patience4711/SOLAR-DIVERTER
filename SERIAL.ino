void handle_Serial () {
    Serial.println("\n* * * handle_Serial, type LIST-COMMANDS");  
    int SerialInByteCounter = 0;
    char InputBuffer_Serial[100] = "";
    byte SerialInByte;  
    // first check if there is enough data, at least 13 bytes
    delay(200); //we wait a little for more data as the esp seems slow
    if(Serial.available() < 7 ) {
      // less then 13, we can't expect more so we give up 
      while(Serial.available()) { Serial.read(); } // make the buffer empty 
      Serial.println("invalid command, abort " + String(InputBuffer_Serial));
     return;
    }

// now we know there are at least 13 bytes so we read them
 while(Serial.available()) {
             SerialInByte=Serial.read(); 
             //Serial.print("+");
            
            if(isprint(SerialInByte)) {
              if(SerialInByteCounter<100) InputBuffer_Serial[SerialInByteCounter++]=SerialInByte;
            }    
            if(SerialInByte=='\n') {                                              // new line character
              InputBuffer_Serial[SerialInByteCounter]=0;
              //   Serial.println(F("found new line"));
             break; // serieel data is complete
            }
       }   
   Serial.println("InputBuffer_Serial = " + String(InputBuffer_Serial) );
   diagNose = 2; // direct the output to serial
   int len = strlen(InputBuffer_Serial);
   // evaluate the incomming data

     //if (strlen(InputBuffer_Serial) > 6) {                                // need to see minimal 8 characters on the serial port
     //  if (strncmp (InputBuffer_Serial,"10;",3) == 0) {                 // Command from Master to RFLink
  
          if (strcasecmp(InputBuffer_Serial, "LIST-COMMANDS") == 0) {
              Serial.println(F("*** AVAILABLE COMMANDS ***"));
              Serial.println(F("DEVICE-REBOOT;"));
              Serial.println(F("PRINT-SPIFFS"));
              Serial.println(F("DISPLAY-TEST;"));
              Serial.println(F("DELETE-FILE=filename; (delete a file)"));
              Serial.println("SETVAR;VAR;VALUE; (e.g. setvar;rowNumber;1;)"); 
              Serial.println("DUMP-VARS (shows important vars)"); 
              return;
    } else 

          if (strcasecmp(InputBuffer_Serial, "DUMP-VARS") == 0)  {
          Serial.println("pollFreq = " + String(pollFreq));
          Serial.println("leftMargin = " + String(leftMargin));
          Serial.println("elementPower = " + String(elementPower));
          Serial.println("diagNose = " + String(diagNose));
          Serial.println("dimmerEnabled = " + String(dimmerEnabled));
          Serial.println("duty_ch1 = " + String(duty_ch1));
          Serial.println("duty_ch2 = " + String(duty_ch2));
          return;
     
    } else
 
         if (strcasecmp(InputBuffer_Serial, "POLL-METER") == 0)  {
          actionFlag = 26;
          Serial.println("going to poll the meter") ;  
     
    } else  
    if (strcasecmp(InputBuffer_Serial, "DISPLAY-TEST") == 0) {  
        scanI2C();
        #if defined SG110X_HA || defined U8G2_HA
        writeDisplay();
        #endif
        #ifdef SSD1306_JO
        writeDisplay();
        #endif
        consoleOut("test ledpin");
        consoleOut("tresHold + powerBalance = " + String(tresHold + powerBalance));
        if(digitalRead(ledpin) == HIGH) Serial.println("ledpin is high"); else Serial.println("ledpin is low");
        return;
    } else

//    if (strcasecmp(InputBuffer_Serial, "POLL-TEST") == 0) {  
//        Poll_1f();
//        return;
//    } else
    if (strcasecmp(InputBuffer_Serial, "DEVICE-REBOOT") == 0) {
           Serial.println("\ngoing to reboot ! \n");
           delay(1000);
           ESP.restart();
    } else

    if (strcasecmp(InputBuffer_Serial, "PRINT-SPIFFS") == 0) {
          actionFlag=47;
          return;
    } else
         
    if (strncasecmp(InputBuffer_Serial, "DELETE-FILE=", 12 ) == 0) {  
       Serial.println("len = " + String(len));
       String bestand="";
       for(int i=12;  i<len+1; i++) { bestand += String(InputBuffer_Serial[i]); }
       Serial.println("bestand  = " + bestand);
         // now should have like /bestand.json or so;
         if (SPIFFS.exists(bestand)) 
         {
            Serial.println("going to delete file " + bestand); 
            SPIFFS.remove(bestand);
            Serial.println("file " + bestand + " removed!"); 
         } else { Serial.println("unkown file, forgot the / ??"); }
          return;           
   
       } else
           if (strncasecmp(InputBuffer_Serial, "SETVAR",6)==0) {
           Serial.println("meterType, powerBalance, leftMargin, ");
           String setvar = InputBuffer_Serial;
           setvar = setvar.substring(7); // clean 'SETVAR;'
           Serial.println("string setvar cleaned = " + setvar);
           // now should have like variable;123;
           int pos1 = setvar.indexOf(';');
           int pos2 = setvar.lastIndexOf(';');
           String var = setvar.substring(0,pos1);
           Serial.println("string var = " + var);
           String val = setvar.substring(pos1+1,pos2);
           //Serial.println("string val = " + val);                     
           int var_len = var.length();
           Serial.println("var length = " + String(var_len) );
           int value=val.toInt();
           Serial.println("value = " + String(value));
           delay(100); // to get serial.print by
 
           if(var == "powerBalance") {
             delay(100);
             powerBalance = value;
             Serial.println("powerBalance set to " + String(powerBalance) );
              #if defined SG110X_HA || defined U8G2_HA
              writeDisplay();
              #endif
              #ifdef SSD1306_JO
              writeDisplay();
              #endif
             return;
             } else
           if(var == "leftMargin") {
             delay(100);
             leftMargin = value;
             Serial.println("leftMargin set to " + String(leftMargin) );
              #if defined SG110X_HA || defined U8G2_HA
              writeDisplay();
              #endif
              #ifdef SSD1306_JO
              writeDisplay();
              #endif
             return;
           } else 
            if(var == "meterType") {
             delay(100);
             if(value == 1 )  threePhase = true;
             Serial.println("meterType set to " + String(value) );
             eventSend(1);
             return;
           }  
      } else {   
      Serial.println( String(InputBuffer_Serial) + " INVALID COMMAND" );     
     }

       
    // } // end if if (strncmp (InputBuffer_Serial,"10;",3) == 0)
    Serial.println( String(InputBuffer_Serial) + " UNKNOWN COMMAND" );
    //  end if strlen(InputBuffer_Serial) > 6
  // the buffercontent is not making sense so we empty the buffer
  empty_serial();
   //
}   
