char server[] = {"192.168.11.100"} ; //DB Server Address
// FUNCTION FOR SENDING DATA TO SERVER


boolean datasend() {
    EthernetClient UECSclient;
    boolean statusReply = false;
    boolean requested = false;
    byte savedReply = 0;
    byte ci = 0;
    // connect to server
    if(UECSclient.connect(server, 80)) {
        Serial.print(" ~ ");
        //request http in line
        UECSclient.print("GET /api/data.php?api=uecs_data");
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
