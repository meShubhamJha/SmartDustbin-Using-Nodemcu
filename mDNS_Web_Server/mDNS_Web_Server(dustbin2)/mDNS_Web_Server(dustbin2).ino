/*
  ESP8266 mDNS responder sample

  This is an example of an HTTP server that is accessible
  via http://esp8266.local URL thanks to mDNS responder.

  Instructions:
  - Update WiFi SSID and password as necessary.
  - Flash the sketch to the ESP8266 board
  - Install host software:
    - For Linux, install Avahi (http://avahi.org/).
    - For Windows, install Bonjour (http://www.apple.com/support/bonjour/).
    - For Mac OSX and iOS support is built in through Bonjour already.
  - Point your browser to http://esp8266.local, you should see a response.

*/


#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <Servo.h>

#ifndef STASSID
#define STASSID "Students"
#define STAPSK  "0123456789"
#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
const int trigPin = 12;
const int echoPin = 14;

long duration;
float distanceCm;

// TCP server at port 80 will respond to HTTP requests
WiFiServer server(80);
Servo servo;

float getDistance(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * SOUND_VELOCITY/2;
  return 70;
}

void compress(){
  servo.write(180);
  delay(1000);
  servo.write(0);
  delay(1000);
}



void setup(void) {
  Serial.begin(115200);
  servo.attach(2); //D4
  servo.write(0);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Connect to WiFi network
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Set up mDNS responder:
  // - first argument is the domain name, in this example
  //   the fully-qualified domain name is "esp8266.local"
  // - second argument is the IP address to advertise
  //   we send our IP address on the WiFi network
  if (!MDNS.begin("dustbin2")) {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");

  // Start TCP (HTTP) server
  server.begin();
  Serial.println("TCP server started");

  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);
}

void loop(void) {

  MDNS.update();

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  Serial.println("");
  Serial.println("New client");

  // Wait for data from client to become available
  while (client.connected() && !client.available()) {
    //delay(1);
    if (getDistance()<=50){
    Serial.println("Compressing...");
    compress();
    Serial.println("Compressed successfully...");
  }
  }

  // Read the first line of HTTP request
  String req = client.readStringUntil('\r');

  // First line of HTTP request looks like "GET /path HTTP/1.1"
  // Retrieve the "/path" part by finding the spaces
  int addr_start = req.indexOf(' ');
  int addr_end = req.indexOf(' ', addr_start + 1);
  if (addr_start == -1 || addr_end == -1) {
    Serial.print("Invalid request: ");
    Serial.println(req);
    return;
  }
  req = req.substring(addr_start + 1, addr_end);
  Serial.print("Request: ");
  Serial.println(req);
  client.flush();

  String s;
  if (req == "/") {
    IPAddress ip = WiFi.localIP();
    String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>Hello from ESP8266 at ";
    s += ipStr;
    s += "</html>\r\n\r\n";
    Serial.println("Sending 200");
  }
  else if (req == "/getdistance") {
    s = "{\"response\":\"ok\",\"message\":\"70\"}";
    Serial.println("Sending 200");
  }
  else if (req == "/compress") {
    s = "{\"response\":\"ok\",\"message\":\"compressing\"}";
    compress();
    Serial.println("Sending 200");
  }
   else {
    s = "HTTP/1.1 404 Not Found\r\n\r\n";
    Serial.println("Sending 404");
  }
  client.print(s);


  Serial.println("Done with client");
}

