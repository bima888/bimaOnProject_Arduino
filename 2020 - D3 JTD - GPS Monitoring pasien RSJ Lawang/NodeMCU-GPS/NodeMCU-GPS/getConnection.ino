void getConnection(){
  if (WiFi.status()!= WL_CONNECTED)
    {
    Serial.println("Tidak ada Koneksi");
    Serial.println("Sending Notification");
    delay(500);
    Serial.println("Reconnecting");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    Serial.print("."); //print "...." untuk display Loading koneksi 
    delay(5000);
    digitalWrite(12,HIGH);
    digitalWrite(14,LOW);
    Serial.println("LED GREEN OFF - RED ON");
    }
  if (WiFi.status() == WL_CONNECTED)
    {
    Serial.println("WiFi connected");
    Serial.println("Server started");
    Serial.println(WiFi.localIP());
    server.begin();
    //Validasi internet
    Serial.println("No Notification");
    digitalWrite(12,LOW);
    digitalWrite(14,HIGH);
    Serial.println("LED GREEN ON - RED OFF");
    Serial.println(latitude);
    Serial.println(longitude);
    
    delay(5000);
     Serial.print("Updating firebase..");
      timeClient.update();
      jam = timeClient.getHours();
      menit = timeClient.getMinutes();
      detik = timeClient.getSeconds();
      
      waktu = jam + (menit/100) + (detik/10000);
      dataParse();
      Serial.println("completed");
    }
}
