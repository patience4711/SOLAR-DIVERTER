// nodemcu pins
//  16  D0  
//   5  D1  
//   4  D2  
//   0  D3
//   2  D4   
//   GND
//   3,3v
//   14  D5  
//   12  D6  
//   13  D7 -> P02
//   15  D8 -> P03
//   3   D9
//   1   D10

// compile settings FS: Minima SPIFFS with OTA 
/* had to modyfy AsyncWebSynchronization.h to solve a compilation error


 i had much trouble to get the display to work, pi 9 and 10 didný work duo to the
 declaration of pin 8 as output.
 now used pin 7 and 6 Wire.begin(7, 6);  // SDA = GPIO 7, SCL = GPIO 6 and that worked
 */
