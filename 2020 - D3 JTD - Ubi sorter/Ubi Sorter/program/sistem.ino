void sistem() //sistem utama
{
      //tipe data pembacaan sensor
      float thermopileValue = analogRead(tpv); 
      float thermistorValue = analogRead(trv); 
      
      //rumus nilai tegangan Thermistor
      v1 = (thermistorValue / 1024) * 5; 
      r = -(v1*1000)/(v1-5); 
      ambtemp = a + b * sqrt(1+c*r) + d*pow(r,5) + e*pow(r,7); 

      //rumus nilai tegangan Thermopile
      comp = k * (pow(ambtemp,4-delta)-pow(reftemp,4-delta));  
      v2 = (thermopileValue / 1024) * 5 + comp - verr - shiftv; 
      objtemp = pow((v2+k*pow(ambtemp,4-delta))/k, 1/(4-delta));
      
      Serial.println("WiFi Tersambung kembali.");
      Serial.println("Sistem Berjalan");
      Serial.println(WiFi.localIP());
      
      Serial.print("ambient temp = ");
      Serial.println(ambtemp);
      Serial.print("object temp = ");
      Serial.println(objtemp);
      Serial.println();
      Serial.print("V Termophile = ");
      Serial.println(v2);
      Serial.print("V ambient = ");
      Serial.println(comp);
      Serial.println(); 
      delay(3000);       
}
