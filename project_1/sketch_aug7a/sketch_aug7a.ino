#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ESP8266WiFi.h>

// Pin and type configuration for DHT sensor
#define DHTPIN D4
#define DHTTYPE DHT11

// WiFi credentials
const char* ssid = "RRastin";
const char* password = "rastin00000";

// Initialize DHT sensor and WiFi server
DHT dht(DHTPIN, DHTTYPE);
WiFiServer server(80);

// Function to connect to WiFi
void connectToWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

// Function to send temperature and humidity data to the client
void sendDataToClient(WiFiClient client) {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  String data = "temperature: " + String(temperature) + " | humidity: " + String(humidity);
  client.print(data);
  Serial.println(data);
}

// Function to handle client connections
void handleClient() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client Connected.");
    while (client.connected()) {
      if (client.available()) {
        sendDataToClient(client);
        delay(1000); // Delay between readings
      }
    }
    client.stop();
    Serial.println("Client disconnected");
  }
}

// Setup function
void setup() {
  delay(5000); // Initial delay to ensure everything is set up
  Serial.begin(9600);
  dht.begin();
  connectToWiFi();
  server.begin();
  Serial.println("Server started");
}

// Main loop
void loop() {
  delay(1000); // Delay between loop iterations
  Serial.print("Checking... IP is ");
  Serial.println(WiFi.localIP());
  handleClient(); // Handle any incoming clients
}
