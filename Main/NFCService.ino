// Returns the string scanned by the nfc captor
String getTagId() {
  //Serial.println("\nScan a cat tag\n");
  NfcTag tag = nfc.read();
  String id = tag.getUidString();
  //Serial.println("id detected : " + id);
  return id;
}

void updateTagId(String &id) {
  id = getTagId();
}

//void loop(void) {
//    //Serial.println("\nScan a cat tag\n");
//    if (nfc.tagPresent())
//    {
//       //scanTag(id);
//       Serial.println(getTagId());
//    }
//    delay(5000);
//}
