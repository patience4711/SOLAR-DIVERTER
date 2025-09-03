// pinout:  https://www.studiopieters.nl/esp32-c3-super-mini-pinout/

# define TEST


#include <WiFi.h>
#include <esp_wifi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <AsyncEventSource.h>
#include <AsyncWebSocket.h>
#include <DNSServer.h>

HardwareSerial p1Serial(2);

 
#include "OTA.h"
#include <Update.h>
//#include <Hash.h>

#include <TimeLib.h>
#include <time.h>
#include <sunMoon.h>

#include "soc/soc.h" // ha for brownout
#include "soc/rtc_cntl_reg.h" // ha for brownout
#include <esp_task_wdt.h> // ha



#include <SPIFFS.h>
#include "FS.h"

#include <EEPROM.h>

#include <ArduinoJson.h>
#include <Arduino.h>

AsyncWebServer server(80); //
AsyncEventSource events("/events"); 
AsyncWebSocket ws("/ws");

#include <PubSubClient.h>

#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
const byte DNS_PORT = 53;
DNSServer dnsServer;

//#include "CRC16.h"

// #include "HTML.H"
// #include "AAA_MENUPAGE.H"
// #include "AAA_HOMEPAGE.H"
// #include "PORTAL_HTML.H"
// #include "REPORT.H"

// includes for the oled display
//#define SSD1306_JO // johans oled
#include <Wire.h>
#include <Adafruit_GFX.h>

// #ifdef SG110X_HA
// #include <Adafruit_SH110X.h>
// Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &Wire, -1); // 128x64 SH1106 display
// #define DISPLAY_COLOR    SH110X_WHITE
// #define SCREEN_WIDTH 128
// #define SCREEN_HEIGHT 64
// #endif
// #ifdef SSD1306_JO
// #include <Adafruit_SSD1306.h>
// #define SCREEN_WIDTH 128
// #define SCREEN_HEIGHT 64
// #define OLED_RESET -1
// #define SDA_PIN 7
// #define SCL_PIN 6
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// #define DISPLAY_COLOR  SSD1306_WHITE
// #endif


#include <U8g2lib.h>
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, 6, 5);
// the display = 72 x 40
int xPos = 28; // found with oledtest
int yPos = 24; // found with oledtest for u8g2_font_helvB18_tr
#define TOP_MARGIN 34



bool displayPresent = false;
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <Update.h>
//#include <Hash.h>

#include <TimeLib.h>
#include <time.h>
#include <sunMoon.h>

#include "soc/soc.h" // ha for brownout
#include "soc/rtc_cntl_reg.h" // ha for brownout
#include <esp_task_wdt.h> // ha

#include <SPIFFS.h>
#include "FS.h"

#include <EEPROM.h>

#include <ArduinoJson.h>
#include <Arduino.h>
#include <AsyncTCP.h>

#include <PubSubClient.h>

#include <NTPClient.h>
#include <WiFiUdp.h>

//#include "CRC16.h"

#include "HTML.h"
#include "AAA_MENUPAGE.h"
#include "AAA_HOMEPAGE.h"
#include "PORTAL_HTML.h"
//#include "REPORT.H"

//#include <TriacDimmer.h>
//#include <RBDdimmer.h>  // Download from https://github.com/RobotDynOfficial/RBDDimmer
//#include <RBDdimmer.h>  // RobotDyn Dimmer library

// ==== Pin Definitions (adapt to your board!) ====
#define ZC_PIN      4    // Zero-cross detector input
#define TRIAC1_PIN  5    // Triac channel 1 output
#define TRIAC2_PIN  6    // Triac channel 2 output
// ==== Control Variables ====
volatile bool zc_flag = false;     // Set on each zero-cross
volatile uint16_t cycle_count = 0; // Counts mains cycles
// Power levels (0–100%)
//volatile uint8_t power_ch1 = 0;   // 50% power
//volatile uint8_t power_ch2 = 0;   // 75% power
float(duty_ch1);
float(duty_ch2);
// Burst length in cycles (e.g., 20 cycles ≈ 0.4s at 50Hz)
const uint16_t BURST_WINDOW = 20;

  char ssid[33] = ""; // was 33 
  char pass[64] = ""; // was 40
  bool tryConnectFlag = false;

 // variables wificonfig
  char pswd[11] = "0000";
  char userPwd[11] = {"1111"};  
  float longi = 5.123;
  float lati = 51.123;
  char gmtOffset[5] = "+60";  //+5.30 GMT
  bool dts = true;
  int procesId = 1;
  char requestUrl[12] = {"/"}; // to remember from which webpage we came  

