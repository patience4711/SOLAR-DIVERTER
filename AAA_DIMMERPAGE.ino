//{ check

const char DIMMERCONFIG[] PROGMEM = R"=====(
<body>
<div id='msect'><div id='bo'></div>
<div id="menu">

<a href="#" id="sub" onclick='submitFunction()'>save</a>
<a href="#" class='close' onclick='cl();'>&times;</a>
</div>
</div>
<div id='msect'>
<br><kop>SOLAR DIVERTER DIMMER SETTINGS</kop><br><br>
</div>
<div id='msect'>
  <div class='divstijl' style='height:52vh;'>
  <form id='formulier' method='get' action='submitform' oninput='showSubmit()'>
  <center><table>
  <tr><td style='width:140px;'>dimmer behavior<td><select name='dset' class='sb1'>
    <option value='0' dset_0>switched off</option>
    <option value='1' dset_1>dimming</option>
    <option value='2' dset_2>switched on</option>
    </select></tr>
  <tr><td>heaters number<td><input class='inp3' type='number' min='0' max='3' name='helnbr' length='8' value='{helnbr}'></input></tr>
  <tr><td>heater power<td><input class='inp3' name='hepwr' length='8' value='{hepwr}'></input></tr>
  <tr><td>heater calibration % <td><input class='inp3' type='number' min='-20' max='+20' name='calibr' length='8' value='{calibr}'></input></tr>
  <tr><td>nightly heating<td><input type='checkbox' style='width:30px; height:30px;' name='nighth' #chack></input></tr>
  <tr><td>night heating start<td><input class='inp4' type='time' name='swon' value='{inx}' title='hh:mm'></input></tr>
  <tr><td>night heating stop<td><input class='inp4' type='time' name='swof' value='{ofx}' title='hh:mm'></input></tr>
  </table> 
  
  </div></div><br></form>
<br></body></html>
)=====";

// <tr><td>heating str.:&nbsp<td><select name='fm' class='sb1' id='sel' >
//     <option value='1' fm_0>first element</option>
//     <option value='1' fm_1>second element</option>
//     <option value='1' fm_1>both evenly</option>
//     </select>

void zendPageDIMMERconfig( AsyncWebServerRequest *request ) {

    String webPage = FPSTR(HTML_HEAD);
    webPage += FPSTR(DIMMERCONFIG);  
    
    // put back the data
    webPage.replace("{helnbr}",  String(elementNumber) );
    webPage.replace("{hepwr}",  String(elementPower) );
    webPage.replace("{calibr}",  String(caliBration) );
    webPage.replace("{inx}"  , String(switchOn));
    webPage.replace("{ofx}" , String(switchOff));
  int putBack = dimmerEnabled; // during nightheating this value is 5 greater
  if (putBack > 2) putBack -= 5;
  switch (putBack) 
      { 
          case 0:
            webPage.replace("dset_0", "selected");
            break;
          case 1:
            webPage.replace("dset_1", "selected");
            break;
          case 2:
            webPage.replace("dset_2", "selected");
      } 
     if (nightHeat) { 
    //  Serial.println("dts = true");
      webPage.replace("#chack", "checked");
      }    
        request->send(200, "text/html", webPage);
        webPage="";
}

//void handleGEOconfig(AsyncWebServerRequest *request) {
////char static_ip2[16] = "";
//
//  //de serverargumenten verzamelen
////strcpy(lengte, request->getParam("le")->value().c_str());
//
//longi = request->getParam("le")->value().toFloat();
//
////strcpy(breedte, request->getParam("be")->value().c_str());
//
//lati = request->getParam("be")->value().toFloat();
//
//strcpy(gmtOffset, request->getParam("tz")->value().c_str());
//
//
////BEWARE CHECKBOX
//String dag = "";
//if(request->hasParam("ts")) {
//dag = request->getParam("ts")->value();  
//}
//   if (dag == "on") { dts = true; } else { dts = false;}
//   
//      wifiConfigsave();
//
//      actionFlag=25; // recalculate with these settings 
//} 