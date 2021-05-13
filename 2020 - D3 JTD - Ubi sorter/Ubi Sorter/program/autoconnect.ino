void autoconnect() //sistem auto re-connect saat signal loss
{
while(WiFi.status()!=WL_CONNECTED)
    {
      Serial.println("Tidak ada Koneksi");
      Serial.println("Sistem Berhenti");
      Serial.println("Menyambungkan");
      Serial.println(ssid);
      WiFi.begin(ssid, password);
      Serial.println(".");
      delay(1000);
      }
}
