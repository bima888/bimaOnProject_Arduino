#include <WiFi.h>  //header  komunikasi Wi-Fi ESP 32


/*____________________KOMUNIKASI INTERNET_____________________*/
const char* ssid     = "Pixel"; //ID hotspot (nama hotspot)
const char* password = "bimap8888"; //password hotspot 
WiFiServer server(80); //Server Address
/*____________________________________________________________*/

/*_________________INISIALISASI SENSOR WARNA______________*/
// Frekuensi Photo dioda
int redFrequency1 = 0;
int greenFrequency1 = 0;
int blueFrequency1 = 0;

// Tipe data Warna RGB
int redColor1 = 263;int redcon1;float redstat1;float resultred1;
int redColor1 = 263;int redcon1;float redstat1;float resultred1;
//tipe data (int) namaoffset;, tipe data (int) nama variable1;, tipe data (float) nama variable2;, tipe data (float) nama hasil;
//... dan seterusnya untuk nilai offset yang lain   
int greenColor1 = 0;
int blueColor1 = 128;int bluecon1;float bluestat1;float resultblue1;

// Frekuensi Photo dioda
int redFrequency2 = 0;
int greenFrequency2 = 0;
int blueFrequency2 = 0;
// Tipe data Warna RGB
int redColor2 = 0;
int greenColor2 = 0;
int blueColor2 = 0;
/*____________________________________________________________*/


void setup()
{
    //Baud Rate untuk Serial Monitor
    Serial.begin(115200);

    // Setting output
    // sensor1
    pinMode(12, OUTPUT);
    pinMode(13, OUTPUT);
    pinMode(14, OUTPUT);
    pinMode(27, OUTPUT);
    //sensor2
    pinMode(25, OUTPUT);
    pinMode(33, OUTPUT);
    pinMode(32, OUTPUT);
    pinMode(22, OUTPUT);
    
    // Setting input
    pinMode(23, INPUT);
    pinMode(26, INPUT);
    
    // Setting frequency scaling kelitian 100%
    digitalWrite(12,HIGH);
    digitalWrite(13,HIGH);
    digitalWrite(25,HIGH);
    digitalWrite(33,HIGH);
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
            client.print("<p>\n</p>");
            client.print("\n\nButton <a href=\"/ATAS\">ON</a> Deteksi 1 Dimulai.<br>");
            client.print("Button <a href=\"/BAWAH\">ON</a> Deteksi 2 Dimulai.<br>");
            client.print("Button <a href=\"/STATUS\">CHECK</a> Klik untuk hasil.<br>");
            client.print("<h1>KELOMPOK 5</h1>\n");
            client.print("<h3>Deteksi Anemia dini pada Ibu Hamil</h3>\n");    
            break;
          } else {  
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;      
        }

        // Validasi "GET /ATAS" atau "GET /BAWAH":
        if (currentLine.endsWith("GET /ATAS")) {// SISTEM /ATAS ON
          bacaatas();
             client.print("<p>Status RED Tangan atas : ");
             client.print((int)redColor1);
             client.print("%\n</p>");
             client.print("<p>Status GREEN Tangan atas : ");
             client.print((int)greenColor1);
             client.print("%\n</p>");
             client.print("<p>Status BLUE Tangan atas : ");
             client.print((int)blueColor1);
             client.print("%\n</p>");
            
        }
        if (currentLine.endsWith("GET /BAWAH")) {// SISTEM /BAWAH ON
          bacabawah();
             client.print("<p>Status RED Tangan bawah : ");
             client.print((int)redColor2);
             client.print("%\n</p>");
             client.print("<p>Status GREEN Tangan bawah : ");
             client.print((int)greenColor2);
             client.print("%\n</p>");
             client.print("<p>Status BLUE Tangan bawah : ");
             client.print((int)blueColor2);
             client.print("%\n</p>");
                     
        }
        if (currentLine.endsWith("GET /STATUS"))  {// SISTEM /STATUS ON
            
            if(resultred1>redColor1/*data offset tangan bawah*/ && resultred1>redColor1/*data offset tangan atas*/) //nilai offset dijadikan acuan yang akan dicapai / dilampaui untuk memenuhi kondisi aman
              {
              client.print("<p>Status aman</p>");
              }
              else //nilai dibawah offset akan dianggap tidak memenuhi syarat 
              {client.print("<p>Diagnosa awal Anemia, segera ke Dokter.</p>");}  

            //struktur program baru jika data yang diukur lebih dari 1 dengan kondisi warna tangan yang berbeda-beda dengan nilai offset yang berbeda juga 
            //if (data1 validasi offset 1){kondisi1}
            else if (resultblue1>blueColor1/*data offset tangan bawah*/ && resultblue1>blueColor1/*data offset tangan atas*/){kondisi2}
            //else if (data 3 validasi offset 3) {kondisi3}
            //catatan : jangan lupa untuk mendaftarkan (meng - inisialisasi) tipe data dari nilai / variable offset yang baru sebelum digunakan pada validasi ini
            
        }
      }
    }
    // close connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}






