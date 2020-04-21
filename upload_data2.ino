//char server[] = {"192.168.1.129"} ; //DB Server Address
// FUNCTION FOR SENDING DATA TO SERVER


boolean datasend() {
  char server[] = {"192.168.1.129"} ; //DB Server Address
  EthernetClient UECSclient;
  boolean statusReply = false;
  boolean requested = false;
  byte savedReply = 0;
  byte ci = 0;

  
  // connect to server
  if (UECSclient.connect(server, 80)) {
    Serial.print(" ~ ");
    Serial.print("Connection Established");
    
    Serial.print("GET /api/data.php?temperature=");
    UECSclient.print("GET /api/data.php?temperature=");     //YOUR URL
    Serial.println(U_ccmList[CCMID_InAirTemp].value);
    UECSclient.print((double)U_ccmList[CCMID_InAirTemp].value/10, 1);

    
    UECSclient.print("&humidity=");
    UECSclient.print((double)U_ccmList[CCMID_InAirHumid].value/10, 1);
    Serial.print("&humidity=");
    Serial.println((double)U_ccmList[CCMID_InAirHumid].value/10, 1);

    UECSclient.print(" ");      //SPACE BEFORE HTTP/1.1
    UECSclient.print("HTTP/1.1");
    UECSclient.println();
    // UECSclient.println("Host: 192.168.100.105");
    UECSclient.println("Connection: close");

    UECSclient.println();
    Serial.println("Connection: close");

  } else {
    // failed to connect to server
    //UECSUECSclient.stop();
    Serial.println("CONNECTION FAILED");
    Serial.print("x\n");
    return false;
  }
}
// receive result from http sent request
//  if (requested) {
//    Serial.print(" q\n");
//    unsigned long timeoutRead = millis();
//
//    while ((UECSUECSclient.connected() || UECSclient.available()) && ((millis() - timeoutRead) < 10000)) {
//      if (UECSclient.available() > 0) {
//        char inChar = UECSclient.read();
//        Serial.print(inChar);
//
//        // find the # two times
//        if (inChar == '#') {
//          ci++;
//        } else {
//          ci = 0 ;
//        }
//
//        if (ci == 2) {
//          //authReply = UECSclient.parseInt();
//          savedReply = UECSclient.parseInt();
//          statusReply = true;
//        }
//
//      } else {
//        delay(1000);
//      }
//
//    }
//    // close UECSclient
//    UECSclient.stop();
//  }
//  // debug receive message
//  if (statusReply) {
//    /*
//      Serial.print("Auth:");
//      Serial.println(authReply);
//      Serial.print("Saved:");
//      Serial.println(savedReply);
//    */
//    if (savedReply == 1) {
//      return true;
//    } else {
//      return false;
//    }
//  } else {
//    //Serial.print(" x2\n");
//    return false;
//  }
//}
