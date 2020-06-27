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
  
}
struct writeInfluxDataPoints {
  template<typename Item>
  void apply(Item &i) {
    Point pointItem(settingHostname);
    pointItem.clearTags();
    pointItem.clearFields();
    if (i.present()) 
    {
      if (strlen(Item::unit()) != 0) 
      {
        pointItem.addTag(Item::unit(),Item::name);
      } 
      else 
      {        
        pointItem.addTag("#",Item::name);
      }
      
      pointItem.addField("value", i.val());
      DebugT("Writing to influxdb:");
      Debugln(pointItem.toLineProtocol());
    }
    if (!client.writePoint(pointItem)) {
      DebugT("InfluxDB write failed: ");
      Debugln(client.getLastErrorMessage());
    }
//    pointItem.empty();
  }
};

void handleInfluxDB()
{
  // Enable messages batching
  //client.setWriteOptions(WritePrecision::MS, 10, 60);
  DSMRdata.applyEach(writeInfluxDataPoints());
  // Check whether buffer in not empty
  //if (!client.isBufferEmpty()) {
     // Write all remaining points to db
  //client.flushBuffer();
 }
  
}
