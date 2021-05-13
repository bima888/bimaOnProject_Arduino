#include <WiFi.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>

//Defining Sensor
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define setupGPIO 8
 
// Constants
const char *ssid = "ESP32-AP";
const char *password =  "bisabisabisa";
const char *msg_tangan_A = "tanganATAS";
const char *msg_tangan_B = "tanganBAWAH";
const char *msg_get_senA = "getSENStateA";
const char *msg_get_senB = "getSENStateB";
const int dns_port = 53;
const int http_port = 80;
const int ws_port = 1337;
const int led_pin = 15;
 
// Globals
AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(1337);
char msg_buf[10];
int SEN_stateA = 0;
int SEN_stateB = 0;
 
/***********************************************************
 * Functions
 */
 
// Callback: receiving any WebSocket message
void onWebSocketEvent(uint8_t client_num,
                      WStype_t type,
                      uint8_t * payload,
                      size_t length) {
 
  // Figure out the type of WebSocket event
  switch(type) {
 
    // Client has disconnected
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", client_num);
      break;
 
    // New client has connected
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(client_num);
        Serial.printf("[%u] Connection from ", client_num);
        Serial.println(ip.toString());
      }
      break;
 
    // Handle text messages from client
    case WStype_TEXT:
 
      // Print out raw message
      Serial.printf("[%u] Received text: %s\n", client_num, payload);
 
      // Toggle LED
      if ( strcmp((char *)payload, "tanganATAS") == 0 ) {
        SEN_stateA = SEN_stateA ? 0 : 1;
        Serial.printf("Toggling SEN_A to %u\n", SEN_stateA);
        scanning;
 
      // Report the state of the LED
      } else if ( strcmp((char *)payload, "getSENStateA") == 0 ) {
        sprintf(msg_buf, "%d", SEN_stateA);
        Serial.printf("Sending to [%u]: %s\n", client_num, msg_buf);
        webSocket.sendTXT(client_num, msg_buf);
 
      // Message not recognized
      } else {
        Serial.println("[%u] Message not recognized");
      }

      if ( strcmp((char *)payload, "tanganBAWAH") == 0 ) {
        SEN_stateB = SEN_stateB ? 0 : 1;
        Serial.printf("Toggling SEN_B to %u\n", SEN_stateB);
        scanning;
 
      // Report the state of the LED
      } else if ( strcmp((char *)payload, "getSENStateB") == 0 ) {
        sprintf(msg_buf, "%d", SEN_stateB);
        Serial.printf("Sending to [%u]: %s\n", client_num, msg_buf);
        webSocket.sendTXT(client_num, msg_buf);

      else {
        Serial.println("[%u] Message not recognized");
      }
      break;
 
    // For everything else: do nothing
    case WStype_BIN:
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
    default:
      break;
  }
}


//callback scanning sensor
void scanning ()
{
  int redfrek = 0;
  int greenfrek = 0;
  int bluefrek = 0;

  int red = 0;
  int green = 0;
  int blue = 0;
  
  digitalWrite (S2,LOW);
  digitalWrite (S3,LOW);
  redfrek = pulseIn (setupGPIO, LOW);
  red = map(redfrek, 00,00,255,0);
  
  digitalWrite (S2,HIGH);
  digitalWrite (S3,HIGH);
  greenfrek = pulseIn (setupGPIO, LOW);
  green = map(greenfrek, 00,00,255,0);

  digitalWrite (S2,LOW);
  digitalWrite (S3,HIGH);
  bluefrek = pulseIn (setupGPIO, LOW);
  blue = map(bluefrek, 00,00,255,0);

  if (red > green && red > blue){Serial.println("Warna Merah");}
  if (green > red && green > blue){Serial.println("Warna Hijau");}
  if (blue > red && blue > green){Serial.println("Warna Biru");}
  }

// Callback: send homepage
void onIndexRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/index.html", "text/html");
}
 
// Callback: send style sheet
void onCSSRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/style.css", "text/css");
}
 
// Callback: send 404 if requested file does not exist
void onPageNotFound(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(404, "text/plain", "Not found");
}                     
                      }
 
/***********************************************************
 * Main
 */
 
void setup() {
 //GPIO SETTING INPUT
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  //SETTING sensor sebagai Input
  pinMode (setupGPIO, INPUT);
  //frekuensi scalling ke 20 %
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  
  // Start Serial port
  Serial.begin(115200);
 
  // Make sure we can read the file system
  if( !SPIFFS.begin()){
    Serial.println("Error mounting SPIFFS");
    while(1);
  }
 
  // Start access point
  WiFi.softAP(ssid, password);
 
  // Print our IP address
  Serial.println();
  Serial.println("AP running");
  Serial.print("My IP address: ");
  Serial.println(WiFi.softAPIP());
 
  // On HTTP request for root, provide index.html file
  server.on("/", HTTP_GET, onIndexRequest);
 
  // On HTTP request for style sheet, provide style.css
  server.on("/style.css", HTTP_GET, onCSSRequest);
 
  // Handle requests for pages that do not exist
  server.onNotFound(onPageNotFound);
 
  // Start web server
  server.begin();
 
  // Start WebSocket server and assign callback
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
  
}
 
void loop() {
  
  // Look for and handle WebSocket data
  webSocket.loop();
}
