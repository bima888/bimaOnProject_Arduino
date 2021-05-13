/*PINOUT*/
/*
sensor Ultrasonic 1 = vcc - 5V ; GND - GND ; Trigger - PIN 10 ; echo - PIN 9
sensor Ultrasonic 2 = vcc - 5V ; GND - GND ; Trigger - PIN 12 ; echo - PIN 11
sensor Infrared     = vcc (coklat) - 5V ; GND (putih) - GND ; Data (abu-abu) - PIN 3
Relay               = vcc - 5V ; GND - GND ; IN1 - PIN 6 ; IN2 - PIN 7 
*/
/*PINOUT*/

//OUTPUT relay
#define OUT 6
//#define OUT2 7 //spare Output tambahan di PIN 7
//ultrasonic 1
#define echo1 9
#define trigger1 10
//ultrasonic 2
#define echo2 11
#define trigger2 12
//Infrared
//#define IRED 3
//tipe data variable sensor
long jarak1,jarak2, waktu1,waktu2;

void setup() {
Serial.begin(9600);
//inisialisasi sensor Infrared sebagai input interrupt Pull up (Diff. down)
pinMode(IRED,INPUT_PULLUP);
//inisialisasi echo sensor sebagai input
pinMode(echo1, INPUT);
pinMode(echo2, INPUT);
//inisialisasi trigger sensor sebagai output
pinMode(trigger1, OUTPUT);
pinMode(trigger2, OUTPUT);
//inisialisasi relay sebagai OUTPUT
pinMode(OUT, OUTPUT);
//set awal OUTPUT OFF
digitalWrite(OUT, HIGH);

}

void loop() {

//Ultra1
//struktur jangan dibalik
  digitalWrite(trigger1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger1, LOW);
  waktu1 = pulseIn(echo1, HIGH);
//Ultra2
//struktur jangan dibalik
  digitalWrite(trigger2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger2, LOW);
  waktu2 = pulseIn(echo2, HIGH); 

  //variable jarak satuan cm 
  jarak1 = (waktu1/2)/29.1;  
  jarak2 = (waktu2/2)/29.1;
  delay(50);//delay sekian detik
  
Serial.print("Jarak sensor 1 = ");
Serial.println(jarak1);
Serial.print("Jarak sensor 2 = ");
Serial.println(jarak2);

  if (jarak1 > 0 && jarak1 <= 200) //atur jarak (cm) sensor 1
    {
      digitalWrite(OUT, LOW);//ON
      attachInterrupt(digitalPinToInterrupt(3),RED,RISING);
      }
  if (jarak2 >= 0 && jarak2 <= 200) //atur jarak (cm) sensor 2
    {
        if(jarak2<200)
        {
        Serial.println("sensor jarak 2 Deteksi");
        digitalWrite(OUT, HIGH);//OFF
        jarak1 = -100;
        delay(5000); //delay dari sensor 2
        }
        
      }
      delay(100);
}

////fungsi interrupt
//void RED ()
//{
//  Serial.println("InfraRed Deteksi");
//  delay(500);
//  Serial.println("OUTPUT OFF - SYSTEM RELOOP");
//  delay(200); 
//  }
