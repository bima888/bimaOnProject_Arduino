//File Header dan Library
#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h> 
#define FIREBASE_HOST "thermopile-d893f.firebaseio.com/" //URL Database
#define FIREBASE_AUTH "UUskL8bZuXS2cHhXHISgsWvLM5nO3guh4d7byrHZ" //Secret KEY
//ID pinout
#define tpv 35
#define trv 25


//ID koneksi
const char* ssid     = "Pixel"; //nama Hotspot
const char* password = "bima8888"; //password

//konstanta perhitungan Thermistor
const float a = -412.6;
const float b = 140.41;
const float c = 0.00764;
const float d = -0.0000000000000000625;
const float e = -0.00000000000000000000000115;

//konstanta perhitungan Thermopile
const float k = 0.004313; 
const float delta = 2.468;
const float reftemp = 25; // referensi Temperature 25C
const float shiftv = 0.6; // Shifting tegangan
const float verr = 0.6;  // Error Tegangan

//tipe data rumus
float v1 = 0;
float r = 0;
float ambtemp = 0;
float comp = 0;
float v2 = 0;
float objtemp = 0;

void setup()
{
    //komunikasi Serial
    Serial.begin(115200); 
    
    //GPIO
   // analogReadResolution(10);
    //analogSetAttenuation(ADC_0db);
    delay(10);

    //koneksi WiFi
    Serial.println();
    Serial.println();
    Serial.print("menyambungkan ke ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi Tersambung.");
    Serial.println("Sistem Berjalan");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());   

    //Komunikasi Firebase
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

}
void loop()
{
    autoconnect();
    if(WiFi.status()==WL_CONNECTED)
    {
      sistem();
      parsing();
      }
}
