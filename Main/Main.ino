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
  unsigned int permission_in;
  unsigned int permission_out;
};

struct cat cats[MAX_CAT];
int nbCats = 0;

/**** WIFI VAR ****/
const IPAddress apIP(192, 168, 4, 1);
const char *apssid = "NeKoDo";

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


/**** ROLE VAR ****/
int isMaster; //we consider slave is out, it lets the cat get IN


void loop(void) {
  M5.update();

  if(isMaster) { // master
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

    if(Serial.available()) {
      checkChipFromSlave();
    }
    
  } else { // slave
    //TODO: when NFC detected, call checkChipFromMaster(String chip) ; returns the permission (0: can't go, 1: can go) in char
  
    if(M5.BtnA.wasPressed()) {
      if(isOpen) {
        closeServo();
      } else {
        openServo();
      }
    }
  }
}


void setup(void) {
  m5.begin();
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);

  // Init role master or slave
  Serial2.write(0);
  delay(1000); 

  setupServo();
  
  if(Serial.available()) { // master
    Serial.read(); // empty buffer
    preferences.begin("cat-data");
    setupCatData();

    setupAccessPoint();

    lcdDrawHome();

    isMaster = 1;
  } else { // slave
    isMaster = 0;
    M5.Lcd.println("SLAVE");
  }
}
