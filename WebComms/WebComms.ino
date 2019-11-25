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
  int chip;
  String cat_name;
  unsigned int permission;
};

//cat_id represents the position in cats of the cat

struct cat cats[MAX_CAT];
int nbCats = 0;

const IPAddress apIP(192, 168, 4, 1);
const char *apssid = "CoCaDo";

WebServer server(80);
int serverUp = 0;


/**** SETUP AND LOOP ****/
void setup(void) {
  m5.begin();
  Serial.begin(115200);

  fillCats();

  m5DisplayHome();
}

void fillCats() {
  struct cat garfield;
  garfield.cat_name = "Gargar";
  garfield.permission = 0;

  struct cat toto;
  toto.cat_name = "Fofo";
  toto.permission = 1;

  garfield.chip = nbCats;
  cats[nbCats++] = garfield;
  toto.chip = nbCats;
  cats[nbCats++] = toto;

  Serial.println(toto.chip + toto.cat_name + toto.permission);
  //Serial.println(cats);
}

void setupAccessPoint() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(apssid);
  WiFi.mode(WIFI_AP);

  M5.Lcd.setTextSize(1);
  M5.Lcd.println("Starting Access Point...");
  
  configServerHandlers();
  server.begin();
  
  M5.Lcd.println("Access Point OK.");

  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(40, 65);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.print("Connect to \"");
  M5.Lcd.print(apssid);
  M5.Lcd.println("\",");
  M5.Lcd.setCursor(20, 90);
  M5.Lcd.println("and go to 192.168.4.1 !");
  M5.Lcd.setCursor(120, 130);
  M5.Lcd.setTextColor(ORANGE);
  M5.Lcd.println("=^._.^=");
}

void configServerHandlers() {
  server.on("/", handleHome);
  server.on("/home", handleHome);
  server.on("/permissions", permissionsMenu);
  server.on("/catInfos", displayCatInfos);
  server.on("/updateCatInfos", updateCatInfos);
  server.on("/scanCat", scanCat);
  server.on("/addCat", addCat);
  server.onNotFound(handleNotFound);
}

void m5DisplayHome() {
  M5.Lcd.setTextColor(PINK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(65, 10);
  M5.Lcd.println("Connected CatDoor");
  M5.Lcd.setCursor(3, 55);
  M5.Lcd.println("Press button B to start");
  M5.Lcd.println("connection");
  //Arrow, vertical
  M5.Lcd.drawLine(160, 180, 160, 239, RED);
  M5.Lcd.drawLine(159, 180, 159, 239, RED);
  M5.Lcd.drawLine(161, 180, 161, 239, RED);
  //Arrow, left
  M5.Lcd.drawLine(160, 239, 140, 210, RED);
  M5.Lcd.drawLine(159, 239, 139, 210, RED);
  M5.Lcd.drawLine(161, 239, 141, 210, RED);
  //Arrow, right
  M5.Lcd.drawLine(160, 239, 180, 210, RED);
  M5.Lcd.drawLine(159, 239, 179, 210, RED);
  M5.Lcd.drawLine(161, 239, 181, 210, RED);
}

void loop(void) {
  if (M5.BtnB.wasPressed()) {
    M5.Lcd.clear(BLACK);
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.setCursor(0,0);
    setupAccessPoint();
    serverUp = 1;
  }
  
  if(serverUp) {
    server.handleClient();
  }
  
  M5.update();
}
/**** END SETUP AND LOOP ****/


/**** WEB PAGES ****/
void handleHome() {
  server.send(200, "text/html", makePage( "Home", getHomeHTML() ));
}

void permissionsMenu() {
  server.send(200, "text/html", makePage( "Permissions", getPermissionsMenuHTML() ));
}

void displayCatInfos() {
  int cat_id = server.arg("cat_id").toInt();
  String s = getPermissionsMenuHTML();
  s += getCatInfosHTML(cat_id);
  server.send(200, "text/html", makePage("Cat infos", s));
}

void updateCatInfos() {
  int cat_id = server.arg("cat_updated_id").toInt();
  int permission_updated = server.arg("permission").toInt();
  cats[cat_id].permission = permission_updated;
  permissionsMenu();
}

void scanCat() {
  String s = "<p>Please scan your cat</p>";
  server.send(200, "text/html", makePage("Scan", s));

  // TODO : SCAN HERE TO ADD A CAT

  int chip = 0; //FIXME chip = the thing scanned, identifying the cat
  s = getAddCatFormHTML(chip);
  server.send(200, "text/html", makePage("Add cat", s));
}

void addCat() {
  // retrieve form informations
  struct cat c;
  c.chip = server.arg("chip").toInt();
  c.cat_name = server.arg("cat_name");
  c.permission = server.arg("permission").toInt();
  
  cats[nbCats++] = c;

  handleHome(); // go back to main menu
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

String getHomeHTML() {
  String s = "<h1>Welcome in your Connected Catdoor manager !</h1>";
  s += "<form>";
  s += "<button type=\"submit\" formaction=\"permissions\">MANAGE PERMISSIONS</button>";
  s += "<br><br><button type=\"submit\" formaction=\"scanCat\">ADD A CAT</button>";
  s += "</form>";
  return s;
}

String getPermissionsMenuHTML() {
  String s = "<form method=\"post\" action=\"home\"><button type=\"submit\">Home</button></form>";
  s += "<br><h2>Permissions menu</h2>";
  s += "<p>Choose a cat to change its permissions:</p>";
  s += "<br><form method=\"post\" action=\"catInfos\">";
  s += "<select name=\"cat_id\">";
  for(int i=0; i<nbCats; i++) {
    s += "<option value=\"" + String(i) + "\">" + cats[i].cat_name + "</option>";
  }
  s += "</select>";
  s += "<input type=\"submit\" value=\"Choose\"></form>";
  return s;
}

String getCatInfosHTML(const int cat_id) {
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

String getAddCatFormHTML(const int chip) {
  String s = "<h2>Enter informations for scanned cat</h2>";
  s += "<form method=\"post\" action=\"addCat\">";
  s += "<input type=\"hidden\" value=\"" + String(chip) +"\" name=\"chip\" />";
  s += "<p>Name:</p><input name=\"cat_name\" length=64 type=\"text\">";
  s += "<br><label>OUT ? :</label>";
  s += "<input type=\"radio\" name=\"permission\" value=\"1\" checked> Yes";
  s += "<input type=\"radio\" name=\"permission\" value=\"0\"> No";
  s += "<br><br><input type=\"submit\" value=\"Save\"></form>";
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
