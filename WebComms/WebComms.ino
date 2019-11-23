/*
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

const IPAddress apIP(192, 168, 4, 1);
const char *apssid = "CoCaDo";

WebServer server(80);

String makeRoot() {
  String s = "<h1>Welcome in your cat manager !</h1>";
  s += "<form method=\"post\" action=\"catInfos\">";
  s += "<select name=\"cat_id\">";
  s += "<option value=\"garfield\">Garfield</option>";
  s += "<option value=\"toto\">Toto</option>";
  s += "</select>";
  s += "<input type=\"submit\" value=\"Search\"></form>";
  return s;
}

void handleRoot() {
  String s = makeRoot();
  server.send(200, "text/html", makePage("Home", s));
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

void setup(void) {
  m5.begin();
  Serial.begin(115200);

  setupAccessPoint();
}

String catInfos(const String cat_id) {
  String s = "<br><hr><h2>Current cat: " + cat_id + "</h2>";
  s += "<form method=\"post\" action=\"updateCat\">";
  s += "<input type=\"hidden\" value=\"" + cat_id +"\" name=\"cat_updated_id\" />";
  s += "<br><label>OUT ? :</label>";
  s += "<input type=\"radio\" name=\"rights\" value=\"1\" checked> Yes";
  s += "<input type=\"radio\" name=\"rights\" value=\"0\"> No";
  s += "<br><br><input type=\"submit\" value=\"Update\"></form>";
  return s;
}

void displayCatInfos() {
  String cat_id = server.arg("cat_id");
  String s = makeRoot();
  s += catInfos(cat_id);
  server.send(200, "text/html", makePage("Cat infos", s));
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
  server.onNotFound(handleNotFound);
}

void loop(void) {
  server.handleClient();
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
