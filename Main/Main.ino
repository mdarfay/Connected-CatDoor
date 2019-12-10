#include <M5Stack.h>
#include <NfcAdapter.h>
#include <PN532/PN532/PN532.h>
#include <PN532/PN532_I2C/PN532_I2C.h>
#include <Preferences.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

#define MAX_CAT 10
#define TIME_OPEN 5000

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

/**** NFC VAR ****/
PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);

/**** ROLE VAR ****/
int isMaster; //we consider slave is out, it lets the cat get IN

void setup(void) {
  m5.begin();
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);

  // Init role master or slave
  Serial2.write(0);
  delay(1000); 

  setupServo();
  //nfc.begin();

  // *** MASTER ***
  if(Serial.available()) {
    Serial.read(); // empty buffer
    preferences.begin("cat-data");
    setupCatData();

    setupAccessPoint();

    lcdDrawHome();

    isMaster = 1;
  }
  // *** SLAVE *** 
  else { // slave
    isMaster = 0;
    M5.Lcd.println("SLAVE");
  }

  M5.Lcd.println("Setup done");
}



void loop(void) {
  M5.update();

  // *** MASTER ***
  if(isMaster) {
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

//    if (nfc.tagPresent()) {
//       String chipScanned = getTagId();
//       char permission_to_get_out = getPermissionFromChip(chipScanned, 0);
//       if(permission_to_get_out == 1) {
//         openServo();
//         delay(TIME_OPEN);
//         closeServo();
//       }
//    }
    
  } // *** SLAVE ***
  else {
    M5.update();
    
//    if (nfc.tagPresent()) {
//       String chipScanned = getTagId();
//       M5.Lcd.println(chipScanned);
//       char permission_to_get_in = checkChipFromMaster(chipScanned);
//       M5.Lcd.print("Permission");
//       M5.Lcd.println(permission_to_get_in);
//       if(permission_to_get_in == 1) {
//         openServo();
//         delay(TIME_OPEN);
//         closeServo();
//       }
//    }
  
    if(M5.BtnA.wasReleased()) {
      if(isOpen) {
        closeServo();
      } else {
        openServo();
      }
    }
  }
}
