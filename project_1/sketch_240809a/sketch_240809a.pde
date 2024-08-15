import processing.net.*;

Client myClient;

void setup() {
  size(400, 400);
  String serverIp = "192.168.1.6";
  int port = 80;

  // Setup client connection
  setupClientConnection(serverIp, port);

  textSize(22);  // Set text size for displaying received data
}

// Function to set up the client connection
void setupClientConnection(String serverIp, int port) {
  myClient = new Client(this, serverIp, port);
  delay(3000);  // Allow time for the connection to establish
  myClient.write("Hello");  // Send initial greeting message
}

void draw() {
  if (myClient.available() > 0) {
    String receivedData = myClient.readString();
    displayReceivedData(receivedData);
  }
}

// Function to display the received data
void displayReceivedData(String data) {
  println(data);
  background(0);  // Clear the background
  text(data, 50, 200);  // Display the data on the screen
}
