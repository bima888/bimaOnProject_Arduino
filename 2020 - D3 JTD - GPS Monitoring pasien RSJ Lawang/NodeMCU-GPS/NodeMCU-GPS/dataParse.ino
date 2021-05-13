void dataParse(){
  if(latitude!=0.00){
    Firebase.setString(object, "ESP/" + alat + "lat", lat_str);
    Firebase.setString(object, "ESP/" + alat + "lng", lng_str);
  }
  Firebase.setFloat(object, "ESP/" + alat + "time", waktu);
  Firebase.setString(object, "ESP/" + alat + "IP", WiFi.localIP().toString());
}
