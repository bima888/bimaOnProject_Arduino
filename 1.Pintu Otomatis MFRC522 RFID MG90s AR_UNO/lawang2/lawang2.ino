//Include sections
#include <MFRC522.h>
#include <SPI.h>
#include <LiquidCrystal.h>
#include <Servo.h>

//Define Component to Arduino Pins
#define SS_PIN 10
#define RST_PIN 9
#define SERVO_PIN 5
#define Buzzer 8

//initialize the library with the numbers of the interface pins
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
Servo DoorLock;
MFRC522 rfid(SS_PIN, RST_PIN );

void setup() 
{
  pinMode(Buzzer,OUTPUT); 
  DoorLock.attach(SERVO_PIN);
  Serial.begin(9600); 
  lcd.begin(16,2);
  SPI.begin();
  rfid.PCD_Init();
}

void loop() 
{
  
  DoorLock.write(0); //Servo mengunci
  lcd.clear();
  lcd.print("Selamat Datang");
    
  if ( ! rfid.PICC_IsNewCardPresent()) 
  {
    return;
  }
  if ( ! rfid.PICC_ReadCardSerial()) 
  {
    return;
  }
  
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < rfid.uid.size; i++) //Validasi HEXA kartu
  {
     Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(rfid.uid.uidByte[i], HEX);
     content.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(rfid.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("KARTU : ");
  content.toUpperCase();
  if (content.substring(1) == "BD 31 15 2B") //Alamat UID Kartu 1
        {
              pintu;                 
        }
  else if (content.substring(1) == "BD 31 15 2B") //Alamat UID Kartu 2
        {
              pintu;                 
        }
  //else if ... dan seterusnya brooooo               
  else
          {
            Serial.println("\nGAK KENAL !");
            lcd.clear();
            lcd.print("NGALIHO");
            lcd.setCursor(0,1);
            lcd.print("JANCOK!");

            for(int i = 0; i < 7; i++)
            {
              digitalWrite(Buzzer, HIGH);
            
              delay(500);
              digitalWrite(Buzzer, LOW);
              
              delay(500);              
            }
            delay(1000);            
          }                 
}





void pintu()
  {
            Serial.println("hallo A");
            Serial.println();
            lcd.print("hallo A");
            delay(3000);
  
            //Lawang mbukak
            DoorLock.write(180);            
            delay(200);
            lcd.clear();                                    
            lcd.print("Silahkan");
            lcd.setCursor(0,1);
            lcd.print("Masuk");
            delay(2000);
            lcd.clear();
            
            //Hitung mundur nutup lawang
               for(int i = 10; i > 0; i--)
            {
              lcd.print("Pintu menutup");
              lcd.setCursor(0,1);
              lcd.print("dalam ");
              lcd.print(i);
              delay(1000);
              lcd.clear();
            }

            //tutup
            DoorLock.write(0);
            delay(200);
            lcd.clear();
            lcd.print("Pintu tertutup");
            lcd.setCursor(0,1);
            delay(2000);                            
          
            
            //Pintu tertutup
            DoorLock.write(0);
            delay(200);
            lcd.clear();
            lcd.print("Pintu");
            lcd.setCursor(0,1);
            lcd.print("Tertutup");
            delay(2000);         
  }
