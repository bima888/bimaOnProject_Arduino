
void (*fungsi_reset) (void) = 0;

void setup() {

  //program awal
  
}

void loop() {

  //looping program 

  /*___________FUNGSI RESET____________*/
  for(int i = 18000; i > 0; i--) //delay 18000 detik (5 jam)
            {
              Serial.print("RESET");
              Serial.print("dalam ");
              Serial.print(i);
              delay(1000);
           
            }
            fungsi_reset();
  /*___________FUNGSI RESET____________*/
}
