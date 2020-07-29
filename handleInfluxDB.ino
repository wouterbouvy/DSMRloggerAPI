/*
***************************************************************************  
**  Program  : handleInfluxDB - part of DSMRlogger-Next
**  Version  : v2.1.0-rc0
**
**  Copyright (c) 2020 Robert van den Breemen
**
**  Description: Handles sending all data directly to an influxdb instance
**
**  TERMS OF USE: MIT License. See bottom of file.                                                            
***************************************************************************      
*      Created by Robert van den Breemen (26 june 2020)
*                 my influxdb implementation (30 juli 2020)
*/  

#define LINE_BUFFER 250
char buffer[LINE_BUFFER] {0};
char sAPIwrite[100];

time_t thisEpoch;
WiFiClient  influxdbClient;   

void initInfluxDB()
{
  if ( sizeof(settingInfluxDBhostname) < 8 )  return; 

  // Set InfluxDB 1 authentication params
  // Only needed when query's are dMsg), "http://%s:%d", settingInfluxDBhostname , settingInfluxDBport);
  DebugTf("InfluxDB Connection Setup: [%s] - database: [%s]\r\n", cMsg , settingInfluxDBdatabasename);
  
  snprint(sAPIwrite, sizeof(sAPIwrite), "/write?db=%s&precision=s", settingInfluxDBdatabasename);
  DebugTf("API influxdb: %s",sAPIwrite);

  if (influxdbClient.connect(settingInfluxDBhostname,settingInfluxDBport)) {
     DebugT("Connected to InfluxDB: ");
     Debugln(cMsg);
  }  
}

//=========================================================================
// function to add a value to the lineprotocol buffer for influxdb
//=========================================================================
void addvalueInfluxdb(char *buff, const char *cValue)
{
  strlcat(buff, cValue, LINE_BUFFER);
} // addvalueInfluxdb(*char, *char)
//---------------------------------------------------------------
void addvalueInfluxdb(char *buff, String sValue)
{
  strlcat(buff, sValue.c_str(), LINE_BUFFER);
} // addvalueInfluxdb(*char, String)
//---------------------------------------------------------------
void addvalueInfluxdb(char *buff, int32_t iValue)
{
  char sTmp[20];
  snprintf(sTmp, 10, "%d", iValue);
  strlcat(buff, sTmp, LINE_BUFFER);
  strlcat(buff, "i", LINE_BUFFER);  
} // addvalueInfluxdb(*char,  int)
//---------------------------------------------------------------
void addvalueInfluxdb(char *buff, uint32_t uValue)
{
  char sTmp[20];
  snprintf(sTmp, 10, "%u", uValue);
  strlcat(buff, sTmp, LINE_BUFFER);
  strlcat(buff, "i", LINE_BUFFER);  
} // addvalueInfluxdb(char *buff, *char, uint)
//---------------------------------------------------------------
void addvalueInfluxdb(char *buff, float fValue)
{
  char sTmp[20];
  snprintf(sTmp, 10, "%.3f", fValue);
  strlcat(buff, sTmp, LINE_BUFFER);
} // addvalueInfluxdb(*char, float)

//=========[ writeInfluxDataPoints ]==========================================================

struct writeInfluxDataPoints {

    template<typename Item>
    void apply(Item &i) {
      if (i.present()) 
      {
        if (strlen(Item::unit()) != 0) 
        {
          char sTmp[10];
          memset(buffer, 0, sizeof(buffer));                                //clear lineprotovol
          strlcpy(buffer, Item::unit(), sizeof(buffer));                    //measurement name
          strlcat(buffer, ",instance=", sizeof(buffer));                    //addtag "instance"
          strncat_P(buffer, (PGM_P)Item::name, sizeof(buffer));             //DSMR items name = tag value
          strlcat(buffer, " value=", sizeof(buffer));                       //adddatapoint use "value" as name
          addvalueInfluxdb(buffer, i.val());                                //add actual value (polymorphism addvalue)
          strlcat(buffer, " ", sizeof(buffer));                             //whitespace delimiter for epoch timestamp
          strlcat(buffer, itoa(thisEpoch, sTmp, 10), sizeof(buffer));       //add epoch timestamp
          DebugT("Influxdb lineprotocol [");Debug(buffer);Debugln("]");
        } // if data has a unit
      } // if data is present
  } //apply
};  // struct writeInfluxDataPoints

//=========[ handleInfluxDB ]=================================================================

void handleInfluxDB()
{
  static uint32_t lastTelegram = 0;
  // if (!client.validateConnection()) return; // only write if there is a valid connection to InfluxDB
  if ((telegramCount - lastTelegram)> 0)
  {
    //New telegram received, let's forward that to influxDB
    lastTelegram = telegramCount;
    //Setup the timestamp for this telegram, so all points for this batch are the same.
    thisEpoch = UTC.now();  
    DebugTf("Writing telegram to influxdb - Epoc = %d (this) %d (NL) %d (UTC) \r\n", (int)thisEpoch, (int)localTZ.now(), (int)UTC.now());
    uint32_t timeThis = millis();
    DSMRdata.applyEach(writeInfluxDataPoints());
  
    DebugTf("Influxdb write took [%d] ms\r\n", (int)(millis()-timeThis));
  }
  
}