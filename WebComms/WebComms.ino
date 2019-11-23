/*
   Web comms from: https://github.com/bbx10/WebServer_tng/tree/master/examples/AdvancedWebServer
   
   Copyright (c) 2015, Majenko Technologies
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

 * * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

 * * Redistributions in binary form must reproduce the above copyright notice, this
     list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.

 * * Neither the name of Majenko Technologies nor the names of its
     contributors may be used to endorse or promote products derived from
     this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
   ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <M5Stack.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#define MAX_CAT 10

struct cat {
  int id;
  String cat_name;
  unsigned int permission;
};

struct cat cats[MAX_CAT];
int nbCats = 0;

const IPAddress apIP(192, 168, 4, 1);
const char *apssid = "CoCaDo";

WebServer server(80);


/**** SETUP AND LOOP ****/
void setup(void) {
  m5.begin();
  Serial.begin(115200);

  fillCats();
  
  setupAccessPoint();
}

void fillCats() {
  struct cat garfield;
  garfield.cat_name = "Gargar";
  garfield.permission = 0;

  struct cat toto;
  toto.cat_name = "Fofo";
  toto.permission = 1;

  garfield.id = nbCats;
  cats[nbCats++] = garfield;
  toto.id = nbCats;
  cats[nbCats++] = toto;

  Serial.println(toto.id + toto.cat_name + toto.permission);
  //Serial.println(cats);
}

void setupAccessPoint() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(apssid);
  WiFi.mode(WIFI_AP);

  M5.Lcd.print("Starting Access Point at \"");
  M5.Lcd.print(apssid);
  M5.Lcd.println("\"");

  configServerHandlers();

  server.begin();
  M5.Lcd.print("HTTP server started");
}

void configServerHandlers() {
  server.on("/", handleRoot);
  server.on("/catInfos", displayCatInfos);
  server.on("/updateCatInfos", updateCatInfos);
  server.onNotFound(handleNotFound);
}

void loop(void) {
  server.handleClient();
}
/**** END SETUP AND LOOP ****/


/**** WEB PAGES ****/
void handleRoot() {
  String s = makeRoot();
  server.send(200, "text/html", makePage("Home", s));
}

void displayCatInfos() {
  int cat_id = server.arg("cat_id").toInt();
  String s = makeRoot();
  s += catInfos(cat_id);
  server.send(200, "text/html", makePage("Cat infos", s));
}

void updateCatInfos() {
  int cat_id = server.arg("cat_updated_id").toInt();
  int permission_updated = server.arg("permission").toInt();
  cats[cat_id].permission = permission_updated;
  handleRoot();
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}

String makeRoot() {
  String s = "<h1>Welcome in your cat manager !</h1>";
  s += "<p>Choose a cat to change its permissions:</p>";
  s += "<br><form method=\"post\" action=\"catInfos\">";
  s += "<select name=\"cat_id\">";
  for(int i=0; i<nbCats; i++) {
    s += "<option value=\"" + String(cats[i].id) + "\">" + cats[i].cat_name + "</option>";
  }
  s += "</select>";
  s += "<input type=\"submit\" value=\"Choose\"></form>";
  return s;
}

String catInfos(const int cat_id) {
  String s = "<br><hr><h2>Current cat: " + cats[cat_id].cat_name + "</h2>";
  s += "<form method=\"post\" action=\"updateCatInfos\">";
  s += "<input type=\"hidden\" value=\"" + String(cat_id) +"\" name=\"cat_updated_id\" />";
  s += "<br><label>OUT ? :</label>";
  if(cats[cat_id].permission) {
    s += "<input type=\"radio\" name=\"permission\" value=\"1\" checked> Yes";
    s += "<input type=\"radio\" name=\"permission\" value=\"0\"> No";
  } else {
    s += "<input type=\"radio\" name=\"permission\" value=\"1\"> Yes";
    s += "<input type=\"radio\" name=\"permission\" value=\"0\" checked> No";
  }
  s += "<br><br><input type=\"submit\" value=\"Update\"></form>";
  return s;
}

String makePage(String title, String contents) {
  String s = "<!DOCTYPE html><html><head>";
  s += "<meta name=\"viewport\" content=\"width=device-width,user-scalable=0\">";
  s += "<title>";
  s += title;
  s += "</title></head><body>";
  s += contents;
  s += "</body></html>";
  return s;
}