/*_________________FUNGSI SENSOR WARNA 1______________*/
void bacaatas()
{
    // Setting RED (R) 
  digitalWrite(14,LOW);
  digitalWrite(27,LOW);
  
  // pembacaan Output frekuensi
  redFrequency1 = pulseIn(26, LOW);
  redColor1 = map(redFrequency1, 70, 120, 255,0);
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
  digitalWrite(14,HIGH);
  digitalWrite(27,HIGH);
  
  // pembacaan Output frekuensi
  greenFrequency1 = pulseIn(26, LOW);
  greenColor1 = map(greenFrequency1, 100, 199, 255, 0);

  // Print GREEN (G)  
  Serial.print(" G = ");
  Serial.print(greenColor1);
  delay(100);
 
  // Setting BLUE (B) 
  digitalWrite(14,LOW);
  digitalWrite(27,HIGH);
  /*___RUMUS___*/
  //DATA 1
  bluestat1 = 1.14/100*blueColor1;
  resultblue1 = blueColor1-bluestat1;
  /*___RUMUS___*/
  
   // pembacaan Output frekuensi
  blueFrequency1 = pulseIn(26, LOW);
  blueColor1 = map(blueFrequency1, 38, 84, 255, 0);
  
  // Print BLUE (B) 
  Serial.print(" B = ");
  Serial.print(blueColor1);
  delay(100);
   
  // Tampilan di serial monitor
  if(redColor1 > greenColor1 && redColor1 > blueColor1){
      Serial.println("STATUS RGB - MERAH");
  }
  if(greenColor1 > redColor1 && greenColor1 > blueColor1){
    Serial.println("STATUS RGB - HIJAU");
  }
  if(blueColor1 > redColor1 && blueColor1 > greenColor1){
    Serial.println("STATUS RGB - BIRU");
  }
}
/*____________________________________________________________*/



/*_________________FUNGSI SENSOR WARNA 2______________*/
void bacabawah()
{
    // Setting RED (R) 
  digitalWrite(32,LOW);
  digitalWrite(22,LOW);
  
  // pembacaan Output frekuensi
  redFrequency2 = pulseIn(26, LOW);
  redColor2 = map(redFrequency2, 70, 120, 255,0);

  // Print RED (R)
  Serial.print("R = ");
  Serial.print(redColor2);
  delay(100);
  
  // Setting GREEN (G)
  digitalWrite(32,HIGH);
  digitalWrite(22,HIGH);
  
  // pembacaan Output frekuensi
  greenFrequency2 = pulseIn(26, LOW);
  greenColor2 = map(greenFrequency2, 100, 199, 255, 0);

  // Print GREEN (G)  
  Serial.print(" G = ");
  Serial.print(greenColor2);
  delay(100);
 
  // Setting BLUE (B) 
  digitalWrite(32,LOW);
  digitalWrite(22,HIGH);

   // pembacaan Output frekuensi
  blueFrequency2 = pulseIn(26, LOW);
  blueColor2 = map(blueFrequency2, 38, 84, 255, 0);
  
  // Print BLUE (B) 
  Serial.print(" B = ");
  Serial.print(blueColor2);
  delay(100);
   
  // Tampilan di serial monitor
  if(redColor2 > greenColor2 && redColor2 > blueColor2){
      Serial.println("STATUS RGB - MERAH");
  }
  if(greenColor2 > redColor2 && greenColor2 > blueColor2){
    Serial.println("STATUS RGB - HIJAU");
  }
  if(blueColor2 > redColor2 && blueColor2 > greenColor2){
    Serial.println("STATUS RGB - BIRU");
  }
}
/*____________________________________________________________*/
