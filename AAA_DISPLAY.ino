/* we want to show the power balance value on a display
 *  th display is oled with ic2
 *  vin and ground
 *  scl (gpio22
 *  sda (gpio21
 */
// #ifdef SSD1306_JO
// void printDisplay()
// {  
// // this is a screen animation at the start of the program
// // we print a rectangle with the display dimansions and than the IP
// //#define SCREEN_WIDTH 128 //#define SCREEN_HEIGHT 64
//     display.setCursor(0,0);
//     display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, DISPLAY_COLOR);
//     display.drawRect(4, 4, SCREEN_WIDTH-8, SCREEN_HEIGHT-8, DISPLAY_COLOR);
//     display.setCursor(14,26);
//     display.setTextSize(2);
//     display.print("128 x 64");
//     display.display();
//     delay(2000);
    
//     display.clearDisplay();
//     display.setTextSize(1);
//     display.setCursor(4,1);
//     display.print("hansiart software");
//     display.setTextSize(2);
 
//     //IPAddress ip = WiFi.localIP();
//     //int lastOctet = ip[3];  // Last part of the IP address
//     //display.print("IP = " + String(lastOctet % 1000));
//     display.setCursor(10,10);
//     display.print("IP = ");
//     //String ipStr = "IP = " + WiFi.localIP().toString();
//     //display.print(ipStr);
//     display.setCursor(10,30);
//     display.print( WiFi.localIP().toString().c_str() );
//     display.display();    
//     delay(2000);
//     display.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, DISPLAY_COLOR);
//     display.display();
// }

// void writeDisplay()
// { 
// // writes to the SSD1306
//   consoleOut("printing to the oled");
//   if(displayPresent == false) {
//     Serial.println("no display connected");
//     return;
//  }
//       display.clearDisplay();
//       // screen config from deepSeek
//       // Totaal vermogen (grote letters)
//       display.setTextSize(3);
//       display.setCursor(0, 0);
//       display.print(powerBalance);
//       display.setTextSize(2);
//       display.print(" W");

//       // Fase-specifieke waarden (kleine letters)
//       display.setTextSize(1);

//       // L1 (vermogen + stroom)
//       display.setCursor(0, 30);
//       display.print("L1:");
//       display.print(power_l1);
//       display.print("W ");
//       display.print(current_l1, 1);
//       display.print("A");

//       // L2 (vermogen + stroom)
//       display.setCursor(0, 40);
//       display.print("L2:");
//       display.print(power_l2);
//       display.print("W ");
//       display.print(current_l2, 1);
//       display.print("A");

//       // L3 (vermogen + stroom)
//       display.setCursor(0, 50);
//       display.print("L3:");
//       display.print(power_l3);
//       display.print("W ");
//       display.print(current_l3, 1);
//       display.print("A");

//       display.display(); 
// } 
// #endif 

// #ifdef SG110X_HA

//  void printDisplay() //
// {  
// // this is a screen animation at the start of the program
// // we print a rectangle with the display dimansions and than the IP
// //#define SCREEN_WIDTH 128 //#define SCREEN_HEIGHT 64
//     display.clearDisplay();
//     display.setTextColor(DISPLAY_COLOR); // Draw white text
//     display.setCursor(0,0);
//     display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, DISPLAY_COLOR);
//     display.drawRect(4, 4, SCREEN_WIDTH-8, SCREEN_HEIGHT-8, DISPLAY_COLOR);
//     display.setCursor(12,26);
//     display.setTextSize(2);
//     display.print("128 x 64");
//     display.display();
//     delay(2000);
       
//     display.clearDisplay();
//     display.setTextSize(1);
//     display.setCursor(10,1);
//     display.print("hansiart software");
//     display.setCursor(4,26);    
//     display.setTextSize(2);
//     //display.print("IP = ");
//     IPAddress ip = WiFi.localIP();
//     int lastOctet = ip[3];  // Last part of the IP address
//     //String ipStr = "IP = " + WiFi.localIP().toString();
//     display.print("IP = " + String(lastOctet % 1000));
//     //display.setCursor(4,30);
//     //display.print( WiFi.localIP().toString().c_str() );
//     //display.print(ipStr);
//     display.display();    
//     delay(2000);
//     display.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, DISPLAY_COLOR);
//     display.display();
// }
 
 
//  void writeDisplay() {
//   if(displayPresent == false) {
//     Serial.println("no display connected");
//     return;
//    }  
//   consoleOut("printing to the oled");
//   display.clearDisplay(); //
//   display.setTextColor(DISPLAY_COLOR); // Draw white text
 
//   display.setCursor(6, 1); 
//   display.setTextSize(1);
//   display.println("actual power balance");
//   //String powerBalance = String(
//   display.setTextSize(3);
//   display.setCursor(leftMargin, 26); 
//   if(polled){
//   display.println(powerBalance, 0);
//   } else {
//    display.println("-N/A-"); 
//   }
//   display.display();
//   delay(100);
// }
// #endif


