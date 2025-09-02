# SOLAR-DIVERTER
The well known problem with our solar installations that it's only generating energy during daylight, when we probably don't need it.
So it is becoming more and more important to store this energy and use it later when the sun is gone. This is where the solar diverter (s.i.)
can come in handy.
The s.i. is a means of storing excess solar production in a hot-water cylinder (HWC) instead of deliver it to the grid, where it is unwanted and doesn't make any money  
The HWC essentially functions as a cheap solar battery. (total costs < 20 Euro for the electronics)

So how does this work. First of all we need to detect whether we are delivering power to the grid or not. We can do that by polling data from our smart meter (p1 meter).
In this meter we can put a dongle (home wizzard or diy [look here](https://github.com/patience4711/ESP32-C3-READ-P1-METER) that we can request for data via our wifi network. Now we know exactly how much energy we have excess. 
The s.i. now drives a thyristor dimmer open so that exactly the amount of excess the flows into our HWC. The HWC we can put in series with our normal hot water supply. 
As the HWC normally has to heat cold water, the water is already (partially) warmed up, so here can save energy. 

This dimmer can work with thyristors and also with solid state relais. Complete with cooling body they are not very expensive. 

![DIVERTER](https://github.com/user-attachments/assets/24d69c4e-29d5-4c90-af08-9f484eec1a6b)

