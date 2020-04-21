/////////////////////////////////////////
//UARDECS Sample Program "TempHumidSensor_SHT2x"
//I2C connection test with SHT2x Ver1.1
//By H.kurosaki 2017/03/01
//////////////////////////////////////////

//Sensirion SHT2xシリーズから温度と湿度を読み出します。I2C接続でドライバ不要です。
//SHT2xは3.3V用なので、バス電圧5VのUNOやMEGAに直結できません。
//http://akizukidenshi.com/catalog/g/gM-05452/
//などを使ってセンサ側のバス電圧を3.3Vにする必要があります。


//秋月電子のＩ２Ｃバス用双方向電圧レベル変換モジュールPCA9306を使う場合
//結線方法
//[Arduino]-[PCA9306]
//[SCL]-[SCL1]
//[SDA]-[SDA1]
//[5V]-[VREF1]
//[GND]-[GND]
//[3.3V]-[VREF2]

//[PCA9306]-[SHT2x]
//[SCL2]-[SCL]
//[SDA2]-[SDA]
//[GND]-[GND]
//[VREF2]-[+V]

//注意！
//湿度センサは有機溶剤や酸・アルカリで損傷することがあります。
//アルコールなどの溶剤を近くで使わないこと。
//梱包時に使用するビニール袋やケースのプラスチック素材から揮発する有機ガスで
//損傷することがあるので注意すること。


#include <SPI.h>
#include <Ethernet2.h>
#include <EthernetUdp2.h>
#include <avr/pgmspace.h>
#include <EEPROM.h>
#include <Uardecs_mega.h>
#include <Wire.h>

#define OPRMODE_ERR_SHT2xSENSERR       0x20000000 //0 01000 00000 0 0000 0000000000000000 //センサ異常



//char server[] = {"192.168.1.129"} ; //DB Server Address





char SHT2x_sensSts;
unsigned char SHT2x_errcount=0;


/////////////////////////////////////
//IP reset jupmer pin setting
//IPアドレスリセット用ジャンパーピン設定
/////////////////////////////////////
const byte U_InitPin = 3;
const byte U_InitPin_Sense=HIGH;


// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
//char server[] = "www.google.com";    // name address for Google (using DNS)

// Set the static IP address to use if the DHCP fails to assign


// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
//EthernetClient client;


////////////////////////////////////
//Node basic infomation
//ノードの基本情報
///////////////////////////////////
const char U_name[] PROGMEM= "TempHumid Node";//MAX 20 chars
const char U_vender[] PROGMEM= "WARC/NARO";//MAX 20 chars
const char U_uecsid[] PROGMEM= "000000000000";//12 chars fixed
const char U_footnote[] PROGMEM= "SHT2x Sensor Node";
char U_nodename[20] = "node";   //MAX 19chars (This value enabled in safemode)
UECSOriginalAttribute U_orgAttribute;//この定義は弄らないで下さい
//////////////////////////////////
// html page1 setting
//Web上の設定画面に関する宣言
//////////////////////////////////

//Total number of HTML table rows.
//web設定画面で表示すべき項目の総数
const int U_HtmlLine = 0;

//表示素材の登録
struct UECSUserHtml U_html[U_HtmlLine]={
};

//////////////////////////////////
// UserCCM setting
// CCM用の素材
//////////////////////////////////

enum {
CCMID_InAirTemp,
CCMID_InAirTemp2,
CCMID_InAirQuality,
CCMID_InAirHumid,
CCMID_InAirHD,
CCMID_cnd,
CCMID_dummy, //CCMID_dummyは必ず最後に置くこと
};


//CCM格納変数の宣言
//ここはこのままにして下さい
const int U_MAX_CCM = CCMID_dummy;
UECSCCM U_ccmList[U_MAX_CCM];

//CCM定義用の素材
const char ccmNameTemp[] PROGMEM= "Temperature";
const char ccmTypeTemp[] PROGMEM= "InAirTemp";
const char ccmUnitTemp[] PROGMEM= "C";

const char ccmNameTemp2[] PROGMEM= "Temperature";
const char ccmTypeTemp2[] PROGMEM= "InAirTemp2";
const char ccmUnitTemp2[] PROGMEM= "C";

const char ccmNameTemp3[] PROGMEM= "Air Quality";
const char ccmTypeTemp3[] PROGMEM= "InAirQuality";
const char ccmUnitTemp3[] PROGMEM= " ";

const char ccmNameHumid[] PROGMEM= "Humid";
const char ccmTypeHumid[] PROGMEM= "InAirHumid";
const char ccmUnitHumid[] PROGMEM= "%";

const char ccmNameHD[] PROGMEM= "HumidDiff";
const char ccmTypeHD[] PROGMEM= "InAirHD.mIC";
const char ccmUnitHD[] PROGMEM= "g m-3";

const char ccmNameCnd[] PROGMEM= "NodeCondition";
const char ccmTypeCnd[] PROGMEM= "cnd.mIC";
const char ccmUnitCnd[] PROGMEM= "";

