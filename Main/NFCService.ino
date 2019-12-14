// Returns the string scanned by the nfc captor
String getTagId() {
  NfcTag tag = nfc.read();
  String id = tag.getUidString();
  return id;
}

void updateTagId(String &id) {
  id = getTagId();
}