// variables mqtt ********************************************
  char  Mqtt_Broker[30]=    {"192.168.0.100"};
  char  Mqtt_outTopic[26] = {"domoticz/in"}; // was 26
  char  Mqtt_Username[26] = {""};
  char  Mqtt_Password[26] = {""};
  char  Mqtt_Clientid[26] = {""};
  int   Mqtt_Port =  1883;
  int   Mqtt_Format = 0;
  int   powerIdx = 0;
  int   absorbIdx = 0;
// variables dimmer
  int elementPower=100;
  char switchOn[6] = "00:00";
  char switchOff[6] = "00:00";
  bool nightHeat = false;
  uint8_t heatingStrategy = 3;
  uint8_t elementNumber = 1;
  int dimmerEnabled = 0;
  int caliBration = 0;

// variables domoticz ********************************************
  char dom_Address[30]={"192.168.0.100"};
  int  dom_Port =  8080;

//  int  el_Idx = 456;


  //bool nightTime = false; 
  int event = 0;
  
  int dst;
  bool timeRetrieved = false;
  int networksFound = 0; // used in the portal
  int datum = 0; //

  unsigned long previousMillis = 0;        // will store last temp was read
  static unsigned long laatsteMeting = 0; //wordt ook bij OTA gebruikt en bij wifiportal
  static unsigned long lastCheck = 0; //wordt ook bij OTA gebruikt en bij wifiportal
  
#define LED_AAN    LOW   //sinc
#define LED_UIT    HIGH
#define knop        0  //
#define led_onb     8  // onboard led was 2
#define ledpin      10 // 

//unsigned char sync = 10; //sync pin
//unsigned char channel_1 = 5; // channel 1 pin
//unsigned char channel_2 = 6; // channel 2 pin

String toSend = "";
String debugLog = "";
int value = 0; 
int aantal = 0;



WiFiClient espClient ;
PubSubClient MQTT_Client(espClient) ;

//int Mqtt_stateIDX = 0;

// Vars to store meter readings
float active_power_w = 0;  //Meter reading power - Actual consumption i fase
float power_l1 = 0;  //Meter reading power - Actual return
float power_l2 = 0;
float power_l3 = 0;
// if there is a 3pase meterr
float current_l1 = 0;
float current_l2 = 0;
float current_l3 = 0;
float powerBalance = 0;


uint8_t diagNose = 0;
int actionFlag = 0;
char txBuffer[50];
char reqApi[30];
//bool USB_serial = true;
bool threePhase = false;
bool polled = false;
bool Polling = false;
int pollFreq = 30;
// voor testing
//int www = 0;
char timeStamp[12]={"not polled"};
int tresHold = 2000;
int errorCode = 0;
int securityLevel = 0;
int leftMargin = 2;
#include <Ticker.h>
Ticker resetTicker;
int failCount = 0;
// ISR: Triggered on each zero-cross
void IRAM_ATTR onZeroCross() {zc_flag = true;}

// *****************************************************************************
// *                              SETUP
// *****************************************************************************

