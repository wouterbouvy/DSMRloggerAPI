/*
***************************************************************************  
**  Program  : handleInfluxDB - part of DSMRloggerAPI
**  Version  : v2.0.2
**
**  Copyright (c) 2020 Robert van den Breemen
**
**  Description: Handles sending all data directly to an influxdb instance
**
**  TERMS OF USE: MIT License. See bottom of file.                                                            
***************************************************************************      
*      Created by Robert van den Breemen (26 june 2020)
*/  

// InfluxDB  server url. Don't use localhost, always server name or ip address.
// For InfluxDB 2 e.g. http://192.168.1.48:9999 (Use: InfluxDB UI -> Load Data -> Client Libraries), 
// For InfluxDB 1 e.g. http://192.168.1.48:8086
#define INFLUXDB_URL "http://192.168.88.254:8086"
//#define INFLUXDB_URL "http://127.0.0.1:8086"
// InfluxDB 2 server or cloud API authentication token (Use: InfluxDB UI -> Load Data -> Tokens -> <select token>)
//#define INFLUXDB_TOKEN "toked-id"
// InfluxDB 2 organization id (Use: InfluxDB UI -> Settings -> Profile -> <name under tile> )
//#define INFLUXDB_ORG "org"
// InfluxDB 2 bucket name (Use: InfluxDB UI -> Load Data -> Buckets)
//#define INFLUXDB_BUCKET "bucket"
// InfluxDB v1 database name 
#define INFLUXDB_DB_NAME "dsmr-api"
#include <InfluxDbClient.h>

// InfluxDB client instance
//InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);
// InfluxDB client instance for InfluxDB 1
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_DB_NAME);

// Set timezone string according to https://www.gnu.org/software/libc/manual/html_node/TZ-Variable.html
// Examples:
//  Pacific Time:   "PST8PDT"
//  Eastern:        "EST5EDT"
//  Japanesse:      "JST-9"
//  Central Europe: "CET-1CEST,M3.5.0,M10.5.0/3"
#define TZ_INFO "CET-1CEST,M3.5.0,M10.5.0/3"
#define WRITE_PRECISION WritePrecision::S
#define MAX_BATCH_SIZE 16
#define WRITE_BUFFER_SIZE 32

time_t thisEpoch;

void initInfluxDB()
{
  // Set InfluxDB 1 authentication params
  // Only needed when query's are done?
  //client.setConnectionParamsV1(INFLUXDB_URL, INFLUXDB_DB_NAME, INFLUXDB_USER, INFLUXDB_PASSWORD);

  // Connect to influxdb server connection
  if (client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }
    
  //Enable messages batching and retry buffer
//  client.setWriteOptions(WRITE_PRECISION, MAX_BATCH_SIZE, WRITE_BUFFER_SIZE);
}
struct writeInfluxDataPoints {
  template<typename Item>
  void apply(Item &i) {
    
    if (i.present()) 
    {
      if (strlen(Item::unit()) != 0) 
      {
        //when there is a unit, then it is a measurement
        Point pointItem(Item::unit());
//        pointItem.setTime(thisEpoch);
        pointItem.addTag("instance",Item::name);     
        pointItem.addField("value", i.val());
//        pointItem.addField((String)(Item::name), i.val());
        DebugT("Writing to influxdb:");
        Debugln(pointItem.toLineProtocol());
        if (!client.writePoint(pointItem)) {
          DebugT("InfluxDB write failed: ");
          Debugln(client.getLastErrorMessage());
        }
      }//writing to influxdb      
    }
  }
};

void handleInfluxDB()
{
  static uint32_t lastTelegram = 0;
//   DebugTf("telegramCount=[%d] telegramErrors=[%d] lastTelegram=[%d]\r\n", telegramCount, telegramErrors, lastTelegram);
  
//  if ((telegramCount - telegramErrors) > 1) 
//  {
//    Debugln("InfluxDB: Waiting for first telegram...");  
//    return;
//  }
  if ((telegramCount - lastTelegram)> 0)
  {
    //New telegram received, let's forward that to influxDB
    lastTelegram = telegramCount;
    //Setup the timestamp for this telegram, so all points for this batch are the same.
    thisEpoch = now();  
    DebugTf("Epoc = %d\r\n", (int)now());
    DSMRdata.applyEach(writeInfluxDataPoints());
    // Check whether buffer in not empty
    if (!client.isBufferEmpty()) {
      // Write all remaining points to db
      client.flushBuffer();
    }
  }
  
}
