//<link rel="icon" type="image/x-icon" href="/favicon.ico" />
//<script type="text/javascript" src="SECURITY"></script>
//
const char ABOUT [] PROGMEM = R"=====(
<!DOCTYPE html><html><head>
<meta http-equiv="refresh" content="180">
<meta name="viewport" content="width=device-width, initial-scale=1">
<meta charset='utf-8'>
<link rel="stylesheet" type="text/css" href="/STYLESHEET">
<title>ESP-P1 METER</title>
<style>
table, th, td {border: 1px solid blue; width:font-size:12px; background-color: #ffffcc; }
th, td { padding-left: 4px; padding-right: 4px;}
th {background-color: #22ffb3;}
body {font-size:12px;} tr {height:26px;} 
</style>
<script type="text/javascript" src="SECURITY"></script>
<script> function cl() { window.location.href='/MENU'; }</script>
</head><body>
<div id='msect'>
<div id='menu'>
<a href="#" class='close' onclick='cl();'>&times;</a>
</div></div>
<center>
<h2>SOLAR DIVERTER DATA</h2>

)=====";

void handleAbout(AsyncWebServerRequest *request) {
  char page[1536] = {0};
  char temp[100]={0};
  strcpy_P(page, ABOUT);

  int minutens = millis()/60000;
  int urens = minutens/60;
  int dagen = urens/24;
 
  strcat(page, "<br><table><tr><TH colspan='2'>ESP SYSTEM INFORMATION</th></tr>" );
  #ifdef TEST
  strcat(page, "<tr><td>firmware version<td>ESP32C3-SOLAR_DIVERTER-TEST</tr>");
  #else
  strcat(page, "<tr><td>firmware version<td>ESP32C3-SOLAR_DIVERTER-v0_0</tr>");
  #endif
  if ( timeRetrieved ) strcat(page,"<tr><td>time retrieved<td>yes</tr>"); else strcat(page,"<tr><td>time retrieved<td>no</tr>");
  sprintf(temp, "<tr><td>systemtime<td> %d:%d " , hour(), minute());
  switch (dst) {
    case 1: strncat(temp, "summertime</td>", 19 ); break;
    case 2: strncat(temp, "wintertime</td>", 19 ); break;
    case 0: strncat(temp, "no dst set</td>", 19 ); 
  }
  strcat(page, temp);
  
  sprintf(temp, "<tr><td>system uptime<td>%d d %d h %d m </td>", dagen, urens-dagen*24, minutens-urens*60);
  strcat(page, temp);
  sprintf(temp, "<tr><td>wifi signalstrength<td>%lddB</td>", WiFi.RSSI());
  strcat(page, temp);
  sprintf(temp, "<tr><td>ESP CHIP ID nr: <td> %ld</td>", getChipId(true) );
  strcat(page, temp);
  sprintf(temp, "<tr><td>Free heap<td> %u bytes</td>", ESP.getFreeHeap() );
  strcat(page, temp);

  if ( Mqtt_Format != 0 ) { 

    // check if connected
    if ( MQTT_Client.connected() ) { //: add a dot
       sprintf(temp, "<tr><td>mqtt connected<td>%s</td>", Mqtt_Broker);
       } else {
       sprintf(temp, "<tr><td>mqtt status<td>not connected</td>");
       }
   } else {
       sprintf(temp, "<tr><td>mqtt status<td>not configured</td>");
   }
   strcat(page, temp);
  

    sprintf(temp, "<tr><td>failCount<td>%d</td>", failCount); 
    strcat(page, temp);
//  sprintf(temp, "<tr><td>ZB resetCounter<td> %d</td></tr></table>" , resetCounter );
//  strcat(page, temp);
   sprintf(temp, "<tr><td>securityLevel<td>%d</td>" , securityLevel );
   strcat(page, temp);
   request->send(200, "text/html", page); //send the html code to the client
   memset(page, 0, sizeof(page));
}
