void parsing() //sistem parsing data ke Firebase
{
      Firebase.pushFloat("ambient/data",ambtemp);
      Firebase.setFloat("ammbient/latest",ambtemp);
      Firebase.pushFloat("objek/data",objtemp);
      Firebase.setFloat("objek/latest",objtemp);
      Firebase.pushFloat("Vter/data",v2);
      Firebase.setFloat("Vter/latest",v2);
      Firebase.pushFloat("Vamb/data",comp);
      Firebase.setFloat("Vamb/latest",comp);           
      Serial.println("Parsing data ke Firebase");
      delay(1000);
      Serial.println("Parsing Selesai");

}
