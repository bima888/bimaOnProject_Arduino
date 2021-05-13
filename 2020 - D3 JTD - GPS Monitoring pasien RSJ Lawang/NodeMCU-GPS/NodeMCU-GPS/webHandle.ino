void webHandle(){
  WiFiClient client = server.available(); //Validasi koneksi Client WebServer
  if (!client)
  {
    return;
  }
  
  /*___User Interface di WebServer___*/ 
  
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n <!DOCTYPE html> <html> <head> <title>GPS DATA</title> <style>";
  s += "a:link {background-color: YELLOW;text-decoration: none;}";
  s += "table, th, td </style> </head> <body> <h1  style=";
  s += "font-size:300%;";
  s += " ALIGN=CENTER> GPS DATA</h1>";
  s += "<p ALIGN=CENTER style=""font-size:150%;""";
  s += "> <b>Location Details</b></p> <table ALIGN=CENTER style=";
  s += "width:50%";
  s += "> <tr> <th>Latitude</th>";
  s += "<td ALIGN=CENTER >";
  s += lat_str;
  s += "</td> </tr> <tr> <th>Longitude</th> <td ALIGN=CENTER >";
  s += lng_str;
  //s += "</td> </tr> <tr>  <th>Date</th> <td ALIGN=CENTER >";
  //s += date_str;
  //s += "</td></tr> <tr> <th>Time</th> <td ALIGN=CENTER >";
  //s += time_str;
  s  += "</td>  </tr> </table> "; 
  s += "</body> </html>";
  client.print(s); // semua nilai dikirim ke webserver
  delay(100);
}
