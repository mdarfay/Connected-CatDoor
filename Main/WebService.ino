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

/**** WEB SET UP ****/
void setupAccessPoint() {
  M5.Lcd.setTextSize(1);
  M5.Lcd.println("Starting Access Point...");
  
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  
  configServerRoutes();
}

void turnOnAccessPoint() {
  WiFi.softAP(apssid);
  server.begin();

  lcdDrawConnectionPage();
}

void turnOffAccessPoint() {
  WiFi.softAPdisconnect(false);
}

void configServerRoutes() {
  server.on("/", handleHome);
  server.on("/home", handleHome);
  server.on("/permissions", permissionsMenu);
  server.on("/catInfos", displayCatInfos);
  server.on("/updateCatInfos", updateCatInfos);
  server.on("/scanNotice", scanNotice);
  server.on("/scanCat", scanCat);
  server.on("/addCat", addCat);
  server.on("/deletionMenu", deletionMenu);
  server.on("/deleteCat", deleteCat);
  server.on("/catsLocation", catsLocation);
  server.onNotFound(handleNotFound);
}
/**** END SET UP ****/


/**** WEB HANDLERS ****/
void handleHome() {
  server.send(200, "text/html", makePage( "Home", getHomeHTML() ));
}

void permissionsMenu() {
  server.send(200, "text/html", makePage( "Permissions", getPermissionsMenuHTML() ));
}

void displayCatInfos() {
  //cat_id represents the position in cats of the cat
  int cat_id = server.arg("cat_id").toInt();
  String s = getPermissionsMenuHTML();
  s += getCatInfosHTML(cat_id);
  server.send(200, "text/html", makePage("Cat infos", s));
}

void updateCatInfos() {
  int cat_id = server.arg("cat_updated_id").toInt();
  int permission_in_updated = server.arg("permission_in").toInt();
  int permission_out_updated = server.arg("permission_out").toInt();
  cats[cat_id].permission_in = permission_in_updated;
  cats[cat_id].permission_out = permission_out_updated;

	saveCatData(cat_id, false);

  permissionsMenu();
}

void scanNotice() {
  String s = "<h3>Grab your cat and prepare for scanning!</h3>";
  s += "<p>Follow instructions on LCD display of M5Stack on your Connected CatDoor.</p>";
  s += "<br><form method=\"post\" action=\"scanCat\"><button type=\"submit\">Begin scan</button></form>";
  server.send(200, "text/html", makePage("Scan", s));
}

void scanCat() {
  M5.Lcd.clear(BLACK);
  M5.Lcd.setCursor(0,0);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);
  M5.Lcd.println("   Put your cat near the");
  M5.Lcd.println("       chip captor.");
  M5.Lcd.println("");
  M5.Lcd.println("   You have 10s to scan");
  M5.Lcd.println("        your cat.");

  int scanOK = 0;
  String chipScanned = "None";
  M5.Lcd.setTextSize(3);
  for(int i=10; i>0; i--) {
    if (nfc.tagPresent(100) && !scanOK) {
      scanOK = 1;
      chipScanned = getTagId();
      M5.Lcd.setCursor(100, 180);
      M5.Lcd.println("SCAN OK");
    }
    
    M5.Lcd.setCursor(150, 130);
    M5.Lcd.setTextColor(BLACK);
    int tmp = i+1;
    M5.Lcd.println(tmp);
    M5.Lcd.setCursor(150, 130);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.println(i);
    delay(1000);
  }

  M5.Lcd.clear(BLACK);
  if (scanOK) {
    String s = getAddCatFormHTML(chipScanned);
    M5.Lcd.setCursor(0,0);
    M5.Lcd.setTextSize(2);
    M5.Lcd.println(" Look on your phone/computer to finish adding your cat!");
    server.send(200, "text/html", makePage("Add cat", s));
  } else {
    lcdDrawConnectionPage();
    handleHome();
  }
}

void addCat() {
  // retrieve form informations
  struct cat c;
  c.chip = server.arg("chip");
  c.cat_name = server.arg("cat_name");
  c.permission_in = server.arg("permission_in").toInt();
  c.permission_out = server.arg("permission_out").toInt();
  c.is_out = -1;
  
  cats[nbCats++] = c;

  saveCatData(nbCats - 1, true);

  M5.Lcd.clear(BLACK);
  handleHome(); // go back to main menu
  lcdDrawConnectionPage();
}

