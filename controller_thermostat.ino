boolean ChangeThermostat(){
  signed long showValueTemp;
  signed long setONOFFAUTO;
  signed long setONTempFromWeb;
  signed long showValueStatus;
  
    showValueTemp = U_ccmList[CCMID_InAirTemp].value;

  setONOFFAUTO=1;

//サーモスタット動作 
if(setONOFFAUTO==0)
    {U_ccmList[CCMID_cnd].value=0;}//Manual OFF
else if(setONOFFAUTO==1)
    {U_ccmList[CCMID_cnd].value=1;}//Manual ON
else if(setONOFFAUTO==2  && U_ccmList[CCMID_InAirTemp].value>32.0)
    {U_ccmList[CCMID_cnd].value=1;}//Auto ON
else
    {U_ccmList[CCMID_cnd].value=0;}//

    showValueStatus = U_ccmList[CCMID_cnd].value;

    Serial.print(showValueStatus);

    return showValueStatus;

    
}