void setup() {

  Serial.begin(115200);
  delay(200);
  
  Serial.println("ESP32C3-SOLAR_DEVIDER booting");

  pinMode(TRIAC1_PIN, OUTPUT);
  pinMode(TRIAC2_PIN, OUTPUT);
  pinMode(ZC_PIN, INPUT);
 
  //pinMode(knop, INPUT_PULLUP); // the button gpio4 D2 
  pinMode(led_onb, OUTPUT); // onboard led gpio0 D3
  pinMode(ledpin, OUTPUT);// with this pin gpio14 (D5)  
  digitalWrite(ledpin, LOW);
  ledblink(1, 800);
// #if defined SG110X_HA || defined SSD1306_JO
//   Wire.begin(7, 6);  // SDA = GPIO 7, SCL = GPIO 6
// #endif

// #ifdef SG110X_HA
//   if(!display.begin(0x3C, true)) {
//      Serial.println("SH110 allocation failed");
//   } else {
//      Serial.println("SH110 allocation success");
//      displayPresent = true;
    
//   }
// #endif

// #ifdef SSD1306_JO
//     if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
//     Serial.println(F("SSD1306 allocation failed"));
//   } else {
//     Serial.println(F("SSD1306 allocation OK"));
//     displayPresent = true;
//     display.setTextColor(WHITE);
//   }
// #endif  
  
  attachInterrupt(digitalPinToInterrupt(knop), isr, FALLING);
  
  SPIFFS_read();
 
  start_wifi(); // start wifi and server

  getTijd(); // retrieve time from the timeserver
  
  displaySetup();
  consoleOut("check for a display, we scan an adres");  
  if( scanI2C() == true ) {
     consoleOut("the scan found an I2C display connected");
     displayPresent = true;
     printDisplay();
  
  } else {
     consoleOut("the scan had no results so there is no display");
     consoleOut("check the wiring, display type should be I2C (not SPI)");
     displayPresent = false;
  }
  actionFlag = 27; // set the dimmer behaviour
  // if (dimmerEnabled == 1) {
  //     attachInterrupt(digitalPinToInterrupt(ZC_PIN), onZeroCross, RISING);
  // } else 
  //     if(dimmerEnabled == 0) {
  //     digitalWrite(TRIAC1_PIN, LOW);
  //     digitalWrite(TRIAC2_PIN, LOW);
  // } else
  //     if(dimmerEnabled == 2) {
  //     digitalWrite(TRIAC1_PIN, HIGH);
  //     digitalWrite(TRIAC2_PIN, HIGH);
  // }
  
  // if(displayPresent) {
  // consoleOut("displayPresent = true");
  // printDisplay(); // print the rectangle with screen dimensions and IP
  // }

  
//  // ****************** mqtt init *********************
       MQTT_Client.setKeepAlive(150);
       MQTT_Client.setBufferSize(512);
       MQTT_Client.setServer(Mqtt_Broker, Mqtt_Port );
       MQTT_Client.setCallback ( MQTT_Receive_Callback ) ;

  if ( Mqtt_Format != 0 ) 
  {
       consoleOut("setup: mqtt configure");
       mqttConnect(); // mqtt connect
  } 
  else 
  {
   consoleOut("mqtt not enabled"); 
  }
  initWebSocket();

  Serial.println(F("esp32c3 solar-diverter booted up"));
  Serial.println(WiFi.localIP());

  delay(1000);
  //ledblink(3,500);

  eventSend(1);
  if ( !timeRetrieved )   getTijd();

// Initialize both the dimmers
//  dimmer1.begin(NORMAL_MODE, ON);
//  dimmer2.begin(NORMAL_MODE, ON);

  consoleOut("Dual-channel dimmer ready (ESP32-C3).");


  if(Polling) Poll(); //we do the first poll
} // end setup()

