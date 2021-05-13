/*_____________CREATED BY_____________*/
      /*Artahsasta Bima Prafista*/
         /*Malang, Indonesia*/
/*_____________CREATED BY_____________*/

/*_____________HEADER_____________*/
#include <WiFi.h>                                                
#include <IOXhop_FirebaseESP32.h>                                             
#define FIREBASE_HOST "XXXXXX "// the project name address from firebase id
#define FIREBASE_AUTH "ztRgolkRUMKEZzaKXmyAC2ZUlGcuWojXXXXXXXXX"// the secret key generated from firebase
#define WIFI_SSID "Ashish"// input your home or public wifi name
#define WIFI_PASSWORD "XXXXXXXX"//password of wifi ssid
/*_____________HEADER_____________*/

/*_____________INISIALISASI DATA_____________*/
String fireStatus = "";// led status received from firebase
int led = 2;                                                               
/*_____________INISIALISASI DATA_____________*/


void setup() {

  Serial.begin(9600);
  delay(1000);

  /*_____________GPIO_____________*/
  pinMode(2, OUTPUT);                
  /*_____________GPIO_____________*/

  /*_____________WIFI CONNECTION_____________*/
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                      
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) 
    {
      Serial.print(".");
      delay(500);
    }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());//print local IP address
  /*_____________WIFI CONNECTION_____________*/

  /*_____________DATABASE CONNECTION_____________*/
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);// connect to firebase
  Firebase.setString("LED_STATUS", "OFF");//send initial string of led status
  /*_____________DATABASE CONNECTION_____________*/

}

void loop() {
  
  fireStatus = Firebase.getString("LED_STATUS");// get led status input from firebase
  if (fireStatus == "ON") 
    {// compare the input of led status received from firebase
      Serial.println("Led Turned ON");                 
      digitalWrite(2, HIGH);// make output led ON
    }
  else if (fireStatus == "OFF") 
    {// compare the input of led status received from firebase
      Serial.println("Led Turned OFF");
      digitalWrite(2, LOW);// make output led OFF
    }
  else 
    {
      Serial.println("Wrong Credential! Please send ON/OFF");
    }
}
