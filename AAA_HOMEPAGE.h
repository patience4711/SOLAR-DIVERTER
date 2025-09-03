//{ test for sysntax
//<input type="number" id="invCount" value="%count%" hidden>
//<script type="text/javascript" src="SECURITY"></script>
// <li><a href='/LOGPAGE'>log</a></li>
  //<li><a href='/INFOPAGE'>info</a></li>  

//  <li><a href='/ABOUT'>about</a></li>
//  <li><a href='/LOGPAGE'>journal</a></li>
  
//  <li id="ml" style='float:right; display:none'><a id='haha' href='/MENU'>menu</a></li>
//<link rel="icon" type="image/x-icon" href="/favicon.ico" />
const char P1_HOMEPAGE[] PROGMEM = R"=====(
<!DOCTYPE html><html><head>

<meta charset='utf-8' name="viewport" content="width=device-width, initial-scale=1">
<title>ESP32-SOLAR_DIVERTER</title>
<link rel="icon" type="image/x-icon" href="/favicon.ico" />
<link rel="stylesheet" type="text/css" href="/STYLESHEET">  
<style>
body {
 background-color: #EEE;
}
span {
  padding: 6px;
}
table, th, td {
  border: 2px solid blue; font-size:16px; padding:6px; text-align:center; border-collapse:collapse;
  }
tr {background-color:#ccffcc;}

#td0, #td00 {width:170px;}
#td1, #p22, #p23, #p24{width:110px;}
#td2, #td02 {width:40px;}
.btn {
  background-color: #199319;
  color: white;
  padding: 5px 22px;
  border-radius:6px;
}

.btn:hover {background: #eeeF; color:black;}

/* Add animation (fade in the popup) */
@-webkit-keyframes fadeIn {
  from {opacity: 0;} 
  to {opacity: 1;}
}

@keyframes fadeIn {
  from {opacity: 0;}
  to {opacity:1 ;}
}

@media only screen and (max-width: 800px) {
th, td { font-size:11px; }
#td0, #td00 {width:120px}
#td1, #p22, #p23, #p24 {width:100px;}
#td2, #td02 {width:40px;}


tr {height:35px;} 
.btn { padding: 5px 18px; font-size:10px;}
}
</style>


</head>

<body onload='loadScript()'>

<div id='msect'>
<div id='menu'>
<a id="ml" href='/MENU' style='float:right; display:none'>menu</a>
</div>
</div>
<div id='msect'>
  <kop>ESP32C3 SOLAR DIVERTER</kop>
</div>
<div id='msect'>
    <div class='divstijl' id='maindiv'>
        <center>
        
        <br><h4>POWER USAGE @ <span id="time"></span></h4><br>
        
        <div id='noOutput' style='display:block'>
          <h4 style='color:red'>waiting for output</h4>
        </div><br><br>
        
        <div id="4channel">
          <center><table id="tafel">
          <tr style='Background-color:lightblue; font-weight:bold; text-align:center; border:4px solid black;'>
          <td id="td0">DESCRIPTION<td id="td1">AMOUNT<td id="td2">UNIT</td></tr>
          <tr><td>active power balance</td><td id="p21"></td><td>W</td>
          <tr><td id="td00">active power phase1</td><td id="p22"></td><td id="td02">W</td></tr>
          <tr><td>active power phase2</td><td id="p23"></td><td>W</td>
          <tr><td>active power phase3</td><td id="p24"></td><td>W</td>
          <tr><td>load consuming</td><td id="p25"></td><td>W</td>
        </table>
        </div>  
        <br><br><p>Powered by Hansiart</p>
             
        </center>
    </div>
<script type="text/javascript" src="JAVASCRIPT"></script>
</div></body>

</html>
)=====";

const char JAVA_SCRIPT[] PROGMEM = R"=====(

function loadScript() {
 //init() 
 getData();
}


window.addEventListener('visibilitychange', () =>{
    if (document.visibilityState === 'visible') {
    //window.location.reload();}
    getData();
    }
})

function celbgc(cel) {
     document.getElementById(cel).style = "background-color:#c6ff1a";  
}
function celbga(cel) {
     document.getElementById(cel).style = "background-color:#f59b0a";  
}

function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() 
  {
    if (this.readyState == 4 && this.status == 200) {
      var antwoord = this.responseText;
      var obj = JSON.parse(antwoord);
      document.getElementById("time").innerHTML = obj.timestamp;
      po = obj.polled;
      mType = obj.meterType;
      var ap1 = obj.power_l1;
      var ap2 = obj.power_l2;
      var ap3 = obj.power_l3;
      var load = obj.load; 
      var apBal = obj.powerBalance;    
      //var aPo = obj.aPo;
      var rem = obj.rm;
      if(rem == 0) {document.getElementById("ml").style.display = "block";} // show menu link         
      
      cel="p21"; //
      if(apBal < 0 ) celbga(cel); else celbgc(cel); // set the color
      // we always show the powerbalance value if polled
      if(po == "1") {
           document.getElementById(cel).innerHTML = apBal;
           document.getElementById("noOutput").style.display = "none";
           } else {
           document.getElementById(cel).innerHTML = "n/a";
           document.getElementById("noOutput").style.display = "block";
           } 
      cel="p25";
      if(load < 0 ) celbga(cel); else celbgc(cel); // set the color
      if(po == "1") {
          document.getElementById(cel).innerHTML = load;
          document.getElementById("noOutput").style.display = "none";
          } else {
          document.getElementById(cel).innerHTML = "n/a";
          document.getElementById("noOutput").style.display = "block";
          }    

       if(mType == 1){ //we make the extra cells n/a
       for(let x=2; x < 5; x++){ document.getElementById("tafel").rows[x].style.display="none";}
       }
       if(mType == 2) // we fill the numbers in the extra rows
       {
         cel="p22";
           if(po == "1") {
              document.getElementById(cel).innerHTML = ap1;
              } else {
              document.getElementById(cel).innerHTML = "n/a";
           }
         cel="p23";
           if(po == "1") {
              document.getElementById(cel).innerHTML = ap2;
              } else {
              document.getElementById(cel).innerHTML = "n/a";
           }
         cel="p24";
           if(po == "1") {
              document.getElementById(cel).innerHTML = ap3;
              } else {
              document.getElementById(cel).innerHTML = "n/a";
           }                
      
        } 
      }
  }
  xhttp.open("GET", "/get.Data", true);
  xhttp.send();
}


if (!!window.EventSource) {
 var source = new EventSource('/events');

 source.addEventListener('open', function(e) {
  console.log("Events Connected");
 }, false);
 source.addEventListener('error', function(e) {
  if (e.target.readyState != EventSource.OPEN) {
    console.log("Events Disconnected");
  }
 }, false);

 source.addEventListener('message', function(e) {
  console.log("message", e.data);
  if(e.data == "getall") {
  getData();
  }

 }, false);
}

)=====";

//}
