#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <PubSubClient.h>       // Include the MQTT library
#include <DHT.h>                // Include the DHT sensor library

// Wi-Fi credentials
const char* ssid = "RRastin";
const char* password = "rastin00000";

// MQTT broker details
const char* mqtt_server = "broker.hivemq.com"; 
const int mqtt_port = 1883;

// DHT sensor configuration
#define DHTPIN D4       
#define DHTTYPE DHT11     
DHT dht(DHTPIN, DHTTYPE);

// Wi-Fi and MQTT client objects
WiFiClient espClient;
PubSubClient client(espClient);

// Function to connect to Wi-Fi
void setup_wifi() {
  Serial.println();
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// Function to reconnect to MQTT broker if the connection is lost
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Clientunique1234567")) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

// Function to publish temperature and humidity data to MQTT topics
void publishSensorData() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  client.publish("rastin/temperature", String(temperature).c_str());
  Serial.println("Temperature: " + String(temperature));
  
  client.publish("rastin/humidity", String(humidity).c_str());
  Serial.println("Humidity: " + String(humidity));
}

// Setup function
void setup() {
  delay(5000); // Initial delay to ensure everything is set up
  Serial.begin(9600);
  dht.begin();
  setup_wifi();  // Connect to Wi-Fi
  client.setServer(mqtt_server, mqtt_port);  // Configure MQTT server
}

// Main loop function
void loop() {
  if (!client.connected()) {
    reconnect();  // Reconnect to MQTT broker if needed
  }

  if (!client.loop()) {
    client.connect("ESP8266Clientunique123456");  // Ensure MQTT connection
  }

  publishSensorData();  // Publish sensor data to MQTT topics

  delay(2000);  // Delay between sensor readings
}
