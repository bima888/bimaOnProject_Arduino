#include <WiFi.h>  //header  komunikasi Wi-Fi ESP 32
#include <Servo.h>
const char* ssid     = "Pixel";
const char* password = "bima8888";
WiFiServer server(80); //Server Address


Servo servo;
// Frekuensi Photo dioda
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

// Tipe data Warna RGB
int redColor = 0;
int greenColor = 0;
int blueColor = 0;


void setup()
{
    Serial.begin(115200);
     // Setting output
    pinMode(12, OUTPUT);
    pinMode(13, OUTPUT);
    pinMode(27, OUTPUT);
    pinMode(26, OUTPUT);
    pinMode(32, OUTPUT);
    pinMode(33, OUTPUT);
    servo.attach(14);
    
  // Setting input
    pinMode(25, INPUT);
  
  // Setting frequency scaling 20%
    digitalWrite(12,HIGH);
    digitalWrite(13,LOW);
  // Setting frequency PWM 50 Hz  
    ledcSetup(1, 50, TIMER_WIDTH);
    ledcAttachPin(2, 1); 
    

    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.begin();

}

int value = 0;

void loop(){
 WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H\">here</a> to turn the LED on pin 5 on.<br>");
            client.print("Click <a href=\"/L\">here</a> to turn the LED on pin 5 off.<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(2, HIGH);               // GET /H turns the state on
          baca();
          rotate();
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(2, LOW);                // GET /L turns the state off
          digitalWrite(33, HIGH);
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}

void baca()
{
    // Setting RED (R) 
  digitalWrite(27,LOW);
  digitalWrite(26,LOW);
  
  // Reading the output frequency
  redFrequency = pulseIn(25, LOW);
  // Remaping the value of the RED (R) frequency from 0 to 255
  redColor = map(redFrequency, 70, 120, 255,0);

  // Printing the RED (R) value
  Serial.print("R = ");
  Serial.print(redColor);
  delay(100);
  
  // Setting GREEN (G)
  digitalWrite(27,HIGH);
  digitalWrite(26,HIGH);
  
  // Reading the output frequency
  greenFrequency = pulseIn(25, LOW);
  // Remaping the value of the GREEN (G) frequency from 0 to 255
  greenColor = map(greenFrequency, 100, 199, 255, 0);

  // Printing the GREEN (G) value  
  Serial.print(" G = ");
  Serial.print(greenColor);
  delay(100);
 
  // Setting BLUE (B) 
  digitalWrite(27,LOW);
  digitalWrite(26,HIGH);

   // Reading the output frequency
  blueFrequency = pulseIn(25, LOW);
  // Remaping the value of the BLUE (B) frequency from 0 to 255
  blueColor = map(blueFrequency, 38, 84, 255, 0);
  
  // Printing the BLUE (B) value 
  Serial.print(" B = ");
  Serial.print(blueColor);
  delay(100);


  // a message in the serial monitor
  if(redColor > greenColor && redColor > blueColor){
      Serial.println(" - RED detected!");
  }
  if(greenColor > redColor && greenColor > blueColor){
    Serial.println(" - GREEN detected!");
  }
  if(blueColor > redColor && blueColor > greenColor){
    Serial.println(" - BLUE detected!");
  }
  }


  void rotate()
 {
  servo.write(60);
  delay(500);
  servo.write(120);
  delay(500);
  servo.write(0);
  }