//****************************************************************************
//*                         LOOP
//*****************************************************************************
void loop() {

// the flag is set by the interrupt
if (zc_flag) updateDimmer();
// there is only nightTime when nightheat = set.
// So during this period we omit this test
if(dimmerEnabled == 0)
{
  if(digitalRead(TRIAC1_PIN) != LOW || digitalRead(TRIAC2_PIN) != LOW)
  {
    digitalWrite(TRIAC1_PIN, LOW);
    digitalWrite(TRIAC2_PIN, LOW);
  }
}


//if(moetZenden) send_testGram();
//              polling every 60 seconds 
// ******************************************************************

  unsigned long nu = millis();  // the time the program is running
   if (nu - laatsteMeting >= 1000UL * pollFreq) 
   {
     
      //String toLog = String(pollFreq) + " seconds passed";
      consoleOut( "seconds passed : " + String(pollFreq));
      laatsteMeting += 1000UL * pollFreq ; // 
      if(Polling) Poll();  
  }

  // when there is a new date the time is retrieved
  // if retrieve fails, day will not be datum, so we keep trying
  if (day() != datum) // if date overflew 
  { 
       getTijd(); // retrieve time and recalculate the switch times
  }
  
    if (nightHeat)  
    {
        // we must check the time == true when between in and out time
        if (checkTime())
        {
            if (dimmerEnabled < 4) 
            {
                //nightTime = true;  // prevent repetition
                // we make dimmerEnabled temporarily 5 higher to prevent repetition and to find the original situation
                dimmerEnabled += 5; 
                // we know we are in the defined time period
                detachInterrupt(digitalPinToInterrupt(ZC_PIN)); // prevent interrupts during the isr
                delay(50);
                digitalWrite(TRIAC1_PIN, HIGH);  
                digitalWrite(TRIAC2_PIN, HIGH);
                duty_ch1 = 1.0f;
                duty_ch2 = 1.0f;  
                consoleOut("enter Night mode: dimmer full on");
            }  
        } 
        else // not checktime we should switch off when we are on 
        {
            if (dimmerEnabled > 2) 
            {
                //nightTime = false;  // prevent repetition
                dimmerEnabled -= 5; // now we have the original value back
                digitalWrite(TRIAC1_PIN, LOW); 
                digitalWrite(TRIAC2_PIN, LOW);
                // reset to situation before nightTime
                actionFlag = 27;
                //attachInterrupt(digitalPinToInterrupt(ZC_PIN), onZeroCross, RISING);
                consoleOut("leave Night mode");
            }
        }
    }   
    else  // not nightHeat
    { 
        // if the user changed this during nightTime we must re-start normal dimming  
        if (dimmerEnabled > 2) 
        {
            dimmerEnabled -= 5;  // prevent repetition
            actionFlag = 27;
            consoleOut("user changed nightHeat");
        }
    }

  // we do this before sending polling info and healthcheck
  // this way it can't block the loop
  if(Mqtt_Format != 0 ) MQTT_Client.loop(); //looks for incoming messages
 
  //*********************************************************************
   // is powerbalane > treshold we put a led on
   // powerBalance = negative 
   // compare pb = -5 to th = 10 (10 + -5 yields 5
   // th + pb < 0;
  //put the signal led on when we are over the treshold
  //if(tresHold + powerBalance < 0) digitalWrite(ledpin, HIGH); else digitalWrite(ledpin, LOW);
  
  test_actionFlag();
  
//  if( Serial.available() ) {
//    empty_serial(); // clear unexpected incoming data
//   }

   ws.cleanupClients();
   yield(); // to avoid wdt resets

//  SERIAL: *************** check if there is data on serial **********************
  if(Serial.available()) {
       handle_Serial();
   }

}
//****************  End Loop   *****************************









void flush_wifi() {
     consoleOut(F("erasing the wifi credentials"));
     WiFi.disconnect();
     consoleOut("wifi disconnected");
     //now we try to overwrite the wifi credentials     
     const char* wfn = "dummy";
     const char* pw = "dummy";
     WiFi.begin(wfn, pw);
     consoleOut(F("\nConnecting to dummy network"));
     int teller = 0;
     while(WiFi.status() != WL_CONNECTED){
        Serial.print(F("wipe wifi credentials\n"));
        delay(100);         
        teller ++;
        if (teller > 2) break;
    }
    // if we are here, the wifi should be wiped 
}


void eventSend(byte what) {
  if (what == 1) {
      events.send( "getall", "message"); //getGeneral triggered            
  } 
}


//}
