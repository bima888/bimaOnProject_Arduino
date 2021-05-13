#include <WiFi.h>  //header  komunikasi Wi-Fi ESP 32
#include <ESP32Servo.h> //header handling Servo
#include <DHT.h>
#define DHTTYPE DHT11


/*____________________KOMUNIKASI INTERNET_____________________*/
const char* ssid     = "Pixel";
const char* password = "bima8888";
WiFiServer server(80); //Server Address
/*____________________________________________________________*/

/*_________________INISIALISASI GPIO______________*/
Servo servo;
uint8_t DHTPin = 23;
DHT dht(DHTPin, DHTTYPE);
float Temperature;
//int maximal = 70; //suhu maksimal
//int minimal = 28; //suhu minimal
/*________________________________________________*/

/*_________________INISIALISASI SENSOR WARNA______________*/
// Frekuensi Photo dioda
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;
// Tipe data Warna RGB
int redColor1 = 263;int redcon1;float redstat1;float resultred1;
//tipe data (int) namaoffset;, tipe data (int) nama variable1;, tipe data (float) nama variable2;, tipe data (float) nama hasil;
//... dan seterusnya untuk nilai offset yang lain   
int greenColor = 0;
int blueColor = 0;
/*_________________________________________________________*/


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
    pinMode(DHTPin, INPUT);
    
    // Setting frequency scaling 100%
    digitalWrite(12,HIGH);
    digitalWrite(13,HIGH);

    // Sensor Suhu aktif
    dht.begin();    
    delay(10);
  
    // Koneksi Dimulai 
    Serial.println();
    Serial.println();
    Serial.print("Menyambungkan ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    //Event sukses
    Serial.println("");
    Serial.println("WiFi Tersambung.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.begin();

}


void loop(){
 WiFiClient client = server.available();   // Komunikasi clients

  if (client) {                             
    Serial.println("New Client.");           // Display saat ada event
    String currentLine = "";                
    while (client.connected()) {            
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                   
        if (c == '\n') {                    

          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            Temperature = dht.readTemperature(); // Gets the values of the temperature
            client.print("Button <a href=\"/H\">ON</a> Deteksi Dimulai.<br>");
            client.print("Button <a href=\"/L\">OFF</a> Deteksi mati.<br>");
            client.print("<p>Status Temperatur : ");
            client.print((int)Temperature);
            client.print("°C\n</p>");
            client.print("<h1>KELOMPOK 6</h1>\n");
            client.print("<h3>Pengeraman pisang Ambon</h3>\n");
            client.println();          
            break;
          } else {  
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;      
        }

        // Validasi "GET /H" atau "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(2, HIGH);               // SISTEM /H untuk state on
          digitalWrite(32, HIGH);
          digitalWrite(33, LOW);
          //baca();
          rotate();
             client.print("<p>Status RED : ");
             client.print((int)redColor1);
             client.print("%\n</p>");
             client.print("<p>Status GREEN : ");
             client.print((int)greenColor);
             client.print("%\n</p>");
             client.print("<p>Status BLUE : ");
             client.print((int)blueColor);
             client.print("%\n</p>");

//            if ((int)Temperature == maksimal)
//            {
//              digitalWrite(32, LOW);
//              digitalWrite(33, HIGH);
//              } else if ((int)Temperature  == minimal) 
//            {
//               digitalWrite(32, HIGH);
//               digitalWrite(33, LOW); 
//               }
          
            if(resultred1>redColor1/*data offset tangan bawah*/ && resultred1>redColor1/*data offset tangan atas*/) //Offset warna pisang
            {
              client.print("<p>Pisang sudah matang</p>");
              digitalWrite(2, LOW);                // SISTEM /L untuk state off
              digitalWrite(32, LOW);
              digitalWrite(33, HIGH);
              delay(500);
              digitalWrite(33, HIGH);
              delay(500);
              digitalWrite(33, LOW);    
              }
              else
              {
                rotate();
                client.print("<p>Pisang dalam proses pengeraman</p>");
                }
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(2, LOW);                // SISTEM /L untuk state off
          digitalWrite(32, LOW);
          digitalWrite(33, HIGH);
          delay(500);
          digitalWrite(33, HIGH);
          delay(500);
          digitalWrite(33, LOW);          
        }
      }
    }
    // close connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}




/*_________________FUNGSI SENSOR WARNA______________*/
void baca()
{
  // Setting RED (R) 
  digitalWrite(27,LOW);
  digitalWrite(26,LOW);
  
  // pembacaan Output frekuensi
  redFrequency = pulseIn(25, LOW);
  redColor1 = map(redFrequency, 70, 120, 255,0);
  /*___RUMUS___*/
  //DATA 1
  redstat1 = 1.14/100*redColor1;
  resultred1 = redColor1-redstat1;
  /*___RUMUS___*/

  // Print RED (R)
  Serial.print("R = ");
  Serial.print(redColor1);
  delay(100);
  
  // Setting GREEN (G)
  digitalWrite(27,HIGH);
  digitalWrite(26,HIGH);
  
  // pembacaan Output frekuensi
  greenFrequency = pulseIn(25, LOW);
  greenColor = map(greenFrequency, 100, 199, 255, 0);

  // Print GREEN (G)  
  Serial.print(" G = ");
  Serial.print(greenColor);
  delay(100);
 
  // Setting BLUE (B) 
  digitalWrite(27,LOW);
  digitalWrite(26,HIGH);

   // pembacaan Output frekuensi
  blueFrequency = pulseIn(25, LOW);
  blueColor = map(blueFrequency, 38, 84, 255, 0);
  
  // Print BLUE (B) 
  Serial.print(" B = ");
  Serial.print(blueColor);
  delay(100);
   
  // Tampilan di serial monitor
  if(redColor1 > greenColor && redColor1 > blueColor){
      Serial.println("STATUS RGB - MERAH");
  }
  if(greenColor > redColor1 && greenColor > blueColor){
    Serial.println("STATUS RGB - HIJAU");
  }
  if(blueColor > redColor1 && blueColor > greenColor){
    Serial.println("STATUS RGB - BIRU");
  }
}
/*____________________________________________________________*/



/*_________________FUNGSI SENSOR WARNA 2______________*/
void rotate()
{
  
  for(int i = 0; i < 5; i++)
  {
  baca();//READ
  servo.write(10);
  delay(500);
  servo.write(15);
  delay(500);
  servo.write(20);
  delay(500);
  servo.write(25);
  delay(500);
  servo.write(30);
  delay(500);
  servo.write(35);
  delay(500);
  servo.write(40);
  delay(500);
  servo.write(45);
  delay(500);
  servo.write(50);
  delay(500);
  baca();//READ
  servo.write(55);
  delay(500);
  servo.write(60);
  delay(500);
  servo.write(65);
  delay(500);
  servo.write(70);
  delay(500);
  servo.write(75);
  delay(500);
  servo.write(80);
  delay(500);
  servo.write(85);
  delay(500);
  servo.write(90);
  delay(500);
  servo.write(95);
  delay(500);
  servo.write(100);
  delay(500);
  servo.write(105);
  delay(500);
  servo.write(110);
  delay(500);
  servo.write(115);
  delay(500);
  servo.write(120);
  delay(500);
  servo.write(125);
  delay(500);
  servo.write(130);
  delay(500);
  servo.write(135);
  delay(500);
  servo.write(140);
  delay(500);
  servo.write(145);
  delay(500);
  servo.write(150);
  delay(500);
  servo.write(155);
  delay(500);
  servo.write(160);
  delay(500);
  servo.write(165);
  delay(500);
  servo.write(170);
  delay(500);
  servo.write(175);
  delay(500);
  servo.write(180);
  delay(500);
  baca();//READ
  servo.write(0);
  }
delay(1000);   
}
/*____________________________________________________________*/
