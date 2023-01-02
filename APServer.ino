

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

//SSID and Password to your ESP Access Point
const char* ssid = "ESPWebServer";
//const char* password = "12345678";

ESP8266WebServer server(80); //Server on port 80

//==============================================================
//     This rutine is exicuted when you open its IP in browser
//==============================================================
void handleRoot() {
  server.send(200, "text/html", "NodeMCU controller app Root");
}

void handleCommand() { // If a POST request is made to URI /command
  if(!server.hasArg("command") || server.arg("command") == NULL) { 
    server.send(400, "text/plain", "400: Invalid Request");   // The request is invalid, so send HTTP status 400
    return;
  }
  server.send(200, "text/plain", "got it");
  Serial.println(server.arg("plain"));
  // Get value from request
  String temp = "";
  temp = server.arg("value"); 
  int value = temp.toInt();
  if(value > 1 || value < 0) 
    value = 0;

  digitalWrite(D2, value);

  return;
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
  return;
}

//===============================================================
//                  SETUP
//===============================================================
void setup(void){
  pinMode(D2, OUTPUT);
  digitalWrite(D2, LOW);
  Serial.begin(9600);
  Serial.println("");
  WiFi.mode(WIFI_AP);           //Only Access point
  //WiFi.softAP(ssid, password);  //Start HOTspot removing password will disable security
  WiFi.softAP(ssid);

  IPAddress myIP = WiFi.softAPIP(); //Get IP address
  Serial.print("HotSpt IP:");
  Serial.println(myIP);
 
  server.on("/", HTTP_GET, handleRoot);        // Call the 'handleRoot' function when a client requests URI "/"
  server.on("/command", HTTP_POST, handleCommand); // Call the 'handRequest' function when a POST request is made to URI "/command"
  server.onNotFound(handleNotFound); 

  server.begin();                  //Start server
  Serial.println("HTTP server started");
}
//===============================================================
//                     LOOP
//===============================================================
void loop(void){
  server.handleClient();          //Handle client requests
}

