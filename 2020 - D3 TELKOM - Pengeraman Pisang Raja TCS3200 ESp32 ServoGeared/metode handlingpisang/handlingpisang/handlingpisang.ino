#include <WiFi.h>
#include <Servo.h>
//GPIO sensor warna 
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8

//GPIO sensor suhu
#define S4 2

//GPIO servogeared
#define M1 1

//tipe data servo
Servo Scan;
//define protokol komunikasi ESP32
//diisi dengan ID dan PASSWORD WIFI
const char* ssid     = "";
const char* password = "";

//Set web server port ke 80
//untuk HTTP request ke HTML
WiFiServer server(80);

//tipe data frekuensi sensor warna
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

//tipe data warna
int redColor = 0;
int greenColor = 0;
int blueColor = 0;

// Variable untuk menerima permintaan HTTP
String header;

// variable untuk menyimpan status output
String statusPisang1 = "off";

// variable untuk GPIO 
const int statusPisang = 1;

void setup() 
{
  //protokol komuniikasi serial 
  Serial.begin(115200);

  //inisialisasi GPIO sensor warna
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  
  // inisialisasi variable sebagai output ESP32
  pinMode(statusPisang, OUTPUT);
  
  // OUTPUT normally LOW
  digitalWrite(statusPisang, LOW);

   // Setting frequency sensor warna scaling ke 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);

  // koneksi dengan SSID dn Password
  Serial.print("menyambungkan.. ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED) 
        {// validasi koneksi
        delay(500);
        Serial.print(".");
        }
  // monitoring sambungan koneksi
  Serial.println("");
  Serial.println("WiFi tersambung.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  scan.write(0);//servo posisi awal
  Serial.print("Servo Scanning Siap");
  WiFiClient client = server.available();  
  if (client) {                             
    Serial.println("Hallo kamu");          
    String currentLine = "";                
    while (client.connected()) 
        {   
        //SETTING KONFIGURASI DATA SENSOR WARNA 1
        // Setting RED (R) 
        digitalWrite(S2,LOW);
        digitalWrite(S3,LOW);
        redFrequency = pulseIn(sensorOut, LOW);
        redColor = map(redFrequency, 00, 00, 255,0); //mapping warna RGB, 00 diisi dengan kalibrasi warna yang diinginkan
        // Setting GREEN (G) 
        digitalWrite(S2,HIGH);
        digitalWrite(S3,HIGH);
        greenFrequency = pulseIn(sensorOut, LOW);
        greenColor = map(greenFrequency, 00, 00, 255, 0); //mapping warna RGB, 00 diisi dengan kalibrasi warna yang diinginkan
        // Setting BLUE (B)
        digitalWrite(S2,LOW);
        digitalWrite(S3,HIGH);
        blueFrequency = pulseIn(sensorOut, LOW);
        blueColor = map(blueFrequency, 00, 00, 255, 0); //mapping warna RGB, 00 diisi dengan kalibrasi warna yang diinginkan

      if (client.available()) 
        {             
        char a = client.read();             
        Serial.write(a);                    
        header += a;
          if (a == '\n') {                    
            if (currentLine.length() == 0) {
            client.println("HTTP OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // GPIO Handling
            if (header.indexOf("GET /1/read") >= 0) 
              {
                baca;//1
                delay(500);
                scan.write(30);
                delay(500);
                baca;//2
                delay(500);
                scan.write(60);
                delay(500);
                baca;//3
                delay(500);
                scan.write(90);
                delay(500);
                baca;//4
                delay(500);
                scan.write(120);
                delay(500);
                baca;//5
                delay(500);
                scan.write(150);
                delay(500);
                baca;//6
                delay(500);
                scan.write(180);
                delay(500);
                baca;//7
                delay(500);
                scan.write(0);//kembali Re-Loop
              } 
            else if (header.indexOf("GET /1/off") >= 0) 
              {
              Serial.println("GPIO 1 off");
              tangan1Bawah = "off";
              } 
            
            // Enable HTML
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // tema CSS untuk model button 
            // warna dan model bisa dirubah sesuai keinginan
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // JUDUL di halaman HTML
            client.println("<body><h1>Pengeraman Pisang</h1>");
            
            // Display status terkini GPIO 
            client.println("<p>GPIO 1 - Status " + statusPisang1 + redColor + greenColor + blueColor + "</p>");
            // Display status terkini button tangan bawah      
            if (statusPisang1=="off") 
              {
              client.println("<p><a href=\"/1/on\"><button class=\"button\">READ</button></a></p>");
              } 
            else 
              {
              client.println("<p><a href=\"/1/off\"><button class=\"button button2\">OFF</button></a></p>");
              } 
               
            client.println("</body></html>");
            
            // print blank pada serial monitor tanda HTTP request selesai
            client.println();
            // Break / selesai saat while loop sudah dijalankan
            break;
          } 
          else 
          { //baris alinea baru untuk menghapus karakter lama saat loading
            currentLine = "";
          }
        } else if (a != '\r') 
          {  
          currentLine += a;//karakter tambahan saat komunikasi serial terbaca diletakan pada akhir karakter
          }
      }
    }
    //Header kosong
    header = "";
    //Koneksi selesai
    client.stop();
    Serial.println("Sampai Jumpa");
    Serial.println("");
  }
}

void baca()
  {
              Serial.println("GPIO 1 read");
              tangan1Bawah = "read";
              //print red
              Serial.print("R = ");
              Serial.print(redColor);
              //print green
              Serial.print("G = ");
              Serial.print(greenColor);
              //print blue
              Serial.print("B = ");
              Serial.print(blueColor);
  }
