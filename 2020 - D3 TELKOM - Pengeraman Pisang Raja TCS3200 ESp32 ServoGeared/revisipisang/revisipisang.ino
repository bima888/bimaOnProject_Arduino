#include <WiFi.h>  //header  komunikasi Wi-Fi ESP 32
#include <ESP32Servo.h> //header handling Servo
#include <DHT.h>   //header Sensor Suhu DHT
#include <OneWire.h> //header Sensor DS18B20
#include <DallasTemperature.h> //header Sensor DS18B20
#define DHTTYPE DHT11   //Inisialisasi jenis sensor
#define  ONE_WIRE_BUS 4 //Inisialisasi Pin sensor
#include <WiFiUdp.h>
#include <FirebaseESP32.h>

OneWire OneWire(ONE_WRE_BUS);
DallasTemperature sensors(&oneWire);

/*____________________KOMUNIKASI INTERNET_____________________*/
const char* ssid     = "BASECAMP PBT";        //Nama Jaringan WiFi
const char* password = "tahucampuryuyun";     //Password WiFi
WiFiServer server(80); //Server Address
/*____________________________________________________________*/

/*____________________KOMUNIKASI FIREBASE_____________________*/
const char* fb_host   = "bananadetector.firebaseio.com/"; //URL Firebase
const char* fb_token  = "iEkc8b224Xi0sfacPOwtLl9Rdr5vcFMptBrN1LBt"; //Token Firebase
FirebaseData object;
/*____________________________________________________________*/

/*_________________INISIALISASI GPIO______________*/
Servo servo;
uint8_t DHTPin = 22;
DHT dht(DHTPin, DHTTYPE);
int Temperature;
float Celcius = 0;
/*________________________________________________*/

/*_________________INISIALISASI SENSOR WARNA______________*/
// Frekuensi Photo dioda
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;
// Tipe data Warna RGB
// Tipe data Warna RGB
int redColor = 0; int redcon1 = 235 ; float redstat1; float resultred1;
//tipe data (int) namaoffset;, tipe data (int) nama variable1;, tipe data (float) nama variable2;, tipe data (float) nama hasil;
//... dan seterusnya untuk nilai offset yang lain
int greenColor = 0; int greencon1 = 210 ;          //offseet pada greencon1
int blueColor = 0;
/*______  ___________________________________________________*/

String DayHist;
int hari, limit;
int bacaatasmerah ;
int bacaatasijo ;
int bacaatasbiru ;