void deletionMenu() {
  server.send(200, "text/html", makePage( "Deletion", getDeletionMenuHTML() ));
}

void deleteCat() {
  int cat_id = server.arg("cat_id").toInt();
  cats[cat_id] = cats[--nbCats];

	deleteCatData(cat_id);

  handleHome();
}

void catsLocation() {
  server.send(200, "text/html", makePage( "Cat finder", getCatLocationHTML() ));
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
/**** END WEB HANDLERS ****/


/**** HTML PAGES ****/
String getHomeHTML() {
  String s = "<h1>Welcome in your Connected Catdoor manager !</h1>";
  s += "<form>";
  s += "<button type=\"submit\" formaction=\"permissions\">MANAGE PERMISSIONS</button>";
  s += "<br><br><button type=\"submit\" formaction=\"catsLocation\">FIND YOUR CATS</button>";
  s += "<br><br><button type=\"submit\" formaction=\"scanNotice\">ADD A CAT</button>";
  s += "<br><br><button type=\"submit\" formaction=\"deletionMenu\">REMOVE A CAT</button>";
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
  s += "<br><label>IN? :    </label>";
  if(cats[cat_id].permission_in) {
    s += "<input type=\"radio\" name=\"permission_in\" value=\"1\" checked> Yes";
    s += "<input type=\"radio\" name=\"permission_in\" value=\"0\"> No";
  } else {
    s += "<input type=\"radio\" name=\"permission_in\" value=\"1\"> Yes";
    s += "<input type=\"radio\" name=\"permission_in\" value=\"0\" checked> No";
  }
  s += "<br><label>OUT? :</label>";
  if(cats[cat_id].permission_out) {
    s += "<input type=\"radio\" name=\"permission_out\" value=\"1\" checked> Yes";
    s += "<input type=\"radio\" name=\"permission_out\" value=\"0\"> No";
  } else {
    s += "<input type=\"radio\" name=\"permission_out\" value=\"1\"> Yes";
    s += "<input type=\"radio\" name=\"permission_out\" value=\"0\" checked> No";
  }
  s += "<br><br><input type=\"submit\" value=\"Update\"></form>";
  return s;
}

String getAddCatFormHTML(const String chip) {
  String s = "<h2>Enter informations for scanned cat</h2>";
  s += "<form method=\"post\" action=\"addCat\">";
  s += "<label>Chip scanned: " + chip +"</label>";
  s += "<br><input type=\"hidden\" value=\"" + chip +"\" name=\"chip\" />";
  s += "<label>Name: </label><input name=\"cat_name\" length=64 type=\"text\">";
  s += "<br><br><label>IN? :</label>";
  s += "<input type=\"radio\" name=\"permission_in\" value=\"1\" checked> Yes";
  s += "<input type=\"radio\" name=\"permission_in\" value=\"0\"> No";
  s += "<br><br><label>OUT? :</label>";
  s += "<input type=\"radio\" name=\"permission_out\" value=\"1\" checked> Yes";
  s += "<input type=\"radio\" name=\"permission_out\" value=\"0\"> No";
  s += "<br><br><input type=\"submit\" value=\"Save\"></form>";
  return s;
}

String getDeletionMenuHTML() {
  String s = "<form method=\"post\" action=\"home\"><button type=\"submit\">Home</button></form>";
  s += "<br><h2>Remove cat menu</h2>";
  s += "<p>Choose a cat to remove it:</p>";
  s += "<br><form method=\"post\" action=\"deleteCat\">";
  s += "<select name=\"cat_id\">";
  for(int i=0; i<nbCats; i++) {
    s += "<option value=\"" + String(i) + "\">" + cats[i].cat_name + "</option>";
  }
  s += "</select>";
  s += "<input type=\"submit\" value=\"Remove\"></form>";
  return s;
}

String getCatLocationHTML() {
  String s = "<form method=\"post\" action=\"home\"><button type=\"submit\">Home</button></form>";
  s += "<h2>Find where your cats are</h2>";
  for(int i=0; i<nbCats; i++) {
    s += "<p>" + cats[i].cat_name + ": ";
    if (cats[i].is_out == 0) {
      s += "IN";
    } else if (cats[i].is_out == 1) {
      s += "OUT";
    } else {
      s += "UNKNOWN";
    }
    s += "</p>";
  }
  s += "<form method=\"post\" action=\"catsLocation\"><button type=\"submit\">Refresh</button></form>";
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
/**** END HTML PAGES ****/
