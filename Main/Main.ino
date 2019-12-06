#include <M5Stack.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <Preferences.h>

#define MAX_CAT 10

/**** CAT VAR ****/
struct cat {
  String chip;
  String cat_name;
  unsigned int permission;
};

struct cat cats[MAX_CAT];
int nbCats = 0;

/**** WIFI VAR ****/
const IPAddress apIP(192, 168, 4, 3);
const char *apssid = "NeKoDoM2";

WebServer server(80);
int serverUp = 0;

/**** STORAGE VAR ****/
Preferences preferences;


/**** SERVO VAR ****/
// the number of the servo pin
const int servoPin = 2;  // 2 corresponds to GPIO2
int angle = 0;
// setting PWM properties
const int freq = 50;
const int servoChannel = 0;
const int resolution = 8;
int isOpen = 1;


void loop(void) {
  M5.update();
  if (M5.BtnB.wasPressed()) {
    M5.Lcd.clear(BLACK);
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.setCursor(0,0);
    turnOnAccessPoint();
    serverUp = 1;
  }
  
  if(serverUp) {
    server.handleClient();
  }

  if(M5.BtnC.wasPressed()) {
    server.close();
    serverUp = 0;
    turnOffAccessPoint();
    lcdDrawHome();
  }

  if(M5.BtnA.wasPressed()) {
    if(isOpen) {
      closeServo();
    } else {
      openServo();
    }
  }
}


void setup(void) {
  m5.begin();
  Serial.begin(115200);

  preferences.begin("cat-data");
  //setupCatData();

  setupServo();
  setupAccessPoint();

  lcdDrawHome();
}
