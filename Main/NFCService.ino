#include <NfcAdapter.h>
#include <PN532/PN532/PN532.h>
#include <PN532/PN532_I2C/PN532_I2C.h>

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);

//String id;

void setup(void) {
    Serial.begin(115200);
    //Serial.println("NDEF Reader");
    nfc.begin();
}

String getTagId() {
  Serial.println("\nScan a cat tag\n");
  NfcTag tag = nfc.read();
  String id = tag.getUidString();
  Serial.println("id detected : " + id);
  return id;
}

void updateTagId(String &id) {
  id = getTagId();
}

void loop(void) {
    //Serial.println("\nScan a cat tag\n");
    if (nfc.tagPresent())
    {
       //scanTag(id);
       Serial.println(getTagId());
    }
    delay(5000);
}
