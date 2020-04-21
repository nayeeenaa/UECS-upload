// FUNCTION FOR SENDING DATA TO SERVER


boolean datasend() {
  char server[] = {"192.168.1.129"} ; //DB Server Address
  EthernetClient UECSclient;
 
  // connect to server
  if (UECSclient.connect(server, 80)) {
    Serial.print(" ~ ");
    Serial.print("Connection Established");
    
    Serial.print("GET /api/data.php?temperature=");
    UECSclient.print("GET /api/data.php?temperature=");     //YOUR URL
    Serial.println(U_ccmList[CCMID_InAirTemp].value);
    UECSclient.println((double)U_ccmList[CCMID_InAirTemp].value/10, 1);
    UECSclient.print(" ");      //SPACE BEFORE HTTP/1.1
    UECSclient.print("HTTP/1.1");
    UECSclient.println();
    Serial.println("Connection: close");

  } else {
    // failed to connect to server
    //UECSUECSclient.stop();
    Serial.println("CONNECTION FAILED");
    Serial.print("x\n");
  }
}
