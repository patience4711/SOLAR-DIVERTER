//<style>
//#menu2 a:link, #menu2 a:visited { width: 98px; }
//</style>
//{ check
const char MENUPAGE[] PROGMEM = R"=====(
<body>

<script type="text/javascript" src="SECURITY"></script>
<script> function close() {window.location.href='/';} </script>
<div id='msect'>
<div id='menu'>
<a href="/" class='close' onclick='close();'>&times;</a>  
<a href="/DIMMERCONFIG">dimmer</a>
</div>  
</div>
<br><br>
<div id='msect'><kop>ESP32C3 SOLAR DIVERTER MENU</kop></div><br><br><br>

<div id='msect'>
<div id="menu2">
<table><tr>
  <td style="width:100px"><a href='/ABOUT'>system info</a></td><td style="width:60px"></td><td style="width:100px">
  <a href='/MQTT'>mosquitto</a></<td></tr>
  
  <tr><td style="width:100px"><a href='/CONSOLE'>console</a></td><td style="width:60px"></td><td style="width:100px">
  <a href='/GEOCONFIG'>time config</a></<td></tr>
    
  <tr><td style="width:100px"><a href='/BASISCONFIG'>settings</a></li></td><td style="width:60px"></td><td style="width:100px">
  <a href='/FWUPDATE'>fw update</a></<td></tr>  
  
  <tr><td><a onclick="return confirm('are you sure?')" href='/STARTAP'>start AP</a><td style="width:60px"></td><td style="width:100px">
  <a onclick="return confirm('are you sure?')" href='/REBOOT'>reboot</a></td></tr>
  
  </table>  
  </div>
  </div>
  
</body></html>
  )=====";

//} 
