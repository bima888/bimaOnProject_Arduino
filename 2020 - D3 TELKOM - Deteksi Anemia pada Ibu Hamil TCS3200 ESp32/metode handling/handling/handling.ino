#include <WiFi.h>
//GPIO sensor warna 1
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8

//GPIO sensor warna 2
#define S00 9
#define S11 10
#define S22 11
#define S33 12
#define sensorOut1 15


//define protokol komunikasi ESP32
//diisi dengan ID dan PASSWORD WIFI
const char* ssid     = "Pixel";
const char* password = "";

//Set web server port ke 80
//untuk HTTP request ke HTML
WiFiServer server(80);

//tipe data frekuensi sensor warna 1
int redFrequencysatu = 0;
int greenFrequencysatu = 0;
int blueFrequencysatu = 0;

//tipe data frekuensi sensor warna 2
int redFrequencydua = 0;
int greenFrequencydua = 0;
int blueFrequencydua = 0;


//tipe data warna 1
int redColorsatu = 0;
int greenColorsatu = 0;
int blueColorsatu = 0;

//tipe data warna 2
int redColordua = 0;
int greenColordua = 0;
int blueColordua = 0;

// Variable untuk menerima permintaan HTTP
String header;

// variable untuk menyimpan status output
String tangan1Bawah = "off";
String tangan2Atas = "off";

// variable untuk GPIO 
const int tangan1 = 1;
const int tangan2 = 2;

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
  pinMode(tangan1, OUTPUT);
  pinMode(tangan2, OUTPUT);
  
  // OUTPUT normally LOW
  digitalWrite(tangan1, LOW);
  digitalWrite(tangan2, LOW);

   // Setting frequency scaling ke 20%
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
        redFrequencysatu = pulseIn(sensorOut, LOW);
        redColorsatu = map(redFrequencysatu, 00, 00, 255,0); //mapping warna RGB, 00 diisi dengan kalibrasi warna yang diinginkan
        // Setting GREEN (G) 
        digitalWrite(S2,HIGH);
        digitalWrite(S3,HIGH);
        greenFrequencysatu = pulseIn(sensorOut, LOW);
        greenColorsatu = map(greenFrequencysatu, 00, 00, 255, 0); //mapping warna RGB, 00 diisi dengan kalibrasi warna yang diinginkan
        // Setting BLUE (B)
        digitalWrite(S2,LOW);
        digitalWrite(S3,HIGH);
        blueFrequencysatu = pulseIn(sensorOut, LOW);
        blueColorsatu = map(blueFrequencysatu, 00, 00, 255, 0); //mapping warna RGB, 00 diisi dengan kalibrasi warna yang diinginkan

        //SETTING KONFIGURASI DATA SENSOR WARNA 2
        // Setting RED (R) 
        digitalWrite(S22,LOW);
        digitalWrite(S33,LOW);
        redFrequencydua = pulseIn(sensorOut1, LOW);
        redColordua = map(redFrequencydua, 00, 00, 255,0); //mapping warna RGB, 00 diisi dengan kalibrasi warna yang diinginkan
        // Setting GREEN (G) 
        digitalWrite(S22,HIGH);
        digitalWrite(S33,HIGH);
        greenFrequencydua = pulseIn(sensorOut1, LOW);
        greenColordua = map(greenFrequencydua, 00, 00, 255, 0); //mapping warna RGB, 00 diisi dengan kalibrasi warna yang diinginkan
        // Setting BLUE (B)
        digitalWrite(S22,LOW);
        digitalWrite(S33,HIGH);
        blueFrequencydua = pulseIn(sensorOut1, LOW);
        blueColordua = map(blueFrequencydua, 00, 00, 255, 0); //mapping warna RGB, 00 diisi dengan kalibrasi warna yang diinginkan
        
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
              Serial.println("GPIO 1 read");
              tangan1Bawah = "read";
              //print red
              Serial.print("R = ");
              Serial.print(redColorsatu);
              //print green
              Serial.print(" G = ");
              Serial.print(greenColorsatu);
              //print blue
              Serial.print(" B = ");
              Serial.print(blueColorsatu);
              } 
            else if (header.indexOf("GET /1/off") >= 0) 
              {
              Serial.println("GPIO 1 off");
              tangan1Bawah = "off";
              } 
            else if (header.indexOf("GET /2/read") >= 0) 
              {
              Serial.println("GPIO 2 read");
              tangan2Atas = "read";
              //print red
              Serial.print("R = ");
              Serial.print(redColordua);
              //print green
              Serial.print(" G = ");
              Serial.print(greenColordua);
              //print blue
              Serial.print(" B = ");
              Serial.print(blueColordua);
              } 
            else if (header.indexOf("GET /2/off") >= 0) 
              {
              Serial.println("GPIO 2 off");
              tangan2Atas = "off";
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
            client.println("<body><h1>ANEMIA TEST</h1>");
            
            // Display status terkini GPIO 
            client.println("<p>GPIO 1 - Status " + tangan1Bawah + redColorsatu + greenColorsatu + blueColorsatu + "</p>");
            // Display status terkini button tangan bawah      
            if (tangan1Bawah=="off") 
              {
              client.println("<p><a href=\"/1/on\"><button class=\"button\">READ</button></a></p>");
              } 
            else 
              {
              client.println("<p><a href=\"/1/off\"><button class=\"button button2\">OFF</button></a></p>");
              } 
               
            // Display status terkini button tangan atas  
            client.println("<p>GPIO 2 - Status " + tangan2Atas + redColordua + greenColordua + blueColordua + "</p>");
            // jika statusAtas off, displaynya READ button       
            if (tangan2Atas=="off") 
              {
              client.println("<p><a href=\"/2/on\"><button class=\"button\">READ</button></a></p>");
              } 
            else 
              {
              client.println("<p><a href=\"/2/off\"><button class=\"button button2\">OFF</button></a></p>");
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
