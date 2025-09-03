
void displaySetup() {
     Serial.println("setup display");
     delay(1000);
     if( u8g2.begin() ) Serial.println("U8g2 started");
     u8g2.setContrast(255); // set contrast to maximum 
     u8g2.setBusClock(400000); //400kHz I2C 
     //u8g2.setFont(u8g2_font_helvB18_tr);
     Serial.println("setup display done");
}


// xOffset = 30; // = (132-72)/2 = 30
// yOffset = 12; // = (64-40)/2 = 12
//the display = 72 X 40; ??
void printDisplay()
{
//int xPos = xOffset + posNumber; //via serial -2
//int yPos = yOffset + lineNumber; //via serial -12
//when the print starts on 28/24 its oke
int width = 72;
int height = 40;
String toPrint = "printdisplay on " + String(xPos + leftMargin) + "/" + String(yPos);  //28x24
    Serial.println(toPrint);
    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.clearBuffer(); // clear the internal memory
    u8g2.drawFrame(xPos, yPos, width, height); //draw a frame around the border
    u8g2.setCursor(xPos+15, yPos+25);
    u8g2.printf("%dx%d", width, height);
    u8g2.sendBuffer(); // transfer internal memory to the display
    delay(2000);
    
    
    //now e print the ip last digits
    u8g2.clearBuffer();
    u8g2.setCursor(xPos+leftMargin, yPos+TOP_MARGIN);
    u8g2.setFont(u8g2_font_helvB18_tr); // set the regular font
    IPAddress ip = WiFi.localIP();
    int lastOctet = ip[3];  // Last part of the IP address
    u8g2.print("IP:" + String(lastOctet % 1000));
    u8g2.sendBuffer(); 
    delay(2000);
    Serial.println("printDisplay() ready");
}

void writeDisplay() {
consoleOut("writeDisplay " + String(powerBalance, 1));  
//consoleOut("writeDisplay leftMargin " + String(leftMargin));
//consoleOut("writeDisplay xPos " + String(xPos));
//consoleOut("writeDisplay yPos " + String(yPos));
u8g2.clearBuffer(); // clear the internal memory
u8g2.setCursor(xPos+leftMargin, yPos+TOP_MARGIN); // 28 * 58
u8g2.setFont(u8g2_font_helvB18_tr);  // set the regular font
// this should adapt to -1234.5 removing the .5 so it fits
// if we have 9999.5 it will fit 
// if we have 16700.5 is a problem
if(powerBalance < -999 || powerBalance > 9999) {
u8g2.printf("%.0f", powerBalance); // -2004 will fit
} else {
u8g2.printf("%.1f", powerBalance); // -123.4 will fit  
}
u8g2.sendBuffer();
}
//#endif

bool scanI2C() {
  bool found = false;
  consoleOut("Scanning I2C addresses...");
  int nDevices = 0;
  byte error, address;
    for (byte address = 1; address < 127; ++address) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      consoleOut("I2C device found at 0x");
      consoleOut(String(address, HEX));
      found = true;
    }        
  }
  consoleOut("scanning done");
  if(found == true) { return true; } else { consoleOut("scan has no results"); return false; } 
}