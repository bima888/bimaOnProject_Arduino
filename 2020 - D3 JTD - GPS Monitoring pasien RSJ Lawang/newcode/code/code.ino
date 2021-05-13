#include <TinyGPS++.h> // library untuk modul GPS
#include <SoftwareSerial.h> //Library untuk komunikasi Software Serial
#include <ESP8266WiFi.h> //Komunikasi untuk komunikasi ESP8266 WIFI
#include <FirebaseESP8266.h>

TinyGPSPlus gps;  // The TinyGPS++ object
SoftwareSerial ss(4, 5); // Digital 1,2, Tx,Rx
SoftwareSerial SIM800L(14, 12); // Digital 5,6, Tx,Rx

//ID koneksi
const char* ssid = "Pixel"; //Username Hostspot
const char* password = "bima8888"; //password Hotspot
//ID koneksi

//Set static IP
IPAddress ip(192,168,43,10);
IPAddress subnet(255,255,255,0);
IPAddress gateway(192,168,43,1);
IPAddress dns(8,8,8,8);
//Set static IP

//ID firebase
const char* url = "pasiensjw.firebaseio.com/"; //Username Hostspot
const char* auth = "BvEvd0NZPijR8hqmGFgmZEEVhCLmgQTVrBpOiXWr"; //password Hotspot
FirebaseData object;
//ID firebase

//Tipe data
float latitude , longitude;
int year , month , date, hour , minute , second;
String date_str , time_str , lat_str , lng_str;
int pm;
//Tipe data

WiFiServer server(80);

void dataParse(){
  String ip = WiFi.localIP().toString() + "/";
  Firebase.setInt(object, "ESP/" + ip + "lat", latitude);
  Firebase.setInt(object, "ESP/" + ip + "lng", longitude);
  Firebase.setString(object, "ESP/" + ip + "time", time_str);
}

void setup()
{
  //komunikasi Serial
  Serial.begin(115200);
  ss.begin(9600);
  SIM800L.begin(9600);
  //komunikasi Serial
  
  Serial.println();
  Serial.print("Connecting to "); //upaya menyambungkan
  Serial.println(ssid);// nama username Hotspot
  WiFi.config(ip, gateway, subnet, dns);
  WiFi.begin(ssid, password); //memulai koneksi ke internet
  while (WiFi.status() != WL_CONNECTED)//kondisi ketika tidak tersambung ke internet
  {
    delay(500);
    Serial.print("."); //print "...." untuk display loading koneksi
  }
  Serial.println("");
  Serial.println("WiFi connected"); // koneksi tersambung
  server.begin();
  Serial.println("Server started"); // WebServer dimulai
  Serial.println(WiFi.localIP());  // Print IP address
  Firebase.begin(url, auth);
}

void loop()
{
  //fungsi sms saat lost connection
  if (WiFi.status()!= WL_CONNECTED)
    {
    Serial.println("Tidak ada Koneksi");
    Serial.println("Mengirim SMS");
    delay(500);
    Serial.println("Reconnecting");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    Serial.print("."); //print "...." untuk display Loading koneksi 
    
    SIM800L.println("AT+CNMI=2,2,0,0,0"); //enable instruksi SMS
    SIM800L.println("AT+CMGS=1");   //enable instruksi SMS
    delay(5000);  
    SIM800L.println("AT+CMGS=\"089515480619\"\r");//nomor telepon tujuan SMS yang diganti
    delay(1000);
    SIM800L.println("Latitude=");
    SIM800L.println(latitude, 6);
    SIM800L.println("Longitude=");
    SIM800L.println(longitude, 6);
    delay(100);
    //SIM800L.println((char)26);
    SIM800L.write(0x1A);
    delay(1000);
    }
  if (WiFi.status() == WL_CONNECTED)
    {
    Serial.println("WiFi connected");
    Serial.println("Server started");
    Serial.println(WiFi.localIP());
    //Validasi internet
    Serial.println("NO SMS");
      }
  //fungsi sms saat lost connection
  
  

  //fungsi GPS dan Realtime waktu
  while (ss.available() > 0) 
    if (gps.encode(ss.read()))// pembacaan GPS
    {
      if (gps.location.isValid()) // Validasi pembacaaan format ata GPS      {
        latitude = gps.location.lat();
        lat_str = String(latitude , 6); // latitude di convert STRING
        longitude = gps.location.lng();
        lng_str = String(longitude , 6); //longitude di convert STRING      }
      if (gps.date.isValid()) 
      {
        date_str = "";
        date = gps.date.day();
        month = gps.date.month();
        year = gps.date.year();
        if (date < 10)
          date_str = '0';
        date_str += String(date);
        date_str += " / ";

        if (month < 10)
          date_str += '0';
        date_str += String(month);
        date_str += " / ";
        if (year < 10)
          date_str += '0';
        date_str += String(year); 
      }
      if (gps.time.isValid())
      {
        time_str = "";
        hour = gps.time.hour();
        minute = gps.time.minute();
        second = gps.time.second();
        minute = (minute + 30);
        if (minute > 59)
        {
          minute = minute - 60;
          hour = hour + 1;
        }
        hour = (hour + 5) ;
        if (hour > 23)
          hour = hour - 24;
        if (hour >= 12)
          pm = 1;
        else
          pm = 0;
        hour = hour % 12;
        if (hour < 10)
          time_str = '0';
        time_str += String(hour);
        time_str += " : ";
        if (minute < 10)
          time_str += '0';
        time_str += String(minute);
        time_str += " : ";
        if (second < 10)
          time_str += '0';
        time_str += String(second);
        if (pm == 1)
          time_str += " PM ";
        else
          time_str += " AM ";
      }
    }

    if(WiFi.status()== WL_CONNECTED){
      Serial.print("Updating firebase..");
      dataParse();
      Serial.println("completed");
    }
 
 WiFiClient client = server.available(); //Validasi koneksi Client WebServer
  if (!client)
  {
    return;
  }
  // User Interface di WebServer 
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n <!DOCTYPE html> <html> <head> <title>GPS DATA</title> <style>";
  s += "a:link {background-color: YELLOW;text-decoration: none;}";
  s += "table, th, td </style> </head> <body> <h1  style=";
  s += "font-size:300%;";
  s += " ALIGN=CENTER> GPS DATA</h1>";
  s += "<p ALIGN=CENTER style=""font-size:150%;""";
  s += "> <b>Location Details</b></p> <table ALIGN=CENTER style=";
  s += "width:50%";
  s += "> <tr> <th>Latitude</th>";
  s += "<td ALIGN=CENTER >";
  s += lat_str;
  s += "</td> </tr> <tr> <th>Longitude</th> <td ALIGN=CENTER >";
  s += lng_str;
  s += "</td> </tr> <tr>  <th>Date</th> <td ALIGN=CENTER >";
  s += date_str;
  s += "</td></tr> <tr> <th>Time</th> <td ALIGN=CENTER >";
  s += time_str;
  s += "</td>  </tr> </table> ";
 
  s += "</body> </html>";

  client.print(s); // semua nilai dikirim ke webserver
  delay(100);
}
