

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
  if(!server.hasArg("magnitude") || server.arg("magnitude") == NULL || !server.hasArg("direction") || server.arg("direction") == NULL) { 
    server.send(400, "text/plain", "400: Invalid Request");   // The request is invalid, so send HTTP status 400
    return;
  }
  server.send(200, "text/plain", "got it");
  Serial.println(server.arg("plain"));

  String temp = "";
  // Get magnitude integer value
  temp = server.arg("magnitude"); 
  int mag = temp.toInt();
  // Normalize, exclude outliers
  if(mag > 1 || mag < 0) 
    mag = 0;

  // get direction integer value 0 = left, 1 = forward, 2 = right, 3 = reverse
  temp = server.arg("direction");
  int dir = temp.toInt();
  // set outliers to forward for testing
  if(dir > 3 || dir < 0)
    dir = 0;
  // set outpin based on dir
  int outpin = 0;
  switch(dir) {
    case(0):
      outpin = D1;
      break;
    case(1):
      outpin = D3;
      break;
    case(2):
      outpin = D2;
      break;
    case(3):
      outpin = D4;
      break;
    default:
      outpin = D3;
      break;
  }


  digitalWrite(outpin, mag);

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
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);

  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);

  Serial.begin(9600);
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

