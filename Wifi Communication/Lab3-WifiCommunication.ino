
#ifndef __CC3200R1M1RGC__
#include <SPI.h>
#endif
#include <WiFi.h>

char ssid[] = "wifilab";
char password[] = "password1234";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);      
  pinMode(RED_LED, OUTPUT);     

  Serial.print("Connecting to: ");
  Serial.println(ssid); 
  
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  
  Serial.println("\nConnected !");
  Serial.println("Waiting for an ip address");
  
  while (WiFi.localIP() == INADDR_NONE) {
    Serial.print(".");
    delay(300);
  }

  Serial.print("\nIP Address:");

  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  Serial.print("Goto http://");
  Serial.println(ip);
  server.begin();                          
}

void loop() {
  int i = 0;
  WiFiClient client = server.available();

  if (client) {                            
    char buffer[150] = {0};                
    while (client.connected()) {           
      if (client.available()) {             
        char c = client.read();            
        if (c == '\n') {                    
          if (strlen(buffer) == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.println("<!DOCTYPE html>");
            client.println("<html>");
            client.println("<body style=\"background-color:#202020;\">");
            client.println("<h1 align=center><font color=#FFFFFF>Lab3- Wifi Communication</font></h1><br><br><br><br><br><br>");
            client.println("<button type=\"button\" style=\"margin:auto;background-color:#16B74C;font-size : 45px;color: white;display:block;width:250px;height:150px;\" onclick=\"location.href='/O'\">LIGHT ON</button><br><br>");
            client.println(" <button type=\"button\" style=\"margin:auto;background-color:#F93737;font-size : 42px;color: white;display:block;width:250px;height:150px;\" onclick=\"location.href='/X'\">LIGHT OFF</button><br>");
            client.println("</body>");
            client.println("</html>");
            client.println();
            break;
          }
          else {  
            memset(buffer, 0, 150);
            i = 0;
          }
        }
        else if (c != '\r') {    
          buffer[i++] = c;     
        }
        
        if (endsWith(buffer, "GET /O")) {
          digitalWrite(RED_LED, HIGH);               
        }
        if (endsWith(buffer, "GET /X")) {
          digitalWrite(RED_LED, LOW);                
        }
      }
    }
    client.stop();
  }
}

boolean endsWith(char* inString, char* compString) {
  int compLength = strlen(compString);
  int strLength = strlen(inString);
  int i;
  for (i = 0; i < compLength; i++) {
    char a = inString[(strLength - 1) - i];
    char b = compString[(compLength - 1) - i];
    if (a != b) {
      return false;
    }
  }
  return true;
}

