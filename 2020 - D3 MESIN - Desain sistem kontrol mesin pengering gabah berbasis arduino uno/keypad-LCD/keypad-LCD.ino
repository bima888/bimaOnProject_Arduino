#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

const byte rows = 4;
const byte cols = 4;

char keys[rows][cols] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[rows] = {11,10,9,8};
byte colPins[cols] = {7,6,5,4};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int MIN = 0;
int MAX = 0;
char stringAngka[17];
int indexKeypad = 0;

void setup()
{
  lcd.begin();                      // initialize the lcd 
  //lcd.init();
  Serial.begin(9600);
  // Print a message to the LCD.
  lcd.backlight();
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  lcd.setCursor(0,0);
  lcd.print("Input Suhu");
  lcd.setCursor(0,1);
  lcd.print("Nilai :");
  //lcd.print("You Pressed:");

}

void loop(){
   MIN = GetNumber();
   lcd.setCursor(0,1);
   lcd.print("set min ");
   lcd.setCursor(8,1);
   lcd.print(MIN);
   Serial.print ("Data Suhu Minimal = ");
   Serial.print (MIN);
   MAX = GetNumber();
   lcd.setCursor(0,1);
   lcd.print("set max ");
   lcd.setCursor(8,1);
   lcd.print(MAX);
   Serial.print ("Data Suhu Maximal = ");
   Serial.print (MAX);

   if(MAX == 20)
   {
    digitalWrite(2,LOW);
    delay(3000);
    digitalWrite(2,HIGH);    
    }
   else
   {digitalWrite(2,HIGH);
    }
   if(MIN == 10)
   {
    digitalWrite(3,LOW);
    delay(3000);
    digitalWrite(3,HIGH);        
    }
   else{digitalWrite(3,HIGH);
    }
}

int GetNumber()
{
   int num = 0;
   char key = keypad.getKey();
   while(key != '#')
   {
      switch (key)
      {
         case NO_KEY:
            break;

         case '0': case '1': case '2': case '3': case '4':
         case '5': case '6': case '7': case '8': case '9':
          if (!indexKeypad)
          {
          lcd.setCursor(8,1);
          }
          stringAngka[indexKeypad++] = key;
               
    
            lcd.print(key);
            //num = key - '0';
            num = num * 10 + (key - '0');
            break;

         case '*':
            num = 0;
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Input Suhu");
            lcd.setCursor(0,1);
            lcd.print("Nilai :");
           
            
            break;
      }

      key = keypad.getKey();
   }

   return num;
}