unsigned long waktuAwal = 0;
unsigned long waktuBatas = 8640000; //variabel lama waktu per milisecond
unsigned long waktuSekarang = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  // Setting output
  pinMode(12, OUTPUT); //warna s1
  pinMode(13, OUTPUT); //warna s2
  pinMode(27, OUTPUT); //warna s3
  pinMode(26, OUTPUT); //warna s4
  pinMode(32, OUTPUT); //Kipas
  pinMode(33, OUTPUT); //Lampu
  digitalWrite(32, LOW);
  digitalWrite(33, LOW);
  servo.attach(14);

  // Setting input
  pinMode(25, INPUT);
  pinMode(DHTPin, INPUT);
  dht.begin(); //DHT
  sensors.begin(); //DS18B20
  
  // Setting frequency scaling 100%
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);


  // Koneksi Dimulai
  Serial.println();
  Serial.println();
  Serial.print("Menyambungkan ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int n = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    n++;
    if (n == 10) {
      Serial.println("Connection Failed! Rebooting...");
      delay(3000);
      ESP.restart();
    }
  }

  //Koneksi Firebase
  Firebase.begin(fb_host, fb_token);
  Firebase.setString(object, "IP Address", WiFi.localIP().toString());

  
  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(object, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(object, "unlimited");

  //Event sukses
  Serial.println("");
  Serial.println("WiFi Tersambung.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(2, LOW);

  server.begin();
}


void loop() {
  // Kontrol via WebServer
  //webHandle();

  // Kontrol via Aplikasi
  sensors.requestTemperatures(); 
  FirebaseHandle();
}

void controlRelay(){
float temC = sensors.getTempCbyIndex(0);
  Serial.println(temC);
  if (temC <= 30.0) {  //batas bawah temperature
    digitalWrite(32, LOW);
    digitalWrite(33, HIGH);
    Firebase.setString(object, "Device/fan", "off");
    Firebase.setString(object, "Device/light", "on");
    Serial.println("Lampu Nyala");
  } 
  if (temC >= 35.0) {  //batas atas temperature
    digitalWrite(32, HIGH);
    digitalWrite(33, LOW);
    Firebase.setString(object, "Device/fan", "on");
    Firebase.setString(object, "Device/light", "off");
    Serial.println("Kipas Mati");
  }
}
/*_________________Parse Data Firebase_____________*/
void parseData() {
  digitalWrite(2, HIGH);
  Serial.print("Parsing data..");
  Firebase.setInt(object, "Device/Colour/R", bacaatasmerah);
  Serial.print(".");
  Firebase.setInt(object, "Device/Colour/G", bacaatasijo);
  Serial.print(".");
  Firebase.setInt(object, "Device/Colour/B", bacaatasbiru);
  Serial.print(".");
  Firebase.setFloat(object, "Device/Humidity", dht.readHumidity());
  Serial.print(".");
  Firebase.setFloat(object, "Device/Temp", sensors.getTempCByIndex(0)) //kalau tidak bisa ganti dengan, sensors.requestTemperatures(); 
  Serial.println("Completed");
  digitalWrite(2, LOW);
}
/*__________________________________________________*/


/*_________________Input Average Data Firebase_____________*/
void avgFB() {
  digitalWrite(2, HIGH);
  Serial.print("Updating average data..");
  Firebase.getInt(object, "Device/Colour/R");
  int r = object.intData();
  Firebase.setInt(object, "Device/Avg/Colour/R", (bacaatasmerah + r) / 2);
  Serial.print(".");
  Firebase.getInt(object, "Device/Colour/G");
  int g = object.intData();
  Firebase.setInt(object, "Device/Avg/Colour/G", (bacaatasijo + g) / 2);
  Serial.print(".");
  Firebase.getInt(object, "Device/Colour/B");
  int b = object.intData();
  Firebase.setInt(object, "Device/Avg/Colour/B", (bacaatasbiru + b) / 2);
  Serial.print(".");
  Firebase.getFloat(object, "Device/Temp");
  float temp = object.floatData();
  Firebase.setFloat(object, "Device/Avg/temp", ((sensors.getTempCByIndex(0) + temp) / 2)); //kalau tidak bisa ganti dengan, sensors.requestTemperatures();
  Serial.print(".");
  Firebase.getFloat(object, "Device/Humidity");
  float humi = object.floatData();
  Firebase.setFloat(object, "Device/Avg/humidity", ((dht.readHumidity() + humi) / 2));
  Serial.print(".");
  Serial.println("Completed");
  digitalWrite(2, LOW);
}
/*__________________________________________________*/

/*_________________Input New Data Firebase_____________*/
void newavgFB() {
  digitalWrite(2, HIGH);
  Serial.print("Saving new average data.");
  Firebase.setInt(object, "Device/Avg/Colour/R", bacaatasmerah);
  Serial.print(".");
  Firebase.setInt(object,"Device/Avg/Colour/G", bacaatasijo);
  Serial.print(".");
  Firebase.setInt(object,"Device/Avg/Colour/B", bacaatasbiru);
  Serial.print(".");
  Firebase.setFloat(object,"Device/Avg/temp", sensors.getTempCByIndex(0)); //kalau tidak bisa ganti dengan, sensors.requestTemperatures();
  Serial.print(".");
  Firebase.setFloat(object,"Device/Avg/humidity", dht.readHumidity());
  Serial.println("Completed");
  digitalWrite(2, LOW);
}
/*__________________________________________________*/

/*___________________Save History____________________*/
void saveHist() {
  //menyimpan data pada histori
  digitalWrite(2, HIGH);
  Serial.print("Saving history..");
  Firebase.getInt(object, "Device/Avg/Colour/R");
  Firebase.setInt(object, "Data/" + DayHist + "Colour/R", object.intData());
  Serial.print(".");
  Firebase.getInt(object, "Device/Avg/Colour/G");
  Firebase.setInt(object, "Data/" + DayHist + "Colour/G", object.intData());
  Serial.print(".");
  Firebase.getInt(object, "Device/Avg/Colour/B");
  Firebase.setInt(object, "Data/" + DayHist + "Colour/B", object.intData());
  Serial.print(".");
  Firebase.getString(object, "Device/pisang");
  Firebase.setString(object, "Data/" + DayHist + "Pisang", object.stringData());
  Serial.print(".");
  Firebase.getInt(object, "Device/Avg/temp");
  float temp = object.floatData();
  Firebase.setFloat(object, "Data/" + DayHist + "Temp", temp);
  Serial.print(".");
  Firebase.getInt(object, "Device/Avg/humidity");
  float humi = object.floatData();;
  Firebase.setFloat(object, "Data/" + DayHist + "Humidity", humi);
  Serial.println("Completed");
  digitalWrite(2, LOW);
  //selesai menyimpan
}

/*__________________Control via Firebase__________________*/
void FirebaseHandle() {
  controlRelay();
  Firebase.getString(object, "Device/state");
  String state = object.stringData();
  if ( state == "on") { //Meminta status dari firebase
    waktuSekarang = millis();
    rotate();     //scanning data
    Serial.println("Counting day..");
    Firebase.getInt(object, "Device/day");
    hari = object.intData();
    
    //Menyesuaikan Data Rata-Rata
    if (hari != 0) {
      avgFB();
    }
    else {
      newavgFB();
    }
    parseData();

    if(hari == 0){
      hari = 1;
    }
    
    if (waktuSekarang >= waktuBatas + waktuAwal) {
      hari++;
      waktuSekarang = waktuAwal;
    }
    Firebase.setInt(object, "Device/day", hari);
    Firebase.getInt(object, "Device/limit");
    limit = object.intData();
    DayHist = "Day1/";
    
    //membandingkan count hari ini dengan batas maksimum yang dipilih
    if (hari > limit) {
      Firebase.setString(object, "Device/state", "off");
      Firebase.setInt(object, "Device/limit", 0);
      Firebase.setInt(object, "Device/day", hari-1);
    } else if (hari == 1) {
      DayHist = "Day1/";
    } else if (hari == 2) {
      DayHist = "Day2/";
    } else if (hari == 3) {
      DayHist = "Day3/";
    } else if (hari == 4) {
      DayHist = "Day4/";
    } else if (hari == 5) {
      DayHist = "Day5/";
    } else if (hari == 6) {
      DayHist = "Day6/";
    } else if (hari == 7) {
      DayHist = "Day7/";
    }
    Serial.println("counted");    
    saveHist();
    //membandingkan data yang ada dengan offset
    if (bacaatasmerah > redcon1 && bacaatasijo > greencon1) {
      Firebase.setString(object, "Device/state", "off");
      Firebase.setString(object, "Device/pisang", "Matang");
    } else {
      Firebase.setString(object, "Device/pisang", "Proses");
    }

    //control saat status dari firebase off
  } else if (state == "off") {
    Serial.println("Device off..");
    //menghapus seluruh data internal firebase
    Firebase.getInt(object, "Device/delete");
    int delet = object.intData();
    if (delet == 1) {
      deleteFb();
    }
    
    baca();
    unsigned long rst = 0;
    waktuAwal = rst;
  }
}

/*_____________________Menghapus seluruh data firebase______________*/
void deleteFb() {
  Serial.println("Delete data...");
  Firebase.setInt(object, "Device/limit", 0);
  Firebase.setInt(object, "Device/time", 0);
  Firebase.setInt(object, "Device/day", 0);
  Firebase.setInt(object, "Device/Colour/R", 0);
  Firebase.setInt(object, "Device/Colour/G", 0);
  Serial.println("20%");
  Firebase.setInt(object, "Device/Colour/B", 0);
  Firebase.setInt(object, "Device/Avg/Colour/R", 0);
  Firebase.setInt(object, "Device/Avg/Colour/G", 0);
  Firebase.setInt(object, "Device/Avg/Colour/B", 0);

  Firebase.deleteNode(object, "Data/Day1/Pisang");
  Serial.println("40%");
  Firebase.deleteNode(object, "Data/Day1/Temp");
  Firebase.deleteNode(object, "Data/Day1/Humidity");

  Firebase.deleteNode(object, "Data/Day2/Pisang");
  Firebase.deleteNode(object, "Data/Day2/Temp");
  Firebase.deleteNode(object, "Data/Day2/Humidity");
  Serial.println("60%");

  Firebase.deleteNode(object, "Data/Day3/Pisang");
  Firebase.deleteNode(object, "Data/Day3/Temp");
  Firebase.deleteNode(object, "Data/Day3/Humidity");

  Firebase.deleteNode(object, "Data/Day4/Pisang");
  Firebase.deleteNode(object, "Data/Day4/Temp");
  Serial.println("80%");
  Firebase.deleteNode(object, "Data/Day4/Humidity");

  Firebase.deleteNode(object, "Data/Day5/Pisang");
  Firebase.deleteNode(object, "Data/Day5/Temp");
  Firebase.deleteNode(object, "Data/Day5/Humidity");

  Firebase.setInt(object, "Device/delete", 0);  
  Serial.println("100%");
  Serial.println("Completed");
}
/*________________________________________________________________*/

/*______________________Kontrol dengan web server_________________*/
void webHandle() {
  WiFiClient client = server.available();   // Komunikasi clients

  if (client) {
    Serial.println("New Client.");
    delay(5000);// Display saat ada event
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
            // Sensor Suhu aktif
            sensors.requestTemperatures(); 
            Temperature = sensors.getTempCByIndex(0); //kalau tidak bisa ganti dengan, sensors.requestTemperatures();
            client.print("Button <a href=\"/H\">ON</a> Deteksi Dimulai.<br>");
            client.print("Button <a href=\"/L\">OFF</a> Deteksi mati.<br>");
            client.print("<p>Status Temperatur : ");
            client.print(Temperature);
            client.print("C\n</p>");
            client.print("<h1>KELOMPOK 6</h1>\n");
            client.print("<h3>Monitoring Pengeraman Pisang dengan ESP32</h3>\n");
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
          // SISTEM /H untuk state on
          digitalWrite(32, HIGH);
          digitalWrite(33, LOW); //kipas
          //          baca();
          rotate();
          client.print("<p>Status RED : ");
          client.print((int)bacaatasmerah);
          client.print("%\n</p>");
          client.print("<p>Status GREEN : ");
          client.print((int)bacaatasijo);
          client.print("%\n</p>");
          client.print("<p>Status BLUE : ");
          client.print((int)bacaatasbiru);
          client.print("%\n</p>");

          if (bacaatasmerah > redcon1 && bacaatasijo > greencon1)
          {
            client.print("<p>Pisang sudah matang</p>");
            // SISTEM /L untuk state off
            digitalWrite(32, LOW);
            digitalWrite(33, HIGH);
            delay(500);
          }
          else
          {
            rotate();
            digitalWrite(32, HIGH);
            digitalWrite(33, HIGH); //kipas
            client.print("<p>Pisang dalam proses pengeraman</p>");
          }
        }
        if (currentLine.endsWith("GET /L")) {
          // SISTEM /L untuk state off
          digitalWrite(32, LOW);
          digitalWrite(33, HIGH);
          delay(500);
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
  Serial.println("Reading..");
  // Setting RED (R)
  digitalWrite(27, LOW);
  digitalWrite(26, LOW);

  // pembacaan Output frekuensi
  redFrequency = pulseIn(25, LOW);
  redColor = map(redFrequency, 70, 120, 255, 0);
  bacaatasmerah = redColor-371 ;

  // Print RED (R)
  Serial.print("R = ");
  Serial.print(bacaatasmerah);
  delay(100);

  // Setting GREEN (G)
  digitalWrite(27, HIGH);
  digitalWrite(26, HIGH);

  // pembacaan Output frekuensi
  greenFrequency = pulseIn(25, LOW);
  greenColor = map(greenFrequency, 100, 199, 255, 0);
  bacaatasijo = greenColor-294  ;
  
  // Print GREEN (G)
  Serial.print(" G = ");
  Serial.print(bacaatasijo);
  delay(100);

  // Setting BLUE (B)
  digitalWrite(27, LOW);
  digitalWrite(26, HIGH);

  // pembacaan Output frekuensi
  blueFrequency = pulseIn(25, LOW);
  blueColor = map(blueFrequency, 38, 84, 255, 0);
  bacaatasbiru = blueColor-239 ;

  // Print BLUE (B)
  Serial.print(" B = ");
  Serial.print(bacaatasbiru);
  delay(100);

  // Tampilan di serial monitor
  if (redColor > greenColor && redColor > blueColor) {
    Serial.println("STATUS RGB - MERAH");
  }
  if (greenColor > redColor && greenColor > blueColor) {
    Serial.println("STATUS RGB - HIJAU");
  }
  if (blueColor > redColor && blueColor > greenColor) {
    Serial.println("STATUS RGB - BIRU");
  }
  
  Serial.println("Completed");
}
/*____________________________________________________________*/

/*_________________FUNGSI SENSOR WARNA 2______________*/
void rotate()
{
  digitalWrite(2, HIGH);
  baca();//READ
  Serial.println("45");
  servo.write(45);
  delay(500);
  Serial.println("90");
  servo.write(90);
  delay(500);
  baca();
  Serial.println("135");
  servo.write(135);
  delay(500);
  Serial.println("180");
  servo.write(180);
  delay(500);
  baca();//READ
  Serial.println("0");
  servo.write(0);
  digitalWrite(2, LOW);
}
/*____________________________________________________________*/
