/*******************************************************************
    A sketch for controlling a robot with using a web page 
    hosted on a ESP32
    based on: https://github.com/witnessmenow/simple-wifi-controlled-rc-car
********************************************************************/

// Load Wi-Fi library
#include <WiFi.h>
#include <WebServer.h>

// Replace with your network credentials
const char* ssid = "xxxxxxxx";
const char* password = "yyyyyyyy";

// Motor A
int motor1Pin1 = 15; 
int motor1Pin2 = 13; 

// Motor B
int motor2Pin1 = 12; 
int motor2Pin2 = 14; 

// Setting PWM properties
const int freq = 30000;
const int pwmChannel1 = 0;
const int pwmChannel2 = 1;
const int resolution = 8;
int dutyCycle = 200;

// Set web server port number to 80
WebServer server(80);

const char *webpage = 
#include "motorPage.h"
;

void handleRoot() {

  server.send(200, "text/html", webpage);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void){

  Serial.begin(115200);

  // sets the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);

  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  
  // configure LED PWM functionalitites
  ledcSetup(pwmChannel1, freq, resolution);

  ledcSetup(pwmChannel2, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  //ledcAttachPin(motor1Pin1, pwmChannel1);

  //ledcAttachPin(motor2Pin1, pwmChannel2);

 // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  server.on("/", handleRoot);

  server.on("/forward", [](){
    Serial.println("forward");

    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW); 

    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW); 

    server.send(200, "text/plain", "forward");
  });

  server.on("/driveStop", [](){
    Serial.println("driveStop");

    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);

    server.send(200, "text/plain", "driveStop");
  });

  server.on("/back", [](){
    Serial.println("back");

    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH); 

    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH); 

    server.send(200, "text/plain", "back");
  });

  server.on("/right", [](){
    Serial.println("right");

    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH); 

    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW); 

    server.send(200, "text/plain", "right");
  });

  server.on("/left", [](){
    Serial.println("left");

    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW); 

    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH); 

    server.send(200, "text/plain", "left");
  });

  server.on("/steerStop", [](){
    Serial.println("steerStop");

    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);

    server.send(200, "text/plain", "steerStop");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  
  Serial.println("HTTP Server Started");
  
}

void loop(void){
  server.handleClient();
}
