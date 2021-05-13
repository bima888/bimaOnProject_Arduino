/*__FILE HEADER__*/   
#include <TinyGPS++.h> // library untuk modul GPS
#include <SoftwareSerial.h> //Library untuk komunikasi Software Serial
#include <FirebaseESP8266.h>//Library Firebase untuk ESP8266
#include <NTPClient.h>  //Library NTP
#include <WiFiUdp.h>  //Library Data User
#include <ESP8266WiFi.h> //Library Komunikasi untuk komunikasi ESP8266 WIFI
/*__FILE HEADER__*/

/*__INISIALISASI DATA__*/
TinyGPSPlus gps;  // The TinyGPS++ object
//SoftwareSerial ss(4, 5); // Digital 1,2, Tx,Rx
/*__INISIALISASI DATA__*/

/*__ID FIREBASE__*/
const char* url = "pasiensjw.firebaseio.com"; //Username Hostspot
const char* auth = "BvEvd0NZPijR8hqmGFgmZEEVhCLmgQTVrBpOiXWr"; //password Hotspot
FirebaseData object;
/*__ID FIREBASE__*/

/*___ID KONEKSI___*/
const char* ssid = "iMake Official"; //Username WiFi/Hostspot
const char* password = "ayobuatbuat"; //password WiFi/Hotspot
/*___ID KONEKSI___*/

/*__Alamat Alat__*/
String alat = "Alat1/";
//String alat = "Alat2/";
//String alat = "Alat3/";
/*__Alamat Alat__*/ 

/*__Setting NTP__*/
//set GMT +7
const long utcOffsetInSeconds = 25200;  //per +1 dikali 3600s
//set ntp
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
/*__Setting NTP__*/

/*__TIPE DATA__*/
float latitude , longitude, waktu, menit, detik;
int year , month , date, hour , minute , second;
String date_str , time_str , lat_str , lng_str;
int pm, jam;
/*__TIPE DATA__*/

/*__FUNGSI PARSING__*/
WiFiServer server(80);

/*__FUNGSI PARSING__*/

void setup()
{
  //komunikasi Serial
  Serial.begin(9600);
//  ss.begin(9600);
  //komunikasi Serial

  //GPIO
  pinMode(12,OUTPUT);
  pinMode(14,OUTPUT);
  //GPIO
  
  Serial.println();
  Serial.print("Connecting to "); //upaya menyambungkan
  Serial.println(ssid);// nama username Hotspot
  WiFi.begin(ssid, password); //memulai koneksi ke internet
  while (WiFi.status() != WL_CONNECTED)//kondisi ketika tidak tersambung ke internet
  {
    delay(500);
    Serial.print("."); //print "...." untuk display loading koneksi
  }  
  Serial.println("");
  Serial.println("WiFi connected"); // koneksi tersambung
  server.begin();
  delay(5000);
  Serial.println("Server started"); // WebServer dimulai
  Serial.println(WiFi.localIP());  // Print IP address
  Firebase.begin(url, auth);
  Firebase.reconnectWiFi(true);

  object.setBSSLBufferSize(1024, 1024);

  object.setResponseSize(1024);

  Firebase.setReadTimeout(object, 1000 * 60);
  Firebase.setwriteSizeLimit(object, "unlimited");
  timeClient.begin();
}

void loop()
{
  //fungsi reconnecting saat lost connection
  getConnection();

  //fungsi mengirim data ke firebase
  dataParse();
  
  //fungsi GPS dan Realtime waktu
  getGps();
    
  //fungsi web server
  webHandle(); 
}
