void getGps(){
  while (Serial.available() > 0) 
    if (gps.encode(Serial.read()))// pembacaan GPS
    {
       if (gps.location.isValid()) // Validasi pembacaaan format ata GPS      
       {
        latitude = gps.location.lat();
        lat_str = String(latitude , 6); // latitude di convert STRING
        longitude = gps.location.lng();
        lng_str = String(longitude , 6); //longitude di convert STRING      
       }
    }
}
