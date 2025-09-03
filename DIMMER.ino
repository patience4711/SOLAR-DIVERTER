
void adjustDimmer() {
  // in the testing environment we take a virtual powerbalance as meterinput
  // if the real powerBalance = -200, then, when we had a virtual load
  // this would be -200 + currentLoad
  // so if the meterinput = -200 and we have a virtual load of 180
  // the tooMuch value would be 20
  // the solar produces virtually -20
  // Calculate current heater load in watts
  float calibratedPower = elementPower * (1.0f + caliBration / 100.0f);
  int currentLoad = (int)(duty_ch1 * calibratedPower) + (int)(duty_ch2 * calibratedPower);
  Serial.println("initial Current heater load: " + String(currentLoad)); // calculated consumption
  
  #ifdef TEST 
  int init_powerBalance = powerBalance;
  // this is what the meter would report when the dimmer virtually absorbs the excess power
  // say the meter = -200 and the currentload 180 than we get 20
  Serial.println("Initial powerBalance: " + String(init_powerBalance)); // this is what he meter sys
  
  init_powerBalance = init_powerBalance + currentLoad;  // say this is -200 + 180 = -20
  // this is what the meter would say when there was a current load
  Serial.println("correctedpowerBalance: " + String(init_powerBalance)); 
  int virtualSurplus = -(init_powerBalance) + currentLoad;  // = 20+180 = 200 correct
  #else
  int virtualSurplus = -(powerBalance) + currentLoad;  // = 20+180 = 200 correct
  #endif  
   
  // Virtual surplus: what virtually is total surplus, 
  // this should be consumed so his is where the calculation is based on
  // Clamp to zero (cannot absorb negative)
  if (virtualSurplus < 0) virtualSurplus = 0;
  // Calculate duty per channel using absolute control
  
  float needed = (float)virtualSurplus / calibratedPower;
  //needed *= (1.0f + calibration / 100.0f); // adjust with a percentage
  // Assign to channels
  switch (elementNumber) {
   case 1: 
        //oneElement(needed);
        duty_ch1 = min(1.0f, needed);
        break;
   case 2: 
        float share = needed / 2.0f;
        duty_ch1 = min(1.0f, share);
        duty_ch2 = min(1.0f, share);
        break; 
   //case 3: 
   //     evenlyElements(needed);         
  }
  //duty_ch1 = min(1.0f, needed);
  //duty_ch2 = max(0.0f, min(1.0f, needed - 1.0f));

  #ifdef TEST
  // the currentLoad now becomes
  
  currentLoad = (int)(duty_ch1 * calibratedPower) + (int)(duty_ch2 * calibratedPower);
  Serial.println("Adjusted load: " + String(currentLoad)); // calculated consumption
  
  //powerBalance = powerBalance + currentLoad;
  //consoleOut("the powerBalance adjusted with new virtualLoad = " + String(powerBalance));
  // this value is send via mqtt and comes on the screen and should be null 
  #else
  Serial.println("Current heater load: " + String(currentLoad)); // calculated consumption
  #endif
    // Optional: print for debugging
   
  Serial.println("Virtual surplus: " + String(virtualSurplus));
  
  consoleOut("Duty_ch1: " + String(duty_ch1));
  consoleOut("channel 1 now will consume " + String(calibratedPower * duty_ch1));
  
  if(elementNumber == 2) {
     consoleOut("Duty_ch2: " + String(duty_ch2));
     consoleOut("channel 2 now will consume " + String(calibratedPower * duty_ch2));
  }
  Serial. println("\n * * * * * * * * * * * \n");
}

// void IRAM_ATTR onZeroCross() {zc_flag = true;}
// // we run checkDimmer when the flag os set
void updateDimmer()
{  
      zc_flag = false;
if(dimmerEnabled != 1) return;
      // --- Channel 1 ---
      if (cycle_count < (duty_ch1 * BURST_WINDOW / 100)) 
      {
        digitalWrite(TRIAC1_PIN, HIGH);  // Fire triac
      } else {
        digitalWrite(TRIAC1_PIN, LOW);
      }

      // --- Channel 2 ---
      if (cycle_count < (duty_ch2 * BURST_WINDOW / 100)) 
      {
        digitalWrite(TRIAC2_PIN, HIGH);
      } else {
        digitalWrite(TRIAC2_PIN, LOW);
      }

      // Update cycle counter
      cycle_count = cycle_count + 1;
      if (cycle_count >= BURST_WINDOW) 
      {
        cycle_count = 0;
      }
      
}

// void oneElement(float needed) {
//   duty_ch1 = min(1.0f, needed);
//   //duty_ch2 = max(0.0f, min(1.0f, needed - 1.0f));
// }
// // void twoElements(float needed) {
// //   duty_ch2 = min(1.0f, needed);
// //   duty_ch1 = max(0.0f, min(1.0f, needed - 1.0f));
// // }
// void twoElements(float needed) {
//   float share = needed / 2.0f;
//   duty_ch1 = min(1.0f, share);
//   duty_ch2 = min(1.0f, share);dump-vars

// }
