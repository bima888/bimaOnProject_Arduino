#include <TinyGPS++.h> // library untuk modul GPS
#include <SoftwareSerial.h> //Library untuk komunikasi Software Serial
#include <ESP8266WiFi.h> //Komunikasi untuk komunikasi ESP8266 WIFI

TinyGPSPlus gps;  // The TinyGPS++ object
SoftwareSerial SIM800L(4, 5); // Digital 1,2, Tx,Rx
//SoftwareSerial SIM800L(14, 12); // Digital 5,6, Tx,Rx

//ID koneksi
const char* ssid = "Pixel"; //Username Hostspot
const char* password = "bima8888"; //password Hotspot
//ID koneksi

//Tipe data
float latitude , longitude;
int year , month , date, hour , minute , second;
String date_str , time_str , lat_str , lng_str;
int pm;
//Tipe data

WiFiServer server(80);

void setup()
{
  //komunikasi Serial
  Serial.begin(9600);
  SIM800L.begin(9600);
//  SIM800L.begin(9600);
  //komunikasi Serial
  
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
  Serial.println("Server started"); // WebServer dimulai
  Serial.println(WiFi.localIP());  // Print IP address


}


void loop()
{
   //Validasi internet
      while (WiFi.status() != WL_CONNECTED)
      {
        
        Serial.println("Reconnecting");
        Serial.println(ssid);
        WiFi.begin(ssid, password);
        delay(5000);
        server.begin();
        Serial.println("WiFi connected");
        Serial.println("Server started");
        Serial.println(WiFi.localIP());
        Serial.println("NO SMS");
        //fungsi sms saat lost connection
  if (WiFi.status()!= WL_CONNECTED)
    {
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    delay(5000);
    SIM800L.println("AT+CMGF=1\r");
    delay(1000);
    //SIM800L.println("AT+CNMI=2,2,0,0,0"); //enable instruksi SMS
    //SIM800L.println("AT+CMGF=1");   //enable instruksi SMS
    delay(1000);  
    SIM800L.print("AT+CMGS=\"089515480619\"\r");//nomor telepon tujuan SMS yang diganti
    delay(1000);  
    SIM800L.print("LAT");
    SIM800L.println(latitude, 6);
    SIM800L.write("LONG");
    SIM800L.println(longitude, 6);
    delay(100);
    SIM800L.write((char)26);
    //SIM800L.println(0x1A);
    delay(1000);
    Serial.println("SMS TERKIRIM");
    }
  //fungsi sms saat lost connection
      }
     //Validasi internet
  
  
  

  //fungsi GPS dan Realtime waktu
  while (Serial.available() > 0) 
    if (gps.encode(Serial.read()))// pembacaan GPS
    {
      if (gps.location.isValid())      {
        latitude = gps.location.lat();
        lat_str = String(latitude , 6); // latitude di convert STRING   
        longitude = gps.location.lng();
        lng_str = String(longitude , 6);       
        }
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
