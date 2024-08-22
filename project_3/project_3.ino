#include <ESP8266WiFi.h>
#include "DHT.h"
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

const char* ssid = "SSID";
const char* password = "PASS";
int keyIndex = 0;            // your network key Index number (needed only for WEP)

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

#define DHTPIN D4
#define DHTTYPE DHT11

// Initialize DHT sensor and WiFi client
DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

void connectToWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void setup() {
  delay(5000); // Initial delay to ensure everything is set up
  Serial.begin(9600);
  dht.begin();
  connectToWiFi();
  Serial.println("Client started");
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}


void loop() {

  float t = dht.readTemperature();
  float h = dht.readHumidity();

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.println();
  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, h);
 int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  delay(20000); // Wait 10 seconds to update the channel again
}