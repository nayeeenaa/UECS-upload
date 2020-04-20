char server[] = {"192.168.11.100"} ; //Enter the IPv4 address or webhost address
// FUNCTION FOR SENDING DATA TO SERVER


/************************************
 * Watchdog Function
 ************************************/
void heartbeat() {
  // Sink current to drain charge from watchdog circuit
  pinMode(pulsePin, OUTPUT);
  digitalWrite(pulsePin, LOW);
  delay(500);
  // Return to high-Z
  pinMode(pulsePin, INPUT);
  lastHeartbeat = millis();
  Serial.println("Heartbeat sent");
}
void hbtime(){
  unsigned long uptime = millis();
  if ((uptime - lastUptimeReport) >= 5000) {
    // It has been at least 5 seconds since our last uptime report.  
    Serial.println("Uptime: " + String((uptime - (uptime % 5000)) / 1000) + " seconds (" + String((uptime - lastHeartbeat) / 1000) + " seconds since last heartbeat)");
    // Pretend we did it exactly on the 5 second mark so we don't start slipping.
    lastUptimeReport = (uptime - (uptime % 5000));
  }
}


/************************************
 RTC Time Checking 
 ************************************/
void rtctime(){
  float dt;
  float tm;
  DateTime now = rtc.now();
  // add time from RTC here
  Serial.print("&D=");
  Serial.print(now.year(),DEC);
  
  Serial.print(now.month(),DEC);
  
  Serial.println(now.day(),DEC);
  Serial.print("&T=");
  Serial.print(now.hour(),DEC);
  
  Serial.print(now.minute(),DEC);
  
  Serial.println(now.second(),DEC); 
  
  }








boolean datasend() {
    DateTime now = rtc.now();
    EthernetClient UECSclient;
    boolean statusReply = false;
    boolean requested = false;
    byte savedReply = 0;
    byte ci = 0;
    // connect to server
    if(UECSclient.connect(server, 80)) {
        Serial.print(" ~ ");
        //request http in line
        UECSclient.print("GET /agri/api.php?api=uecs_data");
        UECSclient.print("&key=");
        UECSclient.print("BsJPaOcXLAiZ8aSqBj1cMNjjD8PuCWDE");
        UECSclient.print("&dt=");
        UECSclient.print(now.year(),DEC);
        UECSclient.print(now.month(),DEC);
        UECSclient.print(now.day(),DEC);
        UECSclient.print("&tm=");
        UECSclient.print(now.hour(),DEC);
        UECSclient.print(now.minute(),DEC);
        UECSclient.print(now.second(),DEC);
        UECSclient.print("&ch0=");
        UECSclient.print((double)U_ccmList[CCMID_InAirTemp].value/10, 1);
        UECSclient.print("&ch1=");
        UECSclient.print((double)U_ccmList[CCMID_InAirHumid].value/10, 1);
        UECSclient.print("&ch3=");
        UECSclient.print((float)U_ccmList[CCMID_InSoilM].value/10, 1);
        UECSclient.print("&ch4=");
        UECSclient.print((int)U_ccmList[CCMID_cnd].value);
        UECSclient.print(" HTTP/1.1\n");
        UECSclient.print("Host: ");
        UECSclient.println(server);
        UECSclient.print("User-Agent: Arduino-agrieye\n");
        UECSclient.println();
        requested = true;
    } else {
        // failed to connect to server
        UECSclient.stop();
        Serial.print("x\n");
        return false;
    }

    // receive result from http sent request
    if(requested) {
        Serial.print(" q\n");
        unsigned long timeoutRead = millis();
        
        while((UECSclient.connected() || UECSclient.available()) && ((millis() - timeoutRead) < 10000)) {
            if(UECSclient.available() > 0) {
                char inChar = UECSclient.read();
                Serial.print(inChar);
                
                // find the # two times
                if(inChar == '#') {
                   ci++;
                } else {
                   ci = 0 ;
                }
                
                if(ci == 2) {
                    //authReply = UECSclient.parseInt();
                    savedReply = UECSclient.parseInt();
                    statusReply = true;
                }
                               
              } else {
                  delay(1000);
              } 
              
            }
            // close UECSclient
            UECSclient.stop();
    }
    // debug receive message
    if(statusReply) {
        /*
        Serial.print("Auth:");
        Serial.println(authReply);
        Serial.print("Saved:");
        Serial.println(savedReply);
        */
        if(savedReply == 1) {
            return true;
        } else {
            return false;
        }
    } else {
        //Serial.print(" x2\n");
        return false;
    }
}
