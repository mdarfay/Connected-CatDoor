void checkChipFromSlave() {
  String chip = "";
  while(Serial.available()) {
   chip += (char) Serial.read();
 }

 for(int i=0; i<nbCats; i++) {
  if(strcmp(cats[i].chip.c_str(), chip.c_str())==0) {
    Serial.write(cats[i].permission); // TODO: put permission OUT!
    break;
  }
 }
}

char checkChipFromMaster(String chip) {
  Serial2.write(chip.c_str());
  while(!Serial2.available()); // active wait for a quick answer
  char permission = (char) Serial2.read();
  return permission;
}