//------------------------------------------------------
void UserInit(){
U_orgAttribute.mac[0] = 0x00;
U_orgAttribute.mac[1] = 0x00;
U_orgAttribute.mac[2] = 0x00;
U_orgAttribute.mac[3] = 0x00;
U_orgAttribute.mac[4] = 0x00;
U_orgAttribute.mac[5] = 0x01;
  
UECSsetCCM(true, CCMID_InAirTemp, ccmNameTemp, ccmTypeTemp, ccmUnitTemp, 29, 1, A_10S_0);
UECSsetCCM(true, CCMID_InAirTemp2, ccmNameTemp2, ccmTypeTemp2, ccmUnitTemp2, 29, 1, A_10S_0);
UECSsetCCM(true, CCMID_InAirQuality, ccmNameTemp3, ccmTypeTemp3, ccmUnitTemp3, 29, 1, A_10S_0);
UECSsetCCM(true, CCMID_InAirHumid, ccmNameHumid, ccmTypeHumid, ccmUnitHumid, 29, 1, A_10S_0);
UECSsetCCM(true, CCMID_InAirHD, ccmNameHD, ccmTypeHD, ccmUnitHD, 29, 2, A_10S_0);
UECSsetCCM(true, CCMID_cnd      , ccmNameCnd , ccmTypeCnd , ccmUnitCnd , 29,3, A_10S_0);
}

//---------------------------------------------------------
void OnWebFormRecieved(){
}
//---------------------------------------------------------
void UserEverySecond(){
  U_ccmList[CCMID_cnd].value=0;
  if(SHT2x_errcount>3)
        {
          SHT2x_errcount=4;
        U_ccmList[CCMID_cnd].value|=OPRMODE_ERR_SHT2xSENSERR;
        }

//------------------------------------------------------温度計測(3秒に一度)  
static char count=0;
count++;
if(count %3==1)
  {

  double t=SHT2x_GetTemperature();
//  Serial.print("TEMPERATURE using t=SHT2x_GetTemperature():");
//  Serial.print (t);
  double rh=SHT2x_GetHumidity();
  
  
  if(SHT2x_sensSts)
      {
        SHT2x_errcount++;
      }
  else{
      SHT2x_errcount=0;
      U_ccmList[CCMID_InAirTemp].value=(long)(t*10);
      
//      Serial.print("TEMPERATURE using U_ccmList[CCMID_InAirTemp].value=(long)(t*10):");
//      Serial.print (U_ccmList[CCMID_InAirTemp].value);
      
      U_ccmList[CCMID_InAirTemp2].value=(long)(t*10);
      U_ccmList[CCMID_InAirQuality].value= 0;
      U_ccmList[CCMID_InAirHumid].value=(long)(rh*10);
      }
  
  //飽差計算
  double humidpress=6.1078*pow(10,(7.5*t/(t+237.3)));
  double humidvol=217*humidpress/(t+273.15);
  U_ccmList[CCMID_InAirHD].value=(100-rh)*humidvol;//少数が下位2桁なのでそのまま出力

  
  
  }

//------------------------------------------------エラー時にCCM送信を停止する
if(SHT2x_errcount)
    {
        U_ccmList[CCMID_InAirTemp].flagStimeRfirst=false;
        U_ccmList[CCMID_InAirTemp2].flagStimeRfirst=false;
        U_ccmList[CCMID_InAirQuality].flagStimeRfirst=false;
        U_ccmList[CCMID_InAirHumid].flagStimeRfirst=false;
        U_ccmList[CCMID_InAirHD].flagStimeRfirst=false;
    }
  

/* to get attribute and type data

//  HTTPGetFormDataEDITCCMPage HTTPGetFormDataEDITCCMPageTRIAL;
//  Serial.print("U_ccmList[CCMID_InAirTemp].baseAttribute[AT_ORDE]");
//  Serial.println(U_ccmList[CCMID_InAirTemp].baseAttribute[AT_ORDE]);

//    Serial.print("U_ccmList[CCMID_InAirTemp].typeStr");
//    Serial.println(U_ccmList[CCMID_InAirTemp].typeStr);

*/

    
  
}

//---------------------------------------------------------
void UserEveryMinute(){

    
  
}
//---------------------------------------------------------
void UserEveryLoop(){
    
   datasend();
  
}



//---------------------------------
void loop(){
UECSloop();

 
   // send_to_database_loop();   

    // send_to_database_total();

    

//delay(5000);
}

//---------------------------------
void setup(){

Wire.begin();
Serial.begin(9600);

//send_to_database_setup();


UECSsetup();

// delay(5000);
}

//----------------------------------------SHT2x
#define eSHT2xAddress   0x40
#define eTempHoldCmd      0xE3
#define eRHumidityHoldCmd 0xE5
#define eTempNoHoldCmd      0xF3
#define eRHumidityNoHoldCmd 0xF5

double SHT2x_GetHumidity(void)
{
  return (-6.0 + 125.0 / 65536.0 * (double)(SHT2x_readSensor(eRHumidityHoldCmd)));
}
double SHT2x_GetTemperature(void)
{
  return (-46.85 + 175.72 / 65536.0 * (double)(SHT2x_readSensor(eTempHoldCmd)));
}

uint16_t SHT2x_readSensor(uint8_t command)
{
    uint16_t result;

    Wire.beginTransmission(eSHT2xAddress);
    Wire.write(command);
    delay(5);
    SHT2x_sensSts=Wire.endTransmission();
    if(SHT2x_sensSts){return 0;}

    Wire.requestFrom(eSHT2xAddress, 3);
    if(Wire.available()==3)
    {
    result = ((Wire.read()) << 8);
    result += Wire.read();
    result &= ~0x0003;   // clear two low bits (status bits)
    return result;
    }
    else
    {
      SHT2x_sensSts=4;
      return 0;
    }
    
}
