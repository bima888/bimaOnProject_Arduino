//----------------------------------
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);           // select the pins used on the LCD panel
//----------------------------------
int Li          = 17;
int Lii         = 1; 
int Ri          = -1;
int Rii         = -1;
//----------------------------------
void setup(){
  lcd.begin(16, 2);                           // start the LCD library
}
//---------------------------------- 
void loop(){
  for (int i = 0; i <= 15; i++) {
    lcd.setCursor(0, 0);
    lcd.print(Scroll_LCD_Right("Arduino Lesson"));
    delay(350);
  }
  for (int i = 0; i <= 50; i++) {                                       // 50 is the result of the lcd = 16 column plus the length of the string, in this example string length 34.                                       
    lcd.setCursor(0, 1);                                                
    lcd.print(Scroll_LCD_Left("This is LCD 16x2 Scrolling Text..."));   // So from the start the text appears until the text disappears with a value of 50.
    delay(350);
  }
  Clear_Scroll_LCD_Left();                    // Use if the text in the string is changed / different from before
  for (int i = 16; i <= 30; i++) {
    lcd.setCursor(0, 0);
    lcd.print(Scroll_LCD_Right("Arduino Lesson"));
    delay(350);
  }
  Clear_Scroll_LCD_Right();
  for (int i = 0; i <= 12; i++) {
    lcd.setCursor(0, 0);
    lcd.print(Scroll_LCD_Right("Uteh Str"));
    delay(350);
  } 
  for (int i = 0; i <= 73; i++) {
    lcd.setCursor(0, 1);
    lcd.print(Scroll_LCD_Left("Welcome to Uteh Str Channel. Don't forget to Subscribe..."));
    delay(350);
  }
  Clear_Scroll_LCD_Left();
  for (int i = 13; i <= 24; i++) {
    lcd.setCursor(0, 0);
    lcd.print(Scroll_LCD_Right("Uteh Str"));
    delay(350);
  }
  Clear_Scroll_LCD_Right();
}
//----------------------------------
String Scroll_LCD_Left(String StrDisplay){
  String result;
  String StrProcess = "                " + StrDisplay + "                ";
  result = StrProcess.substring(Li,Lii);
  Li++;
  Lii++;
  if (Li>StrProcess.length()){
    Li=16;
    Lii=0;
  }
  return result;
}

void Clear_Scroll_LCD_Left(){
  Li=16;
  Lii=0;
}
//----------------------------------
String Scroll_LCD_Right(String StrDisplay){
  String result;
  String StrProcess = "                " + StrDisplay + "                ";
  if (Rii<1){
    Ri  = StrProcess.length();
    Rii = Ri-16;
  }
  result = StrProcess.substring(Rii,Ri);
  Ri--;
  Rii--;
  return result;
}

void Clear_Scroll_LCD_Right(){
  Ri=-1;
  Rii=-1;
}
