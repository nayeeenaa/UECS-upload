/****************************************************************
 * ReadSHT2x
 *  An example sketch that reads the sensor and prints the
 *  relative humidity to the PC's serial port
 *
 *  Tested with:
 *    - SHT21-Breakout Humidity sensor from elechouse.com

 ***************************************************************/

#include <Wire.h>
#include <SHT2x.h>
#include <SPI.h>
#include <Ethernet.h>




float humidityData;
float temperatureData;
char c = 0; //initialization, don't use this code in client server

int delay_time = 10000;
unsigned long initial_time = 0;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //Setting MAC Address


char server[] = "192.168.1.129";
IPAddress ip(192,168,1,7); 
EthernetClient client; 


void setup()
{
  Wire.begin();
  Serial.begin(9600);

  Serial.println ("Serial confirmed");
  
  if (Ethernet.begin(mac) == 0) 
  {
    Serial.println("Failed to configure Ethernet using DHCP");
    Serial.print("mac failed");}
  else 
  {
    Ethernet.begin(mac, ip);
    Serial.println("Connetced");
    Serial.println(Ethernet.localIP());
  }
 // delay(1000);
}

void loop()
{

Serial.println(client);
 
  humidityData=(SHT2x.GetHumidity());
  Serial.print("Humidity(%RH) from sensor: ");
  Serial.println (humidityData);
 
  temperatureData=(SHT2x.GetTemperature());
  Serial.print("Temperature(C) from sensor: ");
  Serial.println (temperatureData);

//while (c!=0)      //until clients' code matches, 1 for match, 0 for unmatch
//{
//   Serial.println("waiting:for client");
//   delay(10000);
// if (client.available()) //need to confirm from user
//
// {  
//    char b = client.available();
//    char c = client.read();
//    Serial.println(c);
//    Serial.println(c);
//    Serial.println(b);
//    Serial.println("waiting:client not matched");
//  }
// 
//}
  
  Sending_To_phpmyadmindatabase(); 
  // delay(1000); // interval

  
}



  void Sending_To_phpmyadmindatabase()   //CONNECTING WITH MYSQL
  {

  
   if (client.connect(server, 80)) 
   {
    
    // Make a HTTP request:
    Serial.print("GET /api/data.php?temperature=");
    client.print("GET /api/data.php?temperature=");     //YOUR URL
    Serial.println(temperatureData);
    client.print(temperatureData);
    client.print("&humidity=");
    
    client.println(humidityData);
    Serial.print("&humidity=");
    Serial.println(humidityData);
    Serial.print("ip=");
    Serial.println(ip);
    client.print("&ipaddress=");
    client.print(ip);
    client.print(" ");      //SPACE BEFORE HTTP/1.1
    client.print("HTTP/1.1");
    client.println();
   // client.println("Host: 192.168.100.105");
    client.println("Connection: close");
    
    client.println();
    Serial.println("Connection: close");


   initial_time = millis();
   Serial.println("delaying");

  while (millis() < initial_time + delay_time)
  {
  }
    
  } 
  else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
 }
